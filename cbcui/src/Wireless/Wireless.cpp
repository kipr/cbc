#include "Wireless.h"
#include "WifiItem.h"
#include "../MainWindow.h"
#include <QFile>

Wireless::Wireless(QWidget *parent) : Page(parent)
{
    setupUi(this);
    connect(ui_refreshButton, SIGNAL(clicked()), this, SLOT(ssidScan()));

    m_ssidScan = new QProcess(this);
    connect(m_ssidScan, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(listRefresh(int)));
    m_netStart = new QProcess(this);
    connect(m_netStart, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(listRefresh(int)));

    m_commAnima = new QTimer(this);
    m_commAnima->setInterval(150);
    connect(m_commAnima, SIGNAL(timeout()), this, SLOT(blinky()));

    // have to wait for the system to load before calling getWifiConnection()
    QTimer::singleShot(5000,this, SLOT(listRefresh()));

    //m_refreshTimer = new QTimer(this);
    //connect(m_refreshTimer, SIGNAL(timeout()), this, SLOT(ssidScan()));
}

void Wireless::show()
{
    this->ssidScan();
    //m_refreshTimer->start(6000);
    Page::show();}

void Wireless::hide()
{
    //m_refreshTimer->stop();
    Page::hide();
}

Wireless::~Wireless()
{
    m_netStart->kill();
    delete m_netStart;
    m_ssidScan->kill();
    delete m_ssidScan;
    if(m_commAnima->isActive())
        m_commAnima->stop();
    delete m_commAnima;
}

void Wireless::ssidScan()
{
    if(m_ssidScan->state() == QProcess::NotRunning)
    {
        m_commAnima->start();
        ui_connectButton->setEnabled(false);

        QProcess macID;
        macID.start("/mnt/kiss/wifi/wifi_macAddr");
        macID.waitForFinished(100);

        this->userMessage(QString("Scanning for Networks!!!\n\nmac address:\n") + QString(macID.readAllStandardOutput()));
        m_ssidScan->start("/mnt/kiss/wifi/wifi_scan.pl");
    }
    else
        this->userMessage("CHILL!\nI'm still scanning!!!");
}

void Wireless::listRefresh(int exitCode)
{
    if(!m_netList.isEmpty())
        m_netList.clear();

    QProcess netStat;
    netStat.start("/mnt/kiss/wifi/wifi_connected.pl");
    netStat.waitForFinished(100);
    m_connectedWifi.ssid = QString(netStat.readAllStandardOutput());

    netStat.start("/mnt/kiss/wifi/wifi_ip.pl");
    netStat.waitForFinished(100);
    m_connectedWifi.ip = QString(netStat.readAllStandardOutput());

    ui_ipLabel->setText(m_connectedWifi.ssid + "\n" + m_connectedWifi.ip);

    if(m_connectedWifi.ip.isEmpty() || m_connectedWifi.ssid.isEmpty())
        MainWindow::instance()->stopWifiCheck();
    else
        MainWindow::instance()->checkWifiSignal();

    if(!this->isVisible())
        return;

    QFile fn("/mnt/kiss/wifi/ssids");
    if(m_commAnima->isActive())
        m_commAnima->stop();
    ui_connectButton->setText("Connect");

    if(exitCode == 0 && fn.exists()){
        ui_ssidListWidget->clear();

        fn.open(QIODevice::ReadOnly);

        while(!fn.atEnd()){
            QString net(fn.readLine());
            this->addSsidToList(net);
        }
        fn.close();
        ui_connectButton->setEnabled(true);
    }   
    else{
        if(exitCode == 1)
            this->userMessage("No Networks Available");
        else if(exitCode == 2)
            this->userMessage("No WiFi card detected");
        else if(exitCode == 3)
            this->userMessage("Network Error\nCheck settings");

        ui_connectButton->setEnabled(false);
    }
}

void Wireless::addSsidToList(QString net)
{
    struct WifiPort wp;

    // get the SSID name
    wp.ssid = net.section("  ",2,2);
    wp.ap = net.section("  ",3,3);
    wp.encryption = net.section("  ",4,4);
    wp.authentication = net.section("  ",5,5);
    if(wp.authentication.contains('2'))
        wp.authentication = "WPA2PSK";
    else if(wp.authentication.contains('-'))
        wp.authentication = "WPAPSK";
    wp.netType = net.section("  ",6,6);

    int quality = net.section("  ",7,7).toInt();
    wp.rates = net.section("  ",8,8).split(";");

    WifiItem *ssid = new WifiItem(wp.ssid);
    ssid->setSignalImage(quality);

    if(wp.authentication == "OPEN" || wp.authentication == "SHARED")
        ssid->setLockEnabled(false);
    else
        ssid->setLockEnabled(true);
    QListWidgetItem *itm = new QListWidgetItem();

//    QListWidgetItem *ssid = new QListWidgetItem(wp.ssid);
//    ssid->setData(Qt::UserRole,wp.ap);
//    // format the list item
//    QSize s = ssid->sizeHint();
//    s.setHeight(35);
//    ssid->setSizeHint(s);
//    QFont f = ssid->font();
//    f.setPointSize(14);
//    f.setBold(true);
//    ssid->setFont(f);

//    QIcon wifiIcon;
//    if(quality > 75)
//        wifiIcon.addFile(":/actions/botguy_wifi3.png");
//    else if(quality > 50)
//        wifiIcon.addFile(":/actions/botguy_wifi2.png");
//    else if(quality > 25)
//        wifiIcon.addFile(":/actions/botguy_wifi1.png");
//    else
//        wifiIcon.addFile(":/actions/botguy_wifi0.png");
//    ssid->setIcon(wifiIcon);

    if(wp.ssid == m_connectedWifi.ssid)
        itm->setBackgroundColor(Qt::green);

    //ui_ssidListWidget->addItem(ssid);
    itm->setSizeHint(ssid->size());
    ui_ssidListWidget->addItem(itm);
    ui_ssidListWidget->setItemWidget(itm,ssid);
    m_netList << wp;
}

void Wireless::on_ui_connectButton_clicked()
{
    int index = ui_ssidListWidget->currentRow();
    if(index < 0)
        return;

    WifiDialog wDialog(&m_netList[index],this);
    if(wDialog.exec() == QDialog::Accepted)
    {
        m_connectedWifi = m_netList[index];

        if(m_connectedWifi.encryption.contains(','))
            m_connectedWifi.encryption = "AES";

        QFile netFile("/psp/cbc_net_config");
        if(netFile.exists()){
            QFile::remove("/psp/old_cbc_net_config");
            netFile.copy("/psp/old_cbc_net_config");
        }
        netFile.remove();
        if(!netFile.open(QIODevice::ReadWrite | QIODevice::Text)){
            qWarning("Open error on network_config file");
            return;
        }

        QTextStream netConfig(&netFile);
        netConfig << "type=wlan\n";
        netConfig << "ssid=" << m_connectedWifi.ssid << "\n";
        netConfig << "ap=" << m_connectedWifi.ap << "\n";
        netConfig << "netType=" << m_connectedWifi.netType << "\n";
        netConfig << "key=" << m_connectedWifi.key << "\n";
        netConfig << "encryption=" << m_connectedWifi.encryption << "\n";
        netConfig << "auth=" << m_connectedWifi.authentication << "\n";
        netConfig << "allocation=" << m_connectedWifi.allocation << "\n";
        netConfig << "encoding=" << m_connectedWifi.encoding << "\n";
        netConfig << "gateway=" << m_connectedWifi.gateway << "\n";
        netConfig << "netmask=" << m_connectedWifi.netmask << "\n";
        netConfig << "ip=" << m_connectedWifi.ip << "\n";
        netConfig << "nameserver1=" << m_connectedWifi.nameserver1 << "\n";
        netConfig << "nameserver2=" << m_connectedWifi.nameserver2 << "\n";
        netConfig << "txrate=" << m_connectedWifi.txRate << "\n";

        netFile.close();

        if(m_netStart->state() == QProcess::NotRunning){
            m_netStart->start("/mnt/kiss/wifi/wifi_start.pl");
            ui_connectButton->setEnabled(false);
            m_commAnima->start();
        }
    }
}

void Wireless::userMessage(QString msg)
{
    ui_ssidListWidget->clear();

    QListWidgetItem *msgItem = new QListWidgetItem(msg);
    // format the list item
    QSize s = msgItem->sizeHint();
    s.setHeight(172);
    msgItem->setSizeHint(s);
    QFont f = msgItem->font();
    f.setPointSize(20);
    f.setBold(true);
    msgItem->setFont(f);
    msgItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    msgItem->setTextColor(Qt::red);

    ui_ssidListWidget->addItem(msgItem);
}

void Wireless::blinky()
{
    static int i=0;

    switch(i){
    case 0:
        ui_connectButton->setText("|");
        break;
    case 1:
        ui_connectButton->setText("( | )");
        break;
    case 2:
        ui_connectButton->setText("(( | ))");
        break;
    case 3:
        ui_connectButton->setText("((( | )))");
        break;
    case 4:
        ui_connectButton->setText("((  |  ))");
        break;
    case 5:
        ui_connectButton->setText("(   |   )");
        break;
    }
    i++;
    if(i>5) i=0;
}
