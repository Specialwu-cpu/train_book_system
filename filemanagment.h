#ifndef FILEMANAGMENT_H
#define FILEMANAGMENT_H
#include <QString>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QWidget>
#include <QProcess>
#include <iostream>
#include <QThread>
#include <QFile>
using namespace std;


class FileManagment: public QWidget
{
    Q_OBJECT
public:
    explicit FileManagment(QWidget *parent = nullptr);
    ~FileManagment();
    static FileManagment * getFileManagment();

    QTcpServer *server;
    QTcpSocket *socket;
    QTcpServer *fileserver;
    QTcpSocket *filesocket;


private slots:
    void acceptFileConnection();
    void updateFileProgress();
    void displayError(QAbstractSocket::SocketError socketError);

private:
    static FileManagment *widget;
//    传送文件相关变量
    qint64 totalBytes;
    qint64 bytesReceived;
    qint64 bytestoWrite;
    qint64 bytesWritten;
    qint64 filenameSize;
    QString filename;
    ///每次发送数据大小
    qint64 perDataSize;
    QFile *localFile;
//    本地缓冲区
    QByteArray inBlock;
    QByteArray outBlock;
};

#endif // FILEMANAGMENT_H
