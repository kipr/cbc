#include <qkbddriverplugin_qws.h>
#include "qcbclinuxinput.h"

#ifndef CBCKBDPLUGIN_H
#define CBCKBDPLUGIN_H

class CBCKbdPlugin : public QKbdDriverPlugin
{
public:
    CBCKbdPlugin();

    QStringList keys() const;
    QCBCLinuxInputKeyboardHandler* create(const QString &driver, const QString &device);
};

#endif // CBCKBDPLUGIN_H
