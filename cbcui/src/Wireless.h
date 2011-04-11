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
    QString     key;
    QString     allocation;
    QString     authentication;
    bool        asciiEncoding;
    QString     encryption;
    int         txRate;
    QString     ip;
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
    void listRefresh();
    void on_ui_connectButton_clicked();
    void on_ui_refreshButton_clicked();

private:
    QProcess        *m_ssidScan;
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
        QLabel          ssid;
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
