#include "regist.h"
#include "ui_regist.h"
#include "loginbox.h"
#include "smtp.h"
#include <QPainter>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTime>
#include <QByteArray>

Regist::Regist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Regist)
{
    ui->setupUi(this);
    this->setWindowTitle("火车订票系统");
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
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

int Regist::getVarificationCode()
{
    return varificationCode;
}

void Regist::setVarificationCode(int code)
{
    this->varificationCode = code;
}

void Regist::on_registButton_clicked()
{
    QString username = ui->accountLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    QString code = ui->codeLineEdit->text();
    int varCode = code.toInt();

    if(varCode != getVarificationCode()){
        QMessageBox::warning(NULL,"提示","验证码不正确,请重新输入。");
    }
    else {
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
                ui->codeLineEdit->clear();
            }
        }else {
            QMessageBox::information(this,"注册提示","账号已存在");
            ui->accountLineEdit->clear();
            ui->passwordLineEdit->clear();
            ui->codeLineEdit->clear();
        }
        delete model;
    }
}

void Regist::on_backButton_clicked()
{
    LoginBox *login = new LoginBox;
//    login->setFixedSize(450,253);
    login->show();
    this->hide();
}

void Regist::on_getCodeButton_clicked()
{
    QString recvaddr0 = ui->accountLineEdit->text();
    QRegExp re("^[A-Za-z0-9]+([_\\.][A-Za-z0-9]+)*@([A-Za-z0-9\\-]+\\.)+[A-Za-z]{2,6}$");
        if(re.exactMatch(recvaddr0)){//验证邮箱格式
            QMessageBox::information(NULL,"提示","验证码将在30s内发送至您的邮箱，请耐心等待。");
            QTime time= QTime::currentTime();
            qsrand(time.msec()+time.second()*1000);
            int code = 1000+qrand() % (9999-1000);    //产生1000~9999以内的随机数
            setVarificationCode(code);
            Smtp smtp("asuna2021730@163.com", "PWKNWADEVEUYGWCQ");//按照刚才的方法实例化一个smtp对象
            QByteArray recvaddr = recvaddr0.toUtf8();//QString转QByteArray
            QString subject="火车售票系统注册验证码";
            QString content="欢迎注册注册火车售票系统，本次操作的验证码为："+QString::number(code);
            smtp.send(recvaddr,subject,content);
        }
        else{
            QMessageBox::warning(NULL,"提示","邮箱格式不正确,请重新输入。");
        }
}
