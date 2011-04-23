#ifndef __WIFIDIALOG_H__
#define __WIFIDIALOG_H__

#include "ui_WifiDialog.h"
#include "../Keyboard/QwertyKeypad.h"

struct WifiPort{
    QString     ssid;
    QString     ap;
    QString     netType;
    QString     key;
    QString     encryption;
    QString     authentication;
    QString     allocation;
    QString     encoding;
    QString     gateway;
    QString     netmask;
    QString     ip;
    QString     nameserver1;
    QString     nameserver2;
    QString     txRate;
    QStringList rates;
};

class WifiDialog : public QDialog, private Ui::WifiDialog
{
    Q_OBJECT

public:
    WifiDialog(struct WifiPort *wp, QWidget *parent = 0);

private:
    QString m_encoding;
    struct WifiPort *port;

public slots:
    void encodingChanged();
    void acceptData();
    void keyInput();
};

#endif // __WIFIDIALOG_H__
