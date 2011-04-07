#include "qcbclinuxinput.h"

#include <QSocketNotifier>
#include <QStringList>
#include <QFile>

#include <qplatformdefs.h>

#include <errno.h>
#include <termios.h>

#include <linux/kd.h>
#include <linux/input.h>

class QCBCLinuxKbPrivate : public QObject
{
    Q_OBJECT
public:
    QCBCLinuxKbPrivate(QCBCLinuxInputKeyboardHandler *h, const QString &device);
    ~QCBCLinuxKbPrivate();

private:
    bool openDevice();
    void closeDevice();
    void switchLed(int, bool);


private Q_SLOTS:
    void readKeycode();

private:
    QCBCLinuxInputKeyboardHandler   *m_handler;
    int                             m_proc_fd;
    int                             m_fd;
    int                             m_tty_fd;
    struct termios                  m_tty_attr;
    int                             m_orig_kbmode;
    QString                         m_dev;
    int                             m_repeat_delay;
    int                             m_repeat_rate;
};



QCBCLinuxInputKeyboardHandler::QCBCLinuxInputKeyboardHandler(const QString &device)
    : QWSKeyboardHandler(device)
{
    d = new QCBCLinuxKbPrivate(this, device);
}

QCBCLinuxInputKeyboardHandler::~QCBCLinuxInputKeyboardHandler()
{
    delete d;
}

bool QCBCLinuxInputKeyboardHandler::filterInputEvent(quint16 &, qint32 &)
{
    return false;
}


QCBCLinuxKbPrivate::QCBCLinuxKbPrivate(QCBCLinuxInputKeyboardHandler *h, const QString &device)
    : m_handler(h),m_proc_fd(-1), m_fd(-1), m_tty_fd(-1), m_orig_kbmode(K_XLATE)
{
    setObjectName(QLatin1String("CBC LinuxInputSubsystem Keyboard Handler"));

    m_dev = QLatin1String("/dev/input/event1");
    m_repeat_delay = -1;
    m_repeat_rate = -1;

    // set the Repeat delay and rate of the keyboard
    // this is based on the values input from the export QWS_KEYBOARD
    // environment variable
    QStringList args = device.split(QLatin1Char(':'));
    foreach (const QString &arg, args){
        if(arg.startsWith(QLatin1String("repeat-delay=")))
            m_repeat_delay = arg.mid(13).toInt();
        else if(arg.startsWith(QLatin1String("repeat-rate=")))
            m_repeat_rate = arg.mid(12).toInt();
        else if(arg.startsWith(QLatin1String("/dev/")))
            m_dev = arg;
    }

    if(!openDevice()){
        qWarning("NO keyboard input device '%s': %s", qPrintable(m_dev), strerror(errno));
        return;
    }
}

bool QCBCLinuxKbPrivate::openDevice()
{
    m_fd = ::open(m_dev.toLocal8Bit().constData(), O_RDWR, 0);

    if (m_fd < 0)
        return false;

    if(m_repeat_delay > 0 && m_repeat_rate > 0){
        int kbdrep[2] = { m_repeat_delay, m_repeat_rate };
        ::ioctl(m_fd, EVIOCSREP, kbdrep);
    }

    QSocketNotifier *notifier;
    notifier = new QSocketNotifier(m_fd, QSocketNotifier::Read, this);
    connect(notifier, SIGNAL(activated(int)), this, SLOT(readKeycode()));

    m_tty_fd = isatty(0) ? 0:-1;

    if(m_tty_fd >= 0){
        tcgetattr(m_tty_fd, &m_tty_attr);
        struct ::termios termdata;
        tcgetattr(m_tty_fd, &termdata);
        ::ioctl(m_tty_fd, KDGKBMODE, &m_orig_kbmode);
        ::ioctl(m_tty_fd, KDSKBMODE, K_MEDIUMRAW);

        termdata.c_iflag = (IGNPAR | IGNBRK) & (~PARMRK) & (~ISTRIP);
        termdata.c_oflag = 0;
        termdata.c_cflag = CREAD | CS8;
        termdata.c_lflag = 0;
        termdata.c_cc[VTIME]=0;
        termdata.c_cc[VMIN]=1;
        cfsetispeed(&termdata, 9600);
        cfsetospeed(&termdata, 9600);
        tcsetattr(m_tty_fd, TCSANOW, &termdata);
    }
    return true;
}

void QCBCLinuxKbPrivate::closeDevice()
{
    if(m_tty_fd >=0){
        ::ioctl(m_tty_fd, KDSKBMODE, m_orig_kbmode);
        tcsetattr(m_tty_fd, TCSANOW, &m_tty_attr);
    }
    if(m_fd >= 0)
        ::close(m_fd);
}

QCBCLinuxKbPrivate::~QCBCLinuxKbPrivate()
{
    this->closeDevice();
}

void QCBCLinuxKbPrivate::switchLed(int led, bool state)
{
    struct ::input_event led_ie;
    ::gettimeofday(&led_ie.time, 0);
    led_ie.type = EV_LED;
    led_ie.code = led;
    led_ie.value = state;

    QT_WRITE(m_fd, &led_ie, sizeof(led_ie));
}

void QCBCLinuxKbPrivate::readKeycode()
{
    struct ::input_event buffer[32];
    int n = 0;

    forever {
        n = QT_READ(m_fd, reinterpret_cast<char *>(buffer) + n, sizeof(buffer) - n);

        if (n == 0) {
            qWarning("Got EOF from the input device.");
            return;
        } else if (n < 0 && (errno != EINTR && errno != EAGAIN)) {
            qWarning("Could not read from input device: %s", strerror(errno));
            return;
        } else if (n % sizeof(buffer[0]) == 0) {
            break;
        }
    }

    n /= sizeof(buffer[0]);

    for (int i = 0; i < n; ++i) {
        if (buffer[i].type != EV_KEY)
            continue;

        quint16 code = buffer[i].code;
        qint32 value = buffer[i].value;

        if (m_handler->filterInputEvent(code, value))
            continue;

        QWSKeyboardHandler::KeycodeAction ka;
        ka = m_handler->processKeycode(code, value != 0, value == 2);

        switch (ka) {
        case QWSKeyboardHandler::CapsLockOn:
        case QWSKeyboardHandler::CapsLockOff:
            switchLed(LED_CAPSL, ka == QWSKeyboardHandler::CapsLockOn);
            break;

        case QWSKeyboardHandler::NumLockOn:
        case QWSKeyboardHandler::NumLockOff:
            switchLed(LED_NUML, ka == QWSKeyboardHandler::NumLockOn);
            break;

        case QWSKeyboardHandler::ScrollLockOn:
        case QWSKeyboardHandler::ScrollLockOff:
            switchLed(LED_SCROLLL, ka == QWSKeyboardHandler::ScrollLockOn);
            break;

        default:
            // ignore console switching and reboot
            break;
        }
    }
}


#include "qcbclinuxinput.moc"
