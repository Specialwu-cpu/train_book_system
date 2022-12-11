#include "loginbox.h"
#include "ui_loginbox.h"
#include <QPainter>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlTableModel>
#include "regist.h"
#include "findpassword.h"
#include <QPixmap>

LoginBox::LoginBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginBox)
{
    ui->setupUi(this);
    this->setWindowTitle("火车订票系统");
    QPixmap pixmap(":/new/prefix1/header/22.jpg");
    pixmap = pixmap.scaled(ui->headerLabel->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QPixmap image(ui->headerLabel->size().width(),ui->headerLabel->size().height());
    image.fill(Qt::transparent);
    QPainterPath painterPath;
    painterPath.addEllipse(0, 0, ui->headerLabel->size().width(), ui->headerLabel->size().height());
    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setClipPath(painterPath);
    painter.drawPixmap(0, 0, ui->headerLabel->size().width(), ui->headerLabel->size().height(), pixmap);
    ui->headerLabel->setPixmap(image);
    ui->loginButton->setFocus();    //设置默认焦点
    ui->loginButton->setShortcut( QKeySequence::InsertParagraphSeparator );  //设置快捷键为键盘的“回车”键

    ui->loginButton->setShortcut(Qt::Key_Enter);  //设置快捷键为enter键

    ui->loginButton->setShortcut(Qt::Key_Return); //设置快捷键为小键盘上的enter键
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}

LoginBox::~LoginBox()
{
    delete ui;
}

void LoginBox::paintEvent(QPaintEvent *)
{
    QPainter painter;
    painter.begin(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/new/prefix1/images/login.webp.jpg"));
    painter.end();
}

void LoginBox::on_loginButton_clicked()
{
    QString username = ui->accountLineEdit->text();
    QString password = ui->passwordLineEdit->text();


    //查询数据库验证
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("userinfo");
    model->setFilter(QString("username='%1' and password='%2'")
                     .arg(username).arg(password));
    model->select();

    //检测是否查询到数据
    int row = model->rowCount();
    if(row>0){
        QMessageBox::information(this,"登录提示","登录成功");
        //w.setFixedSize(1000,600);
        w.setWindowFlags(Qt::WindowCloseButtonHint);
        w.show();
        User user=User();
        user.setName(username);
        user.setPassword(password);
        w.setUser(user);
        this->hide();
    }else{
        QMessageBox::information(this,"登录提示","登录失败");
        ui->accountLineEdit->clear();
        ui->passwordLineEdit->clear();
    }
    delete model;
}

void LoginBox::on_forgetPasswordButton_clicked()
{
    FindPassword *findword = new FindPassword;
    findword->show();
    this->hide();
}

void LoginBox::on_registButton_clicked()
{
    Regist *reg = new Regist;
//    reg->setFixedSize(450,253);
    reg->show();
    this->hide();
}
