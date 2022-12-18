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
#include <math.h>

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
    QSqlQueryModel *model = new QSqlQueryModel(ui->ticketsTable);
    model->setQuery(QString("select username, routeId, start, end, date from ticket;"));
    //列名
    model->setHeaderData(0, Qt::Horizontal, tr("个人邮箱"));
    model->setHeaderData(1, Qt::Horizontal, tr("车次"));
    model->setHeaderData(2, Qt::Horizontal, tr("出发站"));
    model->setHeaderData(3, Qt::Horizontal, tr("终点站"));
    model->setHeaderData(4, Qt::Horizontal, tr("发车日期"));
    ui->ticketsTable->setModel(model);//数据放置进去
    ui->ticketsTable->verticalHeader()->hide();//不显示序号
}

void MainWindow::on_refundButton_clicked()
{
    int route = ui->routeId4->text().toInt();
    QString startPlace = ui->startPlace4->text();
    QString endPlace = ui->endPlace4->text();
    QString date = ui->date4->text();
    QString regist1 = QString("select placeId from place where placeName='%1' or placeName='%2'").arg(startPlace).arg(endPlace);
    QSqlQuery query1;
    int placeId1 = 0;
    int placeId2 = 0;
    if(query1.exec(regist1)&& query1.next()){
        placeId1 = query1.value(0).toInt();
        query1.next();
        placeId2 = query1.value(0).toInt();
    }
    else{
        qDebug()<<"fail";
    }
    QSqlQuery query2;
    QSqlQuery query3;
    QString deleteSql = QString("delete from ticket where username='%1' and routeId='%2'").arg(user.getName()).arg(route);
    QString updateSql = QString("update route set ticketNumbers=ticketNumbers+1 where startId>='%1' and endId<='%2'").arg(placeId1).arg(placeId2);
    if(query2.exec(deleteSql)){
        if(query3.exec(updateSql)){
            QMessageBox::information(this,"更新提示","退票成功");
        }
        else{
            QMessageBox::information(this,"更新提示","退票失败*1");
        }
    }else{
        QMessageBox::information(this,"更新提示","退票失败*2");
    }
}

void MainWindow::on_doPasswordButton_clicked()
{
    QString username = user.getName();
    QString oldpassword = ui->oldPassowrd->text();
    QString newPassword1 = ui->newPassword1->text();
    QString newPassword2 = ui->newPassword2->text();

    if(newPassword1 == newPassword2)
    {
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
    else {
        QMessageBox::information(this,"错误提示","两次输入新密码不一致，请重新输入");
        ui->oldPassowrd->clear();
        ui->newPassword1->clear();
        ui->newPassword2->clear();
    }
}

void MainWindow::on_searchTicketsButton_clicked()
{
    QString startPlace = ui->placeComboBox1->currentText();
    QString endPlace = ui->placeComboBox2->currentText();
    ui->stackedWidget->setCurrentWidget(ui->searchPage);
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
    QString regist = QString("select placeId from place where placeName='%1' or placeName='%2'").arg(startPlace).arg(endPlace);
    QSqlQuery query;
    int placeId1 = 0;
    int placeId2 = 0;
    if(query.exec(regist)&& query.next()){
        placeId1 = query.value(0).toInt();
        query.next();
        placeId2 = query.value(0).toInt();
    }
    else{
        qDebug()<<"fail";
    }
    model->setQuery(QString("select routeId, start, end, date, startId, endId from route where startId>='%1' and endId<='%2'").arg(placeId1).arg(placeId2));
    //列名
    model->setHeaderData(0, Qt::Horizontal, tr("车次"));
    model->setHeaderData(1, Qt::Horizontal, tr("出发站"));
    model->setHeaderData(2, Qt::Horizontal, tr("终点站"));
    model->setHeaderData(3, Qt::Horizontal, tr("日期"));
    model->setHeaderData(4, Qt::Horizontal, tr("起始站序号"));
    model->setHeaderData(5, Qt::Horizontal, tr("终点站序号"));
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
    model->setHeaderData(3, Qt::Horizontal, tr("日期"));
    model->setHeaderData(4, Qt::Horizontal, tr("起始站序号"));
    model->setHeaderData(5, Qt::Horizontal, tr("终点站序号"));
    model->select();
    ui->tableView->setModel(model);//数据放置进去
    int routeId = model->data(model->index(index.row(), 0)).toString().toInt();
    QString route = model->data(model->index(index.row(), 0)).toString();
    QString startPlace = model->data(model->index(index.row(), 1)).toString();
    QString endPlace = model->data(model->index(index.row(), 2)).toString();
    QString date = model->data(model->index(index.row(), 4)).toString();
    QString placesSql = QString("select placeId from place where placeName='%1' or placeName='%2'").arg(ui->placeComboBox1->currentText()).arg(ui->placeComboBox2->currentText());
    QSqlQuery query;
    int placeId1 = 0;
    int placeId2 = 0;
    if(query.exec(placesSql)&& query.next()){
        placeId1 = query.value(0).toInt();
        query.next();
        placeId2 = query.value(0).toInt();
    }
    else{
        qDebug()<<"fail";
    }
    QString numsSql = QString("select min(ticketNumbers) from route where startId>='%1' or endId<='%2'").arg(placeId1).arg(placeId2);
    QSqlQuery query2;
    int ticketNumbers = 0;
    if(query2.exec(numsSql) && query2.next()){
        ticketNumbers = query2.value(0).toString().toInt();
    }
    else{
        qDebug()<<"fail";
    }
    QString totalNumsSql = QString("select total(ticketNumbers) from route where routeId='%1'").arg(routeId);
    QSqlQuery query3;
    int totalNums = 0;
    if(query3.exec(totalNumsSql) && query3.next()){
        totalNums = query3.value(0).toString().toInt();
    }
    else{
        qDebug()<<"fail";
    }
    QString totalRouteIdSql = QString("select COUNT(routeId) from route where routeId='%1'").arg(routeId);
    QSqlQuery query4;
    int totalRouteId = 0;
    if(query4.exec(totalRouteIdSql) && query4.next()){
        totalRouteId = query4.value(0).toString().toInt();
    }
    else{
        qDebug()<<"fail";
    }
    int ticketNums = floor(totalNums * (placeId2 - placeId1) / totalRouteId);
    if(ticketNums < ticketNumbers){
        ticketNumbers = ticketNums;
    }
    QString ticketNumber = QString::number(ticketNumbers);
    qDebug()<<totalNums;
    qDebug()<<totalRouteId;
    ui->stackedWidget->setCurrentWidget(ui->buyPage);
    ui->userMail2->setText(user.getName());
    ui->ticketNum2->setText(ticketNumber);
    ui->routeId2->setText(route);
    ui->startPlace2->setText(ui->placeComboBox1->currentText());
    ui->endPlace2->setText(ui->placeComboBox2->currentText());
    ui->date2->setText(date);
}

void MainWindow::on_buyButton_clicked()
{
    int route = ui->routeId2->text().toInt();
    QString startPlace = ui->startPlace2->text();
    QString endPlace = ui->endPlace2->text();
    QString date = ui->date2->text();
    if(ui->ticketNum2->text().toInt() == 0){
        QMessageBox::information(this,"错误提示","无余票");
    }
    else{
        QString placeSql = QString("select placeId from place where placeName='%1' or placeName='%2'").arg(startPlace).arg(endPlace);
        QSqlQuery query1;
        int placeId1 = 0;
        int placeId2 = 0;
        if(query1.exec(placeSql)&& query1.next()){
            placeId1 = query1.value(0).toInt();
            query1.next();
            placeId2 = query1.value(0).toInt();
        }
        else{
            qDebug()<<"fail";
        }
        QSqlQuery query2;
        QSqlQuery query3;
        QString insertSql = QString("insert into ticket (username,routeId,start,end,date) values('%1','%2','%3','%4','%5')").arg(user.getName()).arg(route).arg(startPlace).arg(endPlace).arg(date);
        QString updateSql = QString("update route set ticketNumbers=ticketNumbers-1 where startId>='%1' and endId<='%2'").arg(placeId1).arg(placeId2);
        if(query2.exec(insertSql)){
            if(query3.exec(updateSql)){
                QMessageBox::information(this,"更新提示","订票成功");
            }
            else{
                QMessageBox::information(this,"更新提示","订票失败*1");
            }
        }else{
            QMessageBox::information(this,"更新提示","订票失败*2");
        }
    }
}

void MainWindow::on_backTobookButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->bookPage);
}

void MainWindow::on_backToSearchButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->searchPage);
    QString startPlace = ui->placeComboBox1->currentText();
    QString endPlace = ui->placeComboBox2->currentText();
    ui->stackedWidget->setCurrentWidget(ui->searchPage);
    QSqlQueryModel *model = new QSqlQueryModel(ui->tableView);
    QString regist = QString("select placeId from place where placeName='%1' or placeName='%2'").arg(startPlace).arg(endPlace);
    QSqlQuery query;
    int placeId1 = 0;
    int placeId2 = 0;
    if(query.exec(regist)&& query.next()){
        placeId1 = query.value(0).toInt();
        query.next();
        placeId2 = query.value(0).toInt();
    }
    else{
        qDebug()<<"fail";
    }
    model->setQuery(QString("select routeId, start, end, date, startId, endId from route where startId>='%1' and endId<='%2'").arg(placeId1).arg(placeId2));
    //列名
    model->setHeaderData(0, Qt::Horizontal, tr("车次"));
    model->setHeaderData(1, Qt::Horizontal, tr("出发站"));
    model->setHeaderData(2, Qt::Horizontal, tr("终点站"));
    model->setHeaderData(3, Qt::Horizontal, tr("日期"));
    model->setHeaderData(4, Qt::Horizontal, tr("起始站序号"));
    model->setHeaderData(5, Qt::Horizontal, tr("终点站序号"));
    ui->tableView->setModel(model);//数据放置进去
    ui->tableView->verticalHeader()->hide();//不显示序号
}

void MainWindow::on_backToticketsButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->ticketsPage);
    QSqlQueryModel *model = new QSqlQueryModel(ui->ticketsTable);
    model->setQuery(QString("select username, routeId, start, end, date from ticket;"));
    //列名
    model->setHeaderData(0, Qt::Horizontal, tr("个人邮箱"));
    model->setHeaderData(1, Qt::Horizontal, tr("车次"));
    model->setHeaderData(2, Qt::Horizontal, tr("出发站"));
    model->setHeaderData(3, Qt::Horizontal, tr("终点站"));
    model->setHeaderData(4, Qt::Horizontal, tr("发车日期"));
    ui->ticketsTable->setModel(model);//数据放置进去
    ui->ticketsTable->verticalHeader()->hide();//不显示序号
}

void MainWindow::on_ticketsTable_activated(const QModelIndex &index)
{
    QSqlTableModel *model = new QSqlTableModel;
    model->setTable("ticket");
    //列名
    model->setHeaderData(0, Qt::Horizontal, tr("个人邮箱"));
    model->setHeaderData(1, Qt::Horizontal, tr("车次"));
    model->setHeaderData(2, Qt::Horizontal, tr("出发站"));
    model->setHeaderData(3, Qt::Horizontal, tr("终点站"));
    model->setHeaderData(4, Qt::Horizontal, tr("发车日期"));
    model->select();
    ui->ticketsTable->setModel(model);//数据放置进去
    QString username = model->data(model->index(index.row(), 1)).toString();
    QString route = model->data(model->index(index.row(), 2)).toString();
    QString startPlace = model->data(model->index(index.row(), 3)).toString();
    QString endPlace = model->data(model->index(index.row(), 4)).toString();
    QString date = model->data(model->index(index.row(), 5)).toString();
    ui->stackedWidget->setCurrentWidget(ui->refundPage);
    ui->userMail4->setText(user.getName());
    ui->routeId4->setText(route);
    ui->startPlace4->setText(startPlace);
    ui->endPlace4->setText(endPlace);
    ui->date4->setText(date);
}
