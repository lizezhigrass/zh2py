#include "dialog.h"
#include "ui_dialog.h"
#include<QDebug>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle("反差拼音小程序");
    py = new Zh2py("./pinyinData/tradition.dict",
                   "./pinyinData/words.dict",
                   "./pinyinData/phrases.dict",
                   "./pinyinData/phrases.dict.map");
    strList << "首字母" << "拼音声调" << "尾部数字" << "无声调";
    ui->comboBox->addItems(strList);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_textEdit_textChanged()
{
    QString str = ui->textEdit->toPlainText();
    if(ui->comboBox->currentText() == "首字母")
        {tp = Zh2py::STYLE_FIRST_LETTER;}
    if(ui->comboBox->currentText() == "拼音声调")
        {tp = Zh2py::STYLE_NORMAL;}
    if(ui->comboBox->currentText() == "无声调")
        {tp = Zh2py::STYLE_TONE;}
    if(ui->comboBox->currentText() == "尾部数字")
        {tp = Zh2py::STYLE_TONE_NUM;}
    ui->textEdit_2->setText(py->getpy(str,tp));
}

void Dialog::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "首字母")
        {tp = Zh2py::STYLE_FIRST_LETTER;}
    if(arg1 == "拼音声调")
        {tp = Zh2py::STYLE_NORMAL;}
    if(arg1 == "无声调")
        {tp = Zh2py::STYLE_TONE;}
    if(arg1 == "尾部数字")
        {tp = Zh2py::STYLE_TONE_NUM;}
    QString str = ui->textEdit->toPlainText();
    ui->textEdit_2->setText(py->getpy(str,tp));
}
