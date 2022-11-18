#include "regist.h"
#include "ui_regist.h"
#include "loginbox.h"
#include <QPainter>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMessageBox>

Regist::Regist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Regist)
{
    ui->setupUi(this);
    this->setWindowTitle("火车订票系统");
}

Regist::~Regist()
{
    delete ui;
}

void Regist::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/new/prefix1/images/login.webp.jpg"));
    painter.end();
}


void Regist::on_registButton_clicked()
{
    QString username = ui->accountLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("userinfo");
    model->setFilter(QString("username='%1'")
                     .arg(username));
    model->select();
    int row = model->rowCount();

    if(row <= 0){
        QString regist = QString("insert into userinfo (username,password) values('%1','%2')").arg(username).arg(password);
        QSqlQuery query;
        if(query.exec(regist)){
            QMessageBox::information(this,"注册提示","注册成功");
            LoginBox *login = new LoginBox;
    //        login->setFixedSize(450,253);
            login->show();
            this->hide();

        }else{
            QMessageBox::information(this,"注册提示","注册失败");
            ui->accountLineEdit->clear();
            ui->passwordLineEdit->clear();
        }
    }else {
        QMessageBox::information(this,"注册提示","账号已存在");
        ui->accountLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
    delete model;
}

void Regist::on_backButton_clicked()
{
    LoginBox *login = new LoginBox;
//    login->setFixedSize(450,253);
    login->show();
    this->hide();
}
