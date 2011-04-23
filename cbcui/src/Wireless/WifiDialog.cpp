#include "WifiDialog.h"

WifiDialog::WifiDialog(WifiPort *wp, QWidget *parent)
    : QDialog(parent), port(wp)
{
    setupUi(this);
    move(50,0);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    this->setStyleSheet("QDialog{border:3px solid blue; border-radius: 5px}");

    ui_ssidLabel->setText(port->ssid);
    ui_macLabel->setText(port->ap);
    ui_allocationCombo->setCurrentIndex(0);
    ui_authLabel->setText(port->authentication);
    ui_encrypLabel->setText(port->encryption);
    ui_txRateCombo->addItems(port->rates);

    if(port->encoding.isEmpty() || port->encoding == "ascii"){
        m_encoding = "ascii";
        ui_asciiCheck->setChecked(true);
    }
    else{
        m_encoding = "hex";
        ui_hexCheck->setChecked(true);
    }

    if(port->txRate.isEmpty()){
        int defTx = ui_txRateCombo->findText("9Mb/s");
        if(defTx < 0)
            ui_txRateCombo->setCurrentIndex(0);
        else
            ui_txRateCombo->setCurrentIndex(defTx);
    }
    else
        ui_txRateCombo->setCurrentIndex(ui_txRateCombo->findText(port->txRate));

    if(port->authentication == "OPEN" && port->encryption == "NONE")
    {
        ui_asciiCheck->setEnabled(false);
        ui_hexCheck->setEnabled(false);
        ui_keyLineEdit->setEnabled(false);
    }

    connect(ui_keyLineEdit, SIGNAL(selectionChanged()), this, SLOT(keyInput()));
    connect(ui_asciiCheck, SIGNAL(clicked()), this, SLOT(encodingChanged()));
    connect(ui_hexCheck, SIGNAL(clicked()), this, SLOT(encodingChanged()));
    connect(ui_connectButton, SIGNAL(clicked()), this, SLOT(acceptData()));
    connect(ui_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

void WifiDialog::encodingChanged()
{
    if(m_encoding == "ascii"){
        ui_asciiCheck->setChecked(false);
        ui_hexCheck->setChecked(true);
        m_encoding = "hex";
    }else{
        ui_asciiCheck->setChecked(true);
        ui_hexCheck->setChecked(false);
        m_encoding = "ascii";
    }
}

void WifiDialog::acceptData()
{
    port->allocation = ui_allocationCombo->currentText();
    port->encoding = m_encoding;
    port->key = ui_keyLineEdit->text();
    port->txRate = ui_txRateCombo->currentText();
    this->accept();
}

void WifiDialog::keyInput()
{
        QwertyKeypad *user_keypad = QwertyKeypad::instance("");
        ui_keyLineEdit->setStyleSheet("QLineEdit#ui_ServoPositionLine{background-color:red}");
        user_keypad->exec();

        ui_keyLineEdit->setText(user_keypad->getString());
        ui_keyLineEdit->setStyleSheet("QLineEdit#ui_ServoPositionLine{background-color:white}");
}
