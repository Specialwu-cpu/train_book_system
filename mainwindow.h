#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "book.h"
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setUser(User user);

private slots:
    void on_bookButton_clicked();

    void on_searchButton_clicked();

    void on_backButton_clicked();

    void on_passwordButton_clicked();

private:
    Ui::MainWindow *ui;
    User user;
};
#endif // MAINWINDOW_H
