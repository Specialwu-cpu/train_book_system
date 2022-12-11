#include "filemanagment.h"
#include <QDataStream>
#include <QMessageBox>
#include <QString>
#include <QByteArray>
#include <QApplication>
#include <QDir>
#include <QTextCodec>

FileManagment * FileManagment::widget=nullptr;
FileManagment::FileManagment(QWidget *parent) : QWidget(parent)
{
    // 文件传送套接字
    this->filesocket = new QTcpSocket(this);
    this->fileserver = new QTcpServer(this);
    this->fileserver->listen(QHostAddress::Any,8888);
    connect(this->fileserver,SIGNAL(newConnection()),this,SLOT(acceptFileConnection()));

    // 文件传送相关变量初始化
    bytesReceived = 0;
    totalBytes = 0;
    filenameSize = 0;
}

FileManagment *FileManagment::getFileManagment()
{
    if(widget==nullptr)
    {
        widget = new FileManagment();
    }
    return widget;
}

FileManagment::~FileManagment()
{
    close();
//    delete m_sever;
}


void FileManagment::acceptFileConnection()
{
    bytesWritten = 0;
    //每次发送数据大小为64kb
    perDataSize = 64*1024;
    this->filesocket = this->fileserver->nextPendingConnection();
    //接受文件
    connect(filesocket,SIGNAL(readyRead()),this,SLOT(updateFileProgress()));
    connect(filesocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(updateFileProgress(qint64)));
    connect(filesocket,SIGNAL(bytesWritten(qint64)),this,SLOT(displayError(QAbstractSocket::SocketError socketError)));

    qDebug() << "acceptFileConnection  ";
}

void FileManagment::updateFileProgress()
{
    QDataStream inFile(this->filesocket);
    inFile.setVersion(QDataStream::Qt_4_8);

    //如果接收到的数据小于16个字节，保存到来的文件头结构
    if(bytesReceived <= sizeof(qint64)*2)
    {
        if((filesocket->bytesAvailable()>=(sizeof(qint64))*2) && (filenameSize==0))
        {
            inFile>>totalBytes>>filenameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if((filesocket->bytesAvailable()>=filenameSize) && (filenameSize != 0))
        {
            inFile>>filename;
            bytesReceived += filenameSize;
            //创建同名文件
            QString filePath = QApplication::applicationDirPath() + "/../../S_Video_v3/fileData";
            qDebug() << " 文件夹filePath!" << filePath;
            QDir dir(filePath);

//            QDir *temp = new QDir;
            bool exist = dir.exists(filePath);
            if(exist)
                qDebug() << "文件夹已经存在！";
            else
            {
                bool ok = dir.mkdir(filePath);
                if( ok )
                qDebug() << "文件夹不存在，创建成功！";
            }

            filePath = dir.absolutePath() + QString("/%1").arg(filename);
            localFile = new QFile(filePath);

//            //            接收的文件放在指定目录下
//            filename = "ClientData/"+filename;
//            localFile = new QFile(filename);
            if(!localFile->open(QFile::WriteOnly))
            {
                qDebug()<<"Server::open file error!";
                return;
            }
        }
        else
            return;
    }
    //    /如果接收的数据小于总数据，则写入文件
    if(bytesReceived < totalBytes)
    {
        bytesReceived += filesocket->bytesAvailable();
        inBlock = filesocket->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    //    数据接收完成时
    if(bytesReceived == totalBytes)
    {
        //        this->ui.browser->append("Receive file successfully!");
        bytesReceived = 0;
        totalBytes = 0;
        filenameSize = 0;
        localFile->close();
        //filesocket->close();
    }
}

void FileManagment::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug()<< "displayError--" << socket->errorString();
    socket->close();
}
