#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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

    void on_backButton_clicked();

    void on_passwordButton_clicked();

    void on_userInfoButton_clicked();

    void on_ticketsButton_clicked();

    void on_refundButton_clicked();

    void on_doPasswordButton_clicked();

    void on_searchTicketsButton_clicked();

    void on_chooseDateButton_clicked();

    void on_calendarWidget_activated(const QDate &date);

private slots:
    void update_startPlace_list();

    void update_endPlace_list();

    void on_tableView_activated(const QModelIndex &index);

    void on_buyButton_clicked();

    void on_backTobookButton_clicked();

    void on_backToSearchButton_clicked();

    void on_backToticketsButton_clicked();

    void on_ticketsTable_activated(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    User user;
};
#endif // MAINWINDOW_H
