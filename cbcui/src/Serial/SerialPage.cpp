#include "SerialPage.h"
#include <QFile>
#include "../Keyboard/Keypad.h"
#include "../Keyboard/QwertyKeypad.h"

#define MAXPORTCOUNT 6

SerialThread::SerialThread(QObject *parent) : QThread(parent),m_activePort(-1)
{
    m_quit = false;
}

SerialThread::~SerialThread()
{
    stop();
}

void SerialThread::stop()
{
    m_mutex.lock();
    m_quit = true;
    m_mutex.unlock();

    wait();
}

void SerialThread::run()
{
    int port = -1;
    int baud;
    QList<SerialPort*> portList;
    populatePorts(portList);
    this->m_command.clear();

    m_quit = false;
    while(!m_quit){

        if(port != this->m_activePort)
        {
            if(port >= 0 && port < portList.size())
                portList[port]->close();

            m_mutex.lock();
            port = this->m_activePort;
            m_mutex.unlock();

            if(port < 0 || port >= portList.size())
                continue;

            portList[port]->open(QIODevice::ReadWrite);
            baud = portList[port]->getBaudRate();

            m_mutex.lock();
            this->m_baudSetting = baud;
            m_mutex.unlock();
            emit baudSetting(baud);
        }
        else if(port < 0 || port >= portList.size()){
            msleep(100);
            continue;
        }

        if(baud != this->m_baudSetting){
            m_mutex.lock();
            baud = this->m_baudSetting;
            m_mutex.unlock();

            portList[port]->setBaudRate(baud);
        }

        if(this->m_command.size() > 0){
            m_mutex.lock();
            QByteArray ba = this->m_command;
            this->m_command.clear();
            m_mutex.unlock();

            portList[port]->write(ba);
        }

        int count = portList[port]->bytesAvailable();
        if(count > 0){
            QByteArray oba = portList[port]->read(count);
            emit newData(oba);
        }

        QThread::yieldCurrentThread();
    }

    if(port >= 0 && port < portList.size())
        portList[port]->close();

    while(portList.size()){
        delete portList.last();
        portList.removeLast();
    }
}

void SerialThread::populatePorts(QList<SerialPort*> &pl)
{
   QStringList portNames;
    // add the CBC Create port first
    if(QFile::exists("/dev/uart1")){
        SerialPort *spt = new SerialPort("/dev/uart1");
        pl.append(spt);
        portNames << "/dev/uart1";
    }

    // find all the external usb-serial ports
    int i=0;
    QString portName = QString("/dev/ttyUSB%1").arg(i);
    while(i < MAXPORTCOUNT)
    {
        if(QFile::exists(portName)){
            SerialPort *spt = new SerialPort(portName,this);
            pl.append(spt);
            portNames << portName;
        }
        i++;
        portName = QString("/dev/ttyUSB%1").arg(i);
    }
    emit portList(portNames);
}

void SerialThread::setActivePort(int index)
{
    QMutexLocker locker(&m_mutex);
    m_activePort = index;
}

void SerialThread::setBaudRate(int baud)
{
    QMutexLocker locker(&m_mutex);
    m_baudSetting = baud;
}

void SerialThread::sendCommand(QByteArray ba)
{
    QMutexLocker locker(&m_mutex);
    m_command = ba;
}

/*#################################################################################*/

SerialPage::SerialPage(QWidget *parent) : Page(parent)
{
    setupUi(this);

    ui_outputConsole->ensureCursorVisible();

    connect(&m_sThread, SIGNAL(newData(QByteArray)), this, SLOT(consoleUpdate(QByteArray)));
    connect(&m_sThread, SIGNAL(baudSetting(int)), this, SLOT(updateBaud(int)));
    connect(&m_sThread, SIGNAL(portList(QStringList)), this, SLOT(updatePortList(QStringList)));
    connect(this, SIGNAL(newBaudRate(int)), &m_sThread, SLOT(setBaudRate(int)));
    connect(this, SIGNAL(sendCommand(QByteArray)), &m_sThread, SLOT(sendCommand(QByteArray)));
    connect(ui_portComboBox, SIGNAL(currentIndexChanged(int)), &m_sThread, SLOT(setActivePort(int)));
    connect(ui_baudComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setBaudRate(int)));

    // populate the symbolic link name list
    //ui_symbolicComboBox->clear();
    //for(int i=0;i<m_portList.size();i++){
      //  ui_symbolicComboBox->addItem(QString("serial%1").arg(i));
        //m_portList[i].linkName = QString("serial%1").arg(i);
    //}
}

SerialPage::~SerialPage()
{
    m_sThread.stop();
}

void SerialPage::show()
{
    m_sThread.start();
    Page::show();
}

void SerialPage::hide()
{
    m_sThread.stop();
    Page::hide();
}

void SerialPage::updatePortList(QStringList pl)
{
    ui_portComboBox->clear();
    ui_portComboBox->addItems(pl);
    ui_portComboBox->setCurrentIndex(0);
}

void SerialPage::updateBaud(int baud)
{
    ui_baudComboBox->setCurrentIndex(ui_baudComboBox->findText(QString::number(baud)));
}

void SerialPage::consoleUpdate(QByteArray ba)
{
    if(ui_byteCodeCheckBox->isChecked()){
        for(int i=0;i<ba.size();i++)
            ui_outputConsole->appendPlainText(QString::number(ba.at(i)));
    }
    else
        ui_outputConsole->insertPlainText(QString(ba));

    ui_outputConsole->centerCursor();
}

void SerialPage::setBaudRate(int index)
{
    emit newBaudRate(ui_baudComboBox->itemText(index).toInt());
}


void SerialPage::on_ui_commandLineEdit_selectionChanged()
{
    ui_commandLineEdit->setStyleSheet("QLineEdit#ui_commandLineEdit{background-color:red}");
    QString value;

    if(ui_byteCodeCheckBox->isChecked()){
        Keypad *user_keypad = Keypad::instance();
        user_keypad->setRange(-1,-1);
        user_keypad->setType(1);
        user_keypad->exec();
        value = user_keypad->getString();
    }
    else{
        QwertyKeypad *user_keypad = QwertyKeypad::instance(ui_commandLineEdit->text());
        user_keypad->exec();
        value = user_keypad->getString();
    }

    ui_commandLineEdit->setText(value);
    ui_commandLineEdit->setStyleSheet("QLineEdit#ui_commandLineEdit{background-color:white}");
}


void SerialPage::on_ui_sendButton_clicked()
{
    QByteArray ba;
    // write the command to the device
    if(ui_byteCodeCheckBox->isChecked()){
        // byte codes are separated by spaces
        QStringList args = ui_commandLineEdit->text().split(QLatin1Char(' '));

        foreach (const QString &arg, args){
            ba.append(arg.toInt());
            //ui_outputConsole->insertPlainText(arg);
        }
    }
    else{
        //send the command as ascii code
        ba = ui_commandLineEdit->text().toLocal8Bit();
    }
    emit sendCommand(ba);
}
