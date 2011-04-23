#ifndef __WIRELESS_H__
#define __WIRELESS_H__

#include "ui_Wireless.h"
#include "WifiDialog.h"
#include "../Page.h"
#include <QProcess>
#include <QTimer>
#include <QtGui>


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
    void listRefresh(int exitCode = 0);
    void on_ui_connectButton_clicked();
    void blinky();

private:
    QList<WifiPort> m_netList;
    struct WifiPort m_connectedWifi;
    QProcess        *m_ssidScan;
    QProcess        *m_netStart;

    QTimer          *m_commAnima;
    //QTimer      *m_refreshTimer;

    void addSsidToList(QString net);
    void userMessage(QString msg);
};

#endif // __WIRELESS_H__
