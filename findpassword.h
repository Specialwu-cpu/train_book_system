#ifndef FINDPASSWORD_H
#define FINDPASSWORD_H

#include <QWidget>

namespace Ui {
class FindPassword;
}

class FindPassword : public QWidget
{
    Q_OBJECT

public:
    explicit FindPassword(QWidget *parent = nullptr);
    ~FindPassword();

private slots:
    void on_setPasswordButton_clicked();

    void on_backButton_clicked();

    void on_getCodeButton_clicked();

private:
    Ui::FindPassword *ui;
    int varificationCode;

public:
    int getVarificationCode();
    void setVarificationCode(int);
};

#endif // FINDPASSWORD_H
