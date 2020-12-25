#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "zh2py.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_textEdit_textChanged();

    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    QStringList strList;
    Zh2py::pytype tp;
    Zh2py *py;

    Ui::Dialog *ui;
};
#endif // DIALOG_H
