#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginbox.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    Book *book = new Book;
    book->show();
    this->hide();
}

void MainWindow::on_searchButton_clicked()
{
}

void MainWindow::on_backButton_clicked()
{
    LoginBox *login = new LoginBox;
//    login->setFixedSize(450,253);
    login->show();
    this->hide();
}
