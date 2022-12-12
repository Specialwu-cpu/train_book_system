#include "mainwindow.h"
#include "loginbox.h"
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
    QString userInfo = "CREATE TABLE IF not EXISTS userinfo(userId INTEGER PRIMARY KEY,username VARCHAR(64),password VARCHAR(64), imagePath VARCHAR(255));";
    QString route = "create table if not exists route(routeId INTEGER PRIMARY KEY,start varchar(64),end varchar(64),ticketNumbers int);";
    QString ticket = "create table if not exists ticket(ticketId INTEGER PRIMARY KEY,username varchar(64), routeId int,FOREIGN KEY (username) REFERENCES userinfo(username),FOREIGN KEY (routeId) REFERENCES route(routeId));";
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
    //自增主键
    QString add = "ALTER userinfo ADD userId int IDENTITY (1, 1) NOT NULL;";
    if(query.exec(add)){
        qDebug()<<"add success";
    }
//    //数据库关闭
//    db.close();
//    return 0;

    QApplication a(argc, argv);
    LoginBox lBox;
//    lBox.setFixedSize(450,253);
    lBox.show();
    return a.exec();
}
