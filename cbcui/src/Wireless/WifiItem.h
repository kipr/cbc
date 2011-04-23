#ifndef __WIFIITEM_H__
#define __WIFIITEM_H__

#include "ui_WifiItem.h"

class WifiItem : public QWidget, private Ui::WifiItem
{
public:
    WifiItem(QString ssid = 0, QWidget *parent = 0);
    ~WifiItem();

    void setSsidText(QString ssid);
    void setAccessPoint(QString ap);
    void setSignalImage(int quality);
    void setLockEnabled(bool lk);
};

#endif // __WIFIITEM_H__
