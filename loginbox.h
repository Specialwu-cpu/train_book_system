#ifndef LOGINBOX_H
#define LOGINBOX_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class LoginBox;
}

class LoginBox : public QWidget
{
    Q_OBJECT

public:
    explicit LoginBox(QWidget *parent = nullptr);
    ~LoginBox();

protected:
    void paintEvent(QPaintEvent*);

private slots:
    void on_loginButton_clicked();

    void on_forgetPasswordButton_clicked();

    void on_registButton_clicked();

    void on_accountLineEdit_editingFinished();

private:
    Ui::LoginBox *ui;
    MainWindow w;
};

#endif // LOGINBOX_H
