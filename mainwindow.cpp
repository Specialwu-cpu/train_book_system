#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginbox.h"
#include <QPainter>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTime>
#include <QByteArray>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->bookPage);
    //增加comboBox的点击事件
    connect(ui->placeComboBox1, SIGNAL(clicked()), this, SLOT(update_startPlace_list()));
    connect(ui->placeComboBox2, SIGNAL(clicked()), this, SLOT(update_endPlace_list()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUser(User user) {
    this->user=user;
}


void MainWindow::on_bookButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->bookPage);
}

void MainWindow::update_startPlace_list()
{
    //首先断开联系，才能使用clear，防止每次点击时就添加，做到点击后只出现一次已添加过的车站名
    disconnect(ui->placeComboBox1, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboIndexChanged(int)));
    ui->placeComboBox1->clear();
    connect(ui->placeComboBox1, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboIndexChanged(int)));
    QString places("select * from place;");
    QSqlQuery query;
    query.exec(places);
    while(query.next()){
        QString placeName = QString("%1").arg(query.value("placeName").toString());
        ui->placeComboBox1->addItem(placeName);
    }
}

void MainWindow::update_endPlace_list()
{
    disconnect(ui->placeComboBox2, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboIndexChanged(int)));
    ui->placeComboBox2->clear();
    connect(ui->placeComboBox2, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboIndexChanged(int)));
    QString places("select * from place;");
    QSqlQuery query;
    query.exec(places);
    while(query.next()){
        QString placeName = QString("%1").arg(query.value("placeName").toString());
        ui->placeComboBox2->addItem(placeName);
    }
}

void MainWindow::on_backButton_clicked()
{
    LoginBox *login = new LoginBox;
//    login->setFixedSize(450,253);
    login->show();
    this->hide();
}

void MainWindow::on_passwordButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->passwordPage);
}

void MainWindow::on_userInfoButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->userInfoPage);
    ui->mailLabel->setText(user.getName());
    QString username = user.getName();
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("userinfo");
    model->setFilter(QString("username='%1'")
                     .arg(username));
    model->select();
    int row = model->rowCount();

    if(row > 0){
        QString regist = QString("select imagePath from userinfo where username='%1'").arg(username);
        QSqlQuery query;
        if(query.exec(regist)&&query.next()){
            QString imagePath = query.value(0).toString();
            if(imagePath == NULL){
                imagePath = ":/new/prefix1/header/22.jpg";//todo图片可以上传保存在数据库中
            }
            QPixmap pixmap(imagePath);
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
        }
    }
    delete model;
}

void MainWindow::on_ticketsButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ticketsPage);
    ui->stackedWidget->setCurrentWidget(ui->searchPage);
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
    model->setQuery(QString("select username,route.routeId, start, end, date from ticket, route where ticket.routeId = route.routeId;"));
    //列名
    model->setHeaderData(0, Qt::Horizontal, tr("个人邮箱"));
    model->setHeaderData(1, Qt::Horizontal, tr("车次"));
    model->setHeaderData(2, Qt::Horizontal, tr("出发站"));
    model->setHeaderData(2, Qt::Horizontal, tr("终点站"));
    model->setHeaderData(4, Qt::Horizontal, tr("发车日期"));
    ui->tableView->setModel(model);//数据放置进去
    ui->tableView->verticalHeader()->hide();//不显示序号
}

void MainWindow::on_refundButton_clicked()
{
}

void MainWindow::on_doPasswordButton_clicked()
{
    QString username = user.getName();
    QString oldpassword = ui->oldPassowrd->text();
    QString newPassword1 = ui->newPassword1->text();
    QString newPassword2 = ui->newPassword2->text();

    if(newPassword1 != newPassword2){
        QMessageBox::warning(NULL,"提示","两次密码输入不一致，请重新输入。");
    }
    else {
        QSqlTableModel *model = new QSqlTableModel;
        model->setTable("userinfo");
        model->setFilter(QString("username='%1' and password='%2'")
                         .arg(username).arg(oldpassword));
        model->select();
        int row = model->rowCount();

        if(row > 0){
            QString regist = QString("update userinfo set password='%1' where username='%2'").arg(newPassword1).arg(username);
            QSqlQuery query;
            if(query.exec(regist)){
                QMessageBox::information(this,"更新提示","更新密码成功");
                LoginBox *login = new LoginBox;
        //        login->setFixedSize(450,253);
                login->show();
                this->hide();

            }else{
                QMessageBox::information(this,"更新提示","更新密码失败");
                ui->oldPassowrd->clear();
                ui->newPassword1->clear();
                ui->newPassword2->clear();
            }
        }else {
            QMessageBox::information(this,"错误提示","账号不存在，请先去注册");
            ui->oldPassowrd->clear();
            ui->newPassword1->clear();
            ui->newPassword2->clear();
        }
        delete model;
    }
}

void MainWindow::on_searchTicketsButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->searchPage);
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
    model->setQuery(QString("select * from route;"));
    //列名
    model->setHeaderData(0, Qt::Horizontal, tr("车次"));
    model->setHeaderData(1, Qt::Horizontal, tr("出发站"));
    model->setHeaderData(2, Qt::Horizontal, tr("终点站"));
    model->setHeaderData(3, Qt::Horizontal, tr("票数量"));
    model->setHeaderData(4, Qt::Horizontal, tr("日期"));
    ui->tableView->setModel(model);//数据放置进去
    ui->tableView->verticalHeader()->hide();//不显示序号
}

void MainWindow::on_chooseDateButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->calendarPage);
}

void MainWindow::on_calendarWidget_activated(const QDate &date)
{
    QString dateStr = date.toString();
    ui->chooseDateButton->setText(dateStr);
    ui->stackedWidget->setCurrentWidget(ui->bookPage);
}

void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("route");
    //列名
    model->setHeaderData(0, Qt::Horizontal, tr("车次"));
    model->setHeaderData(1, Qt::Horizontal, tr("出发站"));
    model->setHeaderData(2, Qt::Horizontal, tr("终点站"));
    model->setHeaderData(3, Qt::Horizontal, tr("票数量"));
    model->setHeaderData(4, Qt::Horizontal, tr("日期"));
    model->select();
    ui->tableView->setModel(model);//数据放置进去
    QString route = model->data(model->index(index.row(), 0)).toString();
    QString startPlace = model->data(model->index(index.row(), 1)).toString();
    QString endPlace = model->data(model->index(index.row(), 2)).toString();
    QString ticketNums = model->data(model->index(index.row(), 3)).toString();
    QString date = model->data(model->index(index.row(), 4)).toString();
    ui->stackedWidget->setCurrentWidget(ui->buyPage);
    ui->userMail2->setText(user.getName());
    ui->routeId2->setText(route);
    ui->startPlace2->setText(startPlace);
    ui->endPlace2->setText(endPlace);
    ui->date2->setText(date);
}

void MainWindow::on_buyButton_clicked()
{
}

void MainWindow::on_backTobookButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->bookPage);
}

void MainWindow::on_backToSearchButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->searchPage);
}

void MainWindow::on_backToticketsButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ticketsPage);
}

void MainWindow::on_ticketsTable_clicked(const QModelIndex &index)
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("tickets");
    //列名
    model->setHeaderData(0, Qt::Horizontal, tr("个人邮箱"));
    model->setHeaderData(1, Qt::Horizontal, tr("车次"));
    model->setHeaderData(2, Qt::Horizontal, tr("出发站"));
    model->setHeaderData(2, Qt::Horizontal, tr("终点站"));
    model->setHeaderData(4, Qt::Horizontal, tr("发车日期"));
    model->select();
    ui->ticketsTable->setModel(model);//数据放置进去
    QString route = model->data(model->index(index.row(), 1)).toString();
    QString startPlace = model->data(model->index(index.row(), 2)).toString();
    QString endPlace = model->data(model->index(index.row(), 3)).toString();
    QString date = model->data(model->index(index.row(), 4)).toString();
    ui->stackedWidget->setCurrentWidget(ui->refundPage);
    ui->userMail4->setText(user.getName());
    ui->routeId4->setText(route);
    ui->startPlace4->setText(startPlace);
    ui->endPlace4->setText(endPlace);
    ui->date4->setText(date);
}
