#include "CBCKbdPlugin.h"
//#include "qcbclinuxinput.h"

QT_BEGIN_NAMESPACE

CBCKbdPlugin::CBCKbdPlugin() : QKbdDriverPlugin()
{
}

QStringList CBCKbdPlugin::keys() const
{
    return (QStringList() << QLatin1String("CBCKbdInput"));
}

QCBCLinuxInputKeyboardHandler* CBCKbdPlugin::create(const QString &driver, const QString &device)
{
    Q_UNUSED(device);
    if(driver.compare(QLatin1String("CBCKbdInput"), Qt::CaseInsensitive))
        return 0;
    return new QCBCLinuxInputKeyboardHandler(device);
}

Q_EXPORT_PLUGIN2(cbckbdplugin, CBCKbdPlugin)

QT_END_NAMESPACE
