#include "QwertyKeypad.h"

QwertyKeypad *QwertyKeypad::m_alphaKeypad=0;

QwertyKeypad::QwertyKeypad(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    this->move(0,50);
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setStyleSheet("QDialog{border: 3px solid blue}");
}

QwertyKeypad::~QwertyKeypad()
{
}

void QwertyKeypad::on_ui_oneButton_clicked()
{
    ui_outputLine->insert(ui_oneButton->text());
}
void QwertyKeypad::on_ui_twoButton_clicked()
{
    ui_outputLine->insert(ui_twoButton->text());
}
void QwertyKeypad::on_ui_threeButton_clicked()
{
    ui_outputLine->insert(ui_threeButton->text());
}
void QwertyKeypad::on_ui_fourButton_clicked()
{
    ui_outputLine->insert(ui_fourButton->text());
}
void QwertyKeypad::on_ui_fiveButton_clicked()
{
    ui_outputLine->insert(ui_fiveButton->text());
}
void QwertyKeypad::on_ui_sixButton_clicked()
{
    ui_outputLine->insert(ui_sixButton->text());
}
void QwertyKeypad::on_ui_sevenButton_clicked()
{
    if(ui_symbolButton->isChecked())
        ui_outputLine->insert("&");
    else
        ui_outputLine->insert(ui_sevenButton->text());
}
void QwertyKeypad::on_ui_eightButton_clicked()
{
    ui_outputLine->insert(ui_eightButton->text());
}
void QwertyKeypad::on_ui_nineButton_clicked()
{
    ui_outputLine->insert(ui_nineButton->text());
}
void QwertyKeypad::on_ui_zeroButton_clicked()
{
    ui_outputLine->insert(ui_zeroButton->text());
}
void QwertyKeypad::on_ui_aButton_clicked()
{
    ui_outputLine->insert(ui_aButton->text());
}
void QwertyKeypad::on_ui_bButton_clicked()
{
    ui_outputLine->insert(ui_bButton->text());
}
void QwertyKeypad::on_ui_cButton_clicked()
{
    ui_outputLine->insert(ui_cButton->text());
}
void QwertyKeypad::on_ui_dButton_clicked()
{
    ui_outputLine->insert(ui_dButton->text());
}
void QwertyKeypad::on_ui_eButton_clicked()
{
    ui_outputLine->insert(ui_eButton->text());
}
void QwertyKeypad::on_ui_fButton_clicked()
{
    ui_outputLine->insert(ui_fButton->text());
}
void QwertyKeypad::on_ui_gButton_clicked()
{
    ui_outputLine->insert(ui_gButton->text());
}
void QwertyKeypad::on_ui_hButton_clicked()
{
    ui_outputLine->insert(ui_hButton->text());
}
void QwertyKeypad::on_ui_iButton_clicked()
{
    ui_outputLine->insert(ui_iButton->text());
}
void QwertyKeypad::on_ui_jButton_clicked()
{
    ui_outputLine->insert(ui_jButton->text());
}
void QwertyKeypad::on_ui_kButton_clicked()
{
    ui_outputLine->insert(ui_kButton->text());
}
void QwertyKeypad::on_ui_lButton_clicked()
{
    ui_outputLine->insert(ui_lButton->text());
}
void QwertyKeypad::on_ui_mButton_clicked()
{
    ui_outputLine->insert(ui_mButton->text());
}
void QwertyKeypad::on_ui_nButton_clicked()
{
    ui_outputLine->insert(ui_nButton->text());
}
void QwertyKeypad::on_ui_oButton_clicked()
{
    ui_outputLine->insert(ui_oButton->text());
}
void QwertyKeypad::on_ui_pButton_clicked()
{
    ui_outputLine->insert(ui_pButton->text());
}
void QwertyKeypad::on_ui_qButton_clicked()
{
    ui_outputLine->insert(ui_qButton->text());
}
void QwertyKeypad::on_ui_rButton_clicked()
{
    ui_outputLine->insert(ui_rButton->text());
}
void QwertyKeypad::on_ui_sButton_clicked()
{
    ui_outputLine->insert(ui_sButton->text());
}
void QwertyKeypad::on_ui_tButton_clicked()
{
    ui_outputLine->insert(ui_tButton->text());
}
void QwertyKeypad::on_ui_uButton_clicked()
{
    ui_outputLine->insert(ui_uButton->text());
}
void QwertyKeypad::on_ui_vButton_clicked()
{
    ui_outputLine->insert(ui_vButton->text());
}
void QwertyKeypad::on_ui_wButton_clicked()
{
    ui_outputLine->insert(ui_wButton->text());
}
void QwertyKeypad::on_ui_xButton_clicked()
{
    ui_outputLine->insert(ui_xButton->text());
}
void QwertyKeypad::on_ui_yButton_clicked()
{
    ui_outputLine->insert(ui_yButton->text());
}
void QwertyKeypad::on_ui_zButton_clicked()
{
    ui_outputLine->insert(ui_zButton->text());
}
void QwertyKeypad::on_ui_forwardslashButton_clicked()
{
    ui_outputLine->insert(ui_forwardslashButton->text());
}
void QwertyKeypad::on_ui_periodButton_clicked()
{
    ui_outputLine->insert(ui_periodButton->text());
}
void QwertyKeypad::on_ui_commaButton_clicked()
{
    ui_outputLine->insert(ui_commaButton->text());
}
void QwertyKeypad::on_ui_semicolonButton_clicked()
{
    ui_outputLine->insert(ui_semicolonButton->text());
}
void QwertyKeypad::on_ui_spaceButton_clicked()
{
    ui_outputLine->insert(" ");
}

void QwertyKeypad::on_ui_enterButton_clicked()
{
    userString = ui_outputLine->text();
    this->close();
}

void QwertyKeypad::on_ui_backspaceButton_clicked()
{
    QString string = ui_outputLine->text();
    ui_outputLine->setText(string.left(string.size() - 1));
}

void QwertyKeypad::on_ui_shiftButton_toggled(bool checked)
{
    if(ui_symbolButton->isChecked())
        ui_symbolButton->setChecked(false);

    if(checked){
        ui_oneButton->setText("1");
        ui_twoButton->setText("2");
        ui_threeButton->setText("3");
        ui_fourButton->setText("4");
        ui_fiveButton->setText("5");
        ui_sixButton->setText("6");
        ui_sevenButton->setText("7");
        ui_eightButton->setText("8");
        ui_nineButton->setText("9");
        ui_zeroButton->setText("0");
        ui_aButton->setText("A");
        ui_bButton->setText("B");
        ui_cButton->setText("C");
        ui_dButton->setText("D");
        ui_eButton->setText("E");
        ui_fButton->setText("F");
        ui_gButton->setText("G");
        ui_hButton->setText("H");
        ui_iButton->setText("I");
        ui_jButton->setText("J");
        ui_kButton->setText("K");
        ui_lButton->setText("L");
        ui_mButton->setText("M");
        ui_nButton->setText("N");
        ui_oButton->setText("O");
        ui_pButton->setText("P");
        ui_qButton->setText("Q");
        ui_rButton->setText("R");
        ui_sButton->setText("S");
        ui_tButton->setText("T");
        ui_uButton->setText("U");
        ui_vButton->setText("V");
        ui_wButton->setText("W");
        ui_xButton->setText("X");
        ui_yButton->setText("Y");
        ui_zButton->setText("Z");

        //ui_semicolonButton->setText(":");
        //ui_commaButton->setText("<");
        //ui_periodButton->setText(">");
        //ui_forwardslashButton->setText("?");
    }
    else{
        ui_oneButton->setText("1");
        ui_twoButton->setText("2");
        ui_threeButton->setText("3");
        ui_fourButton->setText("4");
        ui_fiveButton->setText("5");
        ui_sixButton->setText("6");
        ui_sevenButton->setText("7");
        ui_eightButton->setText("8");
        ui_nineButton->setText("9");
        ui_zeroButton->setText("0");
        ui_aButton->setText("a");
        ui_bButton->setText("b");
        ui_cButton->setText("c");
        ui_dButton->setText("d");
        ui_eButton->setText("e");
        ui_fButton->setText("f");
        ui_gButton->setText("g");
        ui_hButton->setText("h");
        ui_iButton->setText("i");
        ui_jButton->setText("j");
        ui_kButton->setText("k");
        ui_lButton->setText("l");
        ui_mButton->setText("m");
        ui_nButton->setText("n");
        ui_oButton->setText("o");
        ui_pButton->setText("p");
        ui_qButton->setText("q");
        ui_rButton->setText("r");
        ui_sButton->setText("s");
        ui_tButton->setText("t");
        ui_uButton->setText("u");
        ui_vButton->setText("v");
        ui_wButton->setText("w");
        ui_xButton->setText("x");
        ui_yButton->setText("y");
        ui_zButton->setText("z");

        ui_semicolonButton->setText(";");
        ui_commaButton->setText(",");
        ui_periodButton->setText(".");
        ui_forwardslashButton->setText("/");
    }
}

void QwertyKeypad::on_ui_symbolButton_toggled(bool checked)
{
    if(checked){
        ui_oneButton->setText("!");
        ui_twoButton->setText("@");
        ui_threeButton->setText("#");
        ui_fourButton->setText("$");
        ui_fiveButton->setText("%");
        ui_sixButton->setText("^");
        ui_sevenButton->setText("and");
        ui_eightButton->setText("*");
        ui_nineButton->setText("(");
        ui_zeroButton->setText(")");
        ui_aButton->setText(" ");
        ui_aButton->setEnabled(false);
        ui_bButton->setText("-");
        ui_cButton->setText(" ");
        ui_cButton->setEnabled(false);
        ui_dButton->setText(" ");
        ui_dButton->setEnabled(false);
        ui_eButton->setText("<");
        ui_fButton->setText(" ");
        ui_fButton->setEnabled(false);
        ui_gButton->setText("_");
        ui_hButton->setText("+");
        ui_iButton->setText("}");
        ui_jButton->setText(":");
        ui_kButton->setText("'");
        ui_lButton->setText("\"");
        ui_mButton->setText("?");
        ui_nButton->setText("=");
        ui_oButton->setText("\\");
        ui_pButton->setText("|");
        ui_qButton->setText("~");
        ui_rButton->setText(">");
        ui_sButton->setText(" ");
        ui_sButton->setEnabled(false);
        ui_tButton->setText("[");
        ui_uButton->setText("{");
        ui_vButton->setText(" ");
        ui_vButton->setEnabled(false);
        ui_wButton->setText("`");
        ui_xButton->setText(" ");
        ui_xButton->setEnabled(false);
        ui_yButton->setText("]");
        ui_zButton->setText(" ");
        ui_zButton->setEnabled(false);
    }else{
        ui_aButton->setEnabled(true);
        ui_cButton->setEnabled(true);
        ui_dButton->setEnabled(true);
        ui_fButton->setEnabled(true);
        ui_sButton->setEnabled(true);
        ui_vButton->setEnabled(true);
        ui_xButton->setEnabled(true);
        ui_zButton->setEnabled(true);
        on_ui_shiftButton_toggled(ui_shiftButton->isChecked());
    }
}

QString QwertyKeypad::getString()
{
    return userString;
}

void QwertyKeypad::setString(QString txt)
{
    ui_outputLine->setText(txt);
}
