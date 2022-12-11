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

private:
    Ui::FindPassword *ui;
};

#endif // FINDPASSWORD_H
