#ifndef __QWERTYKEYPAD_H__
#define __QWERTYKEYPAD_H__

#include "ui_QwertyKeypad.h"
#include <QDialog>

class QwertyKeypad : public QDialog, private Ui::QwertyKeypad
{
    Q_OBJECT
protected:
    QwertyKeypad(QWidget *parent = 0);
    static QwertyKeypad *m_alphaKeypad;
    ~QwertyKeypad();

public:
    static QwertyKeypad *initialize(QWidget *parent = 0){
        if(!m_alphaKeypad)
            m_alphaKeypad = new QwertyKeypad(parent);
        return m_alphaKeypad;
    }

    static QwertyKeypad *instance(QString txt){
        m_alphaKeypad->setString(txt);
        return m_alphaKeypad;
    }
    static QwertyKeypad *destroy() {
        delete m_alphaKeypad;
        m_alphaKeypad = 0;
        return 0;
    }

    //QwertyKeypad(QWidget *parent = 0);
    QString getString();

public slots:
    void on_ui_oneButton_clicked();
    void on_ui_twoButton_clicked();
    void on_ui_threeButton_clicked();
    void on_ui_fourButton_clicked();
    void on_ui_fiveButton_clicked();
    void on_ui_sixButton_clicked();
    void on_ui_sevenButton_clicked();
    void on_ui_eightButton_clicked();
    void on_ui_nineButton_clicked();
    void on_ui_zeroButton_clicked();
    void on_ui_aButton_clicked();
    void on_ui_bButton_clicked();
    void on_ui_cButton_clicked();
    void on_ui_dButton_clicked();
    void on_ui_eButton_clicked();
    void on_ui_fButton_clicked();
    void on_ui_gButton_clicked();
    void on_ui_hButton_clicked();
    void on_ui_iButton_clicked();
    void on_ui_jButton_clicked();
    void on_ui_kButton_clicked();
    void on_ui_lButton_clicked();
    void on_ui_mButton_clicked();
    void on_ui_nButton_clicked();
    void on_ui_oButton_clicked();
    void on_ui_pButton_clicked();
    void on_ui_qButton_clicked();
    void on_ui_rButton_clicked();
    void on_ui_sButton_clicked();
    void on_ui_tButton_clicked();
    void on_ui_uButton_clicked();
    void on_ui_vButton_clicked();
    void on_ui_wButton_clicked();
    void on_ui_xButton_clicked();
    void on_ui_yButton_clicked();
    void on_ui_zButton_clicked();
    void on_ui_forwardslashButton_clicked();
    void on_ui_periodButton_clicked();
    void on_ui_commaButton_clicked();
    void on_ui_semicolonButton_clicked();
    void on_ui_spaceButton_clicked();

    void on_ui_enterButton_clicked();
    void on_ui_backspaceButton_clicked();
    void on_ui_shiftButton_toggled(bool checked);
    void on_ui_symbolButton_toggled(bool checked);

private:
    QString userString;
    void setString(QString txt);
};

#endif // __QWERTYKEYPAD_H__
