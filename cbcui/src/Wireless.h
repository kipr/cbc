#ifndef __WIRELESS_H__
#define __WIRELESS_H__

#include "ui_Wireless.h"
#include "Page.h"
#include <QProcess>
//#include <QTimer>
#include <QtGui>
#include "Keyboard/QwertyKeypad.h"

struct WifiPort{
    QString     ssid;
    QString     hwaddr;
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
    int         txRate;
};

class wifiDialog;

class Wireless : public Page, private Ui::Wireless
{
  Q_OBJECT
public:
    Wireless(QWidget *parent = 0);
    ~Wireless();

public slots:
    void show();
    void hide();

    void ssidScan();
    void getIPaddress(int exitCode = 0);
    void listRefresh(int exitCode = 0);
    void on_ui_connectButton_clicked();

private:
    QString         m_cbcSSID;
    QString         m_cbcIP;
    QProcess        *m_ssidScan;
    QProcess        *m_netStart;
    struct WifiPort m_connectedWifi;
    //QTimer      *m_refreshTimer;

    void addSsidToList(QString net);
};

class wifiDialog : public QDialog
{
    Q_OBJECT

    public:
        wifiDialog(struct WifiPort *wp, QWidget *parent = 0);

        QComboBox   *allocCombo;
        QComboBox   *authCombo;
        QComboBox   *encrypCombo;
        QCheckBox   *AsciiCheck;
        QCheckBox   *HexCheck;
        QLineEdit   *keyLineEdit;

        QPushButton *connectButton;
        QPushButton *cancelButton;

    private:
        struct WifiPort *port;
        QString         m_encoding;
        QLabel          ssid;
        QLabel          hwaddr;
        QLabel          alloc;
        QLabel          auth;
        QLabel          encryp;
        QLabel          key;

    public slots:
        void encodingChanged();
        void acceptData();
        void keyInput();
};

#endif // __WIRELESS_H__
