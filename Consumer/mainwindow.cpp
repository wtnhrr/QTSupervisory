#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QStringList>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

    timing = new QTimer(this);
    timing->setInterval(1000);


    connect(ui->pushButtonConnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpConnect()));

    connect(ui->pushButtonDesconnect,
            SIGNAL(clicked(bool)),
            this,
            SLOT(tcpDisconnect()));

    connect(ui->pushButtonUpdate,
            SIGNAL(clicked(bool)),
            this,
            SLOT(listIps()));

    connect(ui->horizontalSliderTiming,
            SIGNAL(valueChanged(int)),
            SLOT(interval()));

    connect(ui->pushButtonStart,
            SIGNAL(clicked(bool)),
            this,
            SLOT(start()));

    connect(ui->pushButtonStop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(stop()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getData()
{
    QString str;
    QByteArray array;
    QStringList list;
    qint64 thetime;

    qDebug() << "to get data...";
    if(socket->state() == QAbstractSocket::ConnectedState){
        if(socket->isOpen()){
            qDebug() << "reading...";
            socket->write("get 127.0.0.1 5\r\n");
            socket->waitForBytesWritten();
            socket->waitForReadyRead();
            qDebug() << socket->bytesAvailable();

            while(socket->bytesAvailable()){
                str = socket->readLine().replace("\n","").replace("\r","");
                list = str.split(" ");
                if(list.size() == 2){
                    bool ok;
                    str = list.at(0);
                    thetime = str.toLongLong(&ok);
                    str = list.at(1);
                    qDebug() << thetime << ": " << str;
                }
            }
        }
    }
}

void MainWindow::timming()
{
    int interval = ui->horizontalSliderTiming->value();
    timing->setInterval(interval * 1000);
}

void MainWindow::tcpConnect()
{
    ip = ui->lineEditIPServer->displayText();
    socket->connectToHost(ip,1234);
    qDebug() << "Conectado\n";

}

void MainWindow::tcpDisconnect()
{
    socket->disconnectFromHost();
    qDebug() << "Desconectado\n";
}

void MainWindow::start()
{
    connect(timing, &QTimer::timeout, this, &MainWindow::getData);
    timing->start();
}

void MainWindow::stop()
{
    timing->stop();
}

void MainWindow::interval()
{
    int interval = ui->horizontalSliderTiming->value();
    timing->setInterval(interval * 1000);
}

void MainWindow::listIps()
{
    QString str, list;
    QListWidget* item = ui->listIps;

    list = "list\r\n";
    ui->listIps->clear();
    if(socket->state() == QAbstractSocket::ConnectedState){
        if(socket->isOpen()){

            socket->write(list.toStdString().c_str());
            socket->waitForBytesWritten();
            socket->waitForReadyRead();
            qDebug() << socket->bytesAvailable();
            while(socket->bytesAvailable()){
                str = socket->readLine().replace("\n","").replace("\r","");
                qDebug() << str;
                ui->listIps->addItem(str);
            }
        }
    }
}

void MainWindow::update()
{
    tcpDisconnect();
    QListWidgetItem *item = ui->listIps->currentItem();
    ui->lineEditIPServer->setText(item->text());
}
