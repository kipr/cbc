#include "Wireless.h"

#include <QFile>

Wireless::Wireless(QWidget *parent) : Page(parent)
{
  setupUi(this);
    m_ssidScan = new QProcess(this);
    connect(m_ssidScan, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(listRefresh()));

    m_connectedWifi.asciiEncoding = true;
    //m_refreshTimer = new QTimer(this);
    //connect(m_refreshTimer, SIGNAL(timeout()), this, SLOT(ssidScan()));

}

void Wireless::show()
{
    this->ssidScan();
    //m_refreshTimer->start(6000);
    Page::show();
}

void Wireless::hide()
{
    //m_refreshTimer->stop();
    Page::hide();
}

Wireless::~Wireless()
{
    m_ssidScan->kill();
    delete m_ssidScan;
}

void Wireless::ssidScan()
{
    if(m_ssidScan->state() == QProcess::NotRunning){
        ui_connectButton->setEnabled(false);
        ui_ssidListWidget->clear();
        QListWidgetItem *scanning = new QListWidgetItem("Scanning for Networks!!!");
        // format the list item
        QSize s = scanning->sizeHint();
        s.setHeight(172);
        scanning->setSizeHint(s);
        QFont f = scanning->font();
        f.setPointSize(20);
        f.setBold(true);
        scanning->setFont(f);
        scanning->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        scanning->setTextColor(Qt::red);
        ui_ssidListWidget->addItem(scanning);
        m_ssidScan->start("/mnt/kiss/wifi/wifi_scan.sh", QIODevice::ReadOnly);
    }
}

void Wireless::listRefresh()
{
    QFile fn(QString("/mnt/kiss/wifi/ssids"));

    if(fn.exists()){
        ui_connectButton->setEnabled(true);
        ui_ssidListWidget->clear();
        fn.open(QIODevice::ReadOnly);
        while(!fn.atEnd()){
            QString net(fn.readLine());
            this->addSsidToList(net);
        }
        fn.close();
    }
    else{
        ui_ssidListWidget->currentItem()->setText("No Networks Available");
    }
}

void Wireless::addSsidToList(QString net)
{
    // get the SSID name
    QString ssidName = net.section('"',1,1);

    // get the link quality
    int q = net.indexOf("Quality");
    q = net.indexOf(':',q) + 1;
    int eq = net.indexOf('/',q);
    int quality = net.mid(q,eq-q).toInt();

    QListWidgetItem *ssid = new QListWidgetItem(ssidName);
    // format the list item
    QSize s = ssid->sizeHint();
    s.setHeight(35);
    ssid->setSizeHint(s);
    QFont f = ssid->font();
    f.setPointSize(14);
    f.setBold(true);
    ssid->setFont(f);

    ui_ssidListWidget->addItem(ssid);
}

void Wireless::on_ui_connectButton_clicked()
{
    m_connectedWifi.ssid = ui_ssidListWidget->currentItem()->text();

    wifiDialog wDialog(&m_connectedWifi,this);
    if(wDialog.exec() == QDialog::Rejected)
        return;
}

void Wireless::on_ui_refreshButton_clicked()
{
    this->ssidScan();
}


//////////////////////////////////////////////////////////////////////////
// Wifi Network Config Dialog
/////////////////////////////
wifiDialog::wifiDialog(WifiPort *wp, QWidget *parent)
    : QDialog(parent), port(wp)
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    this->setStyleSheet("QDialog{border:3px solid blue; border-radius: 5px}");

    // Main Window Layout
    QVBoxLayout *mainLayout = new QVBoxLayout;

// SSID
    QHBoxLayout *ssidBox = new QHBoxLayout;
    ssid.setText("SSID:");
    ssidBox->addWidget(&ssid);
    QLabel *ssidName = new QLabel(port->ssid);
    ssidBox->addWidget(ssidName);
    mainLayout->addLayout(ssidBox);

// Allocation
    QHBoxLayout *allocBox = new QHBoxLayout;
    alloc.setText("Allocation:");
    allocBox->addWidget(&alloc);
    allocCombo = new QComboBox(this);
    allocCombo->setMinimumSize(QSize(100,20));
    allocCombo->addItem("Dynamic");
    allocCombo->addItem("Static");
    if(port->allocation.isNull())
        allocCombo->setCurrentIndex(0);
    else
        allocCombo->setCurrentIndex(allocCombo->findText(port->allocation));
    allocBox->addWidget(allocCombo);
    mainLayout->addLayout(allocBox);

// authentication
    QHBoxLayout *authBox = new QHBoxLayout;
    auth.setText("Authentication:");
    authBox->addWidget(&auth);
    authCombo = new QComboBox(this);
    authCombo->setMinimumSize(QSize(100,20));
    authCombo->addItem("NONE");
    authCombo->addItem("OPEN");
    authCombo->addItem("SHARED");
    authCombo->addItem("WPAPSK");
    authCombo->addItem("WPA2PSK");
    if(port->authentication.isNull())
        authCombo->setCurrentIndex(0);
    else{
        authCombo->setCurrentIndex(authCombo->findText(port->authentication));
    }
    authBox->addWidget(authCombo);
    mainLayout->addLayout(authBox);

// Encryption
    QHBoxLayout *encrypBox = new QHBoxLayout;
    encryp.setText("Encryption:");
    encrypBox->addWidget(&encryp);
    encrypCombo = new QComboBox(this);
    encrypCombo->setMinimumSize(QSize(100,20));
    encrypCombo->addItem("NONE");
    encrypCombo->addItem("AES");
    encrypCombo->addItem("TKIP");
    encrypCombo->addItem("WEP");
    if(port->encryption.isNull())
        encrypCombo->setCurrentIndex(0);
    else{
        encrypCombo->setCurrentIndex(encrypCombo->findText(port->encryption));
    }
    encrypBox->addWidget(encrypCombo);
    mainLayout->addLayout(encrypBox);

// Ascii and Hex check boxes
    QHBoxLayout *encodeBox = new QHBoxLayout;
    encodeBox->addStretch();
    AsciiCheck = new QCheckBox("Ascii");
    AsciiCheck->setChecked(port->asciiEncoding);
    encodeBox->addWidget(AsciiCheck);
    HexCheck = new QCheckBox("Hex");
    HexCheck->setChecked(!port->asciiEncoding);
    encodeBox->addWidget(HexCheck);
    mainLayout->addLayout(encodeBox);

// Pass Key
    QHBoxLayout *keyBox = new QHBoxLayout;
    key.setText("Passkey:");
    keyBox->addWidget(&key);
    keyLineEdit = new QLineEdit(this);
    keyLineEdit->setMaximumWidth(150);
    keyBox->addWidget(keyLineEdit);
    mainLayout->addLayout(keyBox);

// Dialog buttons
    QHBoxLayout *buttonBox = new QHBoxLayout;
    connectButton = new QPushButton("Connect");
    connectButton->setMinimumSize(QSize(80,30));
    connectButton->setMaximumSize(QSize(80,30));
    connectButton->setStyleSheet("QPushButton:default{border:0px;background-image:url(:/actions/rivet80x30L.png);color:black;} QPushButton:pressed{border:0px;background-image:url(:/actions/rivet80x30D.png);color:white;}");
    buttonBox->addWidget(connectButton);
    cancelButton = new QPushButton("Cancel");
    cancelButton->setMinimumSize(QSize(80,30));
    cancelButton->setMaximumSize(QSize(80,30));
    cancelButton->setStyleSheet("QPushButton{border:0px;background-image:url(:/actions/rivet80x30L.png);color:black;} QPushButton:pressed{border:0px;background-image:url(:/actions/rivet80x30D.png);color:white;}");
    buttonBox->addWidget(cancelButton);
    mainLayout->addLayout(buttonBox);

    this->setLayout(mainLayout);

    connect(keyLineEdit, SIGNAL(selectionChanged()), this, SLOT(keyInput()));
    connect(AsciiCheck, SIGNAL(clicked()), this, SLOT(encodingChanged()));
    connect(HexCheck, SIGNAL(clicked()), this, SLOT(encodingChanged()));
    connect(connectButton, SIGNAL(clicked()), this, SLOT(acceptData()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void wifiDialog::encodingChanged()
{
    if(port->asciiEncoding){
        AsciiCheck->setChecked(false);
        HexCheck->setChecked(true);
        port->asciiEncoding = false;
    }else{
        AsciiCheck->setChecked(true);
        HexCheck->setChecked(false);
        port->asciiEncoding = true;
    }
}

void wifiDialog::acceptData()
{
    port->allocation = allocCombo->currentText();
    port->authentication = authCombo->currentText();
    port->encryption = encrypCombo->currentText();
    port->asciiEncoding = AsciiCheck->isChecked();
    port->key = keyLineEdit->text();
    port->txRate = 6;
    this->accept();
}

void wifiDialog::keyInput()
{
        QwertyKeypad user_keypad(this);

        keyLineEdit->setStyleSheet("QLineEdit#ui_ServoPositionLine{background-color:red}");
        user_keypad.exec();
        port->key = user_keypad.getString();

        keyLineEdit->setText(port->key);
        keyLineEdit->setStyleSheet("QLineEdit#ui_ServoPositionLine{background-color:white}");
}
