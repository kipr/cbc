#ifndef __SERIALPAGE_H__
#define __SERIALPAGE_H__

#include "ui_Serial.h"
#include "Page.h"
#include "SerialPort.h"
#include <QThread>
#include <QMutex>

class SerialThread : public QThread
{
    Q_OBJECT
public:
    SerialThread(QObject *parent=0);
    ~SerialThread();

    void stop();

signals:
    void portList(QStringList);
    void newData(QByteArray);
    void baudSetting(int);

public slots:
    void setActivePort(int index);
    void setBaudRate(int baud);
    void sendCommand(QByteArray ba);

protected:
    void run();

private:
    QMutex                      m_mutex;
    QByteArray                  m_command;
    int                         m_baudSetting;
    int                         m_activePort;
    bool                        m_quit;

    void populatePorts(QList<SerialPort*> &pl);
};

class SerialPage : public Page, private Ui::SerialPage
{
  Q_OBJECT
public:
    SerialPage(QWidget *parent = 0);
    ~SerialPage();

signals:
    void newBaudRate(int);
    void sendCommand(QByteArray);

public slots:
    void show();
    void hide();

    void updatePortList(QStringList pl);
    void updateBaud(int baud);
    void consoleUpdate(QByteArray ba);
    void setBaudRate(int index);
    void on_ui_commandLineEdit_selectionChanged();
    void on_ui_sendButton_clicked();

private:
    struct _symLink{
        QString dev;
        int linkNum;
    };

    SerialThread        m_sThread;
};

#endif // SERIALPAGE_H
