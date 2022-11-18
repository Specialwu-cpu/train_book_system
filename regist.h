#ifndef REGIST_H
#define REGIST_H

#include <QWidget>
#include "user.h"

namespace Ui {
class Regist;
}

class Regist : public QWidget
{
    Q_OBJECT

public:
    explicit Regist(QWidget *parent = nullptr);
    ~Regist();

protected:
    void paintEvent(QPaintEvent*);

private slots:
    void on_registButton_clicked();

    void on_backButton_clicked();

private:
    Ui::Regist *ui;
};

#endif // REGIST_H
