#include "findpassword.h"
#include "ui_findpassword.h"

FindPassword::FindPassword(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindPassword)
{
    ui->setupUi(this);
}

FindPassword::~FindPassword()
{
    delete ui;
}
