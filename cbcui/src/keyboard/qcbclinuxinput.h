#ifndef QCBCLINUXINPUT_H
#define QCBCLINUXINPUT_H

#include <QWSKeyboardHandler>

class QCBCLinuxKbPrivate;

class QCBCLinuxInputKeyboardHandler : public QWSKeyboardHandler
{
public:
    QCBCLinuxInputKeyboardHandler(const QString &device);
    virtual ~QCBCLinuxInputKeyboardHandler();
    virtual bool filterInputEvent(quint16 &input_code, qint32 &input_value);

private:
    QCBCLinuxKbPrivate *d;
};


#endif // QCBCLINUXINPUT_H
