#include "WifiItem.h"

WifiItem::WifiItem(QString text, QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    ui_ssidLabel->setText(text);
    ui_accessPointLabel->setHidden(true);
    ui_lockWidget->setVisible(false);
}

WifiItem::~WifiItem()
{
}

void WifiItem::setSsidText(QString ssid)
{
    ui_ssidLabel->setText(ssid);
}

void WifiItem::setAccessPoint(QString ap)
{
    ui_accessPointLabel->setText(ap);
}

void WifiItem::setLockEnabled(bool lk)
{
    ui_lockWidget->setVisible(lk);
}

void WifiItem::setSignalImage(int quality)
{
    QString style;
    if(quality > 75)
        style = "QFrame{border:0px;background-image:url(:/actions/botguy_wifi3.png);}";
    else if(quality > 50)
        style = "QFrame{border:0px;background-image:url(:/actions/botguy_wifi2.png);}";
    else if(quality > 25)
        style = "QFrame{border:0px;background-image:url(:/actions/botguy_wifi1.png);}";
    else
        style = "QFrame{border:0px;background-image:url(:/actions/botguy_wifi0.png);}";
    ui_signalStrengthFrame->setStyleSheet(style);
}
