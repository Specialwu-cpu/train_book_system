#include "mainwindow.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>


int main(int argc, char *argv[])
{
    //创建数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //设置数据库名,指定打开数据库文件
    db.setDatabaseName("trainBookSystem.db");

    //指定打开数据库文件
//    db.setDatabaseName("login.db");
//    //设置主机
//    db.setHostName("localhost");
//    //设置端口号
//    db.setPort(1433);
//    //设置数据库账号
//    db.setUserName("sa");
//    //设置数据库密码
//    db.setPassword("123456");
    //打开数据库
    if(db.open()){
        qDebug()<<"open database success";
    }else
    {
        qDebug()<<"open database fail";
    }

    //创建一个数据库表存储用户名和密码
    QString userInfo = "CREATE TABLE IF not EXISTS userinfo(userId INT PRIMARY KEY,username VARCAHR(64),password VARCHAR(64));";
    QString route = "create table if not exists route(routeId int PRIMARY KEY,start varchar(64),end varchar(64),ticketNumbers int);";
    QString ticket = "create table if not exists ticket(ticketId int PRIMARY KEY,username varchar(64), routeId int,FOREIGN KEY (username) REFERENCES userinfo(username),FOREIGN KEY (routeId) REFERENCES route(routeId));";
    //执行sql语句
    QSqlQuery query;
    //创建用户表并执行
    if(query.exec(userInfo)){
        qDebug()<<"exec success";
    }
    //创建车次表并执行
    if(query.exec(route)){
        qDebug()<<"exec success";
    }
    //创建订票记录并执行
    if(query.exec(ticket)){
        qDebug()<<"exec success";
    }
//    //数据库关闭
//    db.close();
//    return 0;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
