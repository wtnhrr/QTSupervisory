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

    QStringList list;
    getIps();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getData()
{

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

void MainWindow::update()
{

}

void MainWindow::interval()
{
    int interval = ui->horizontalSliderTiming->value();
    timing->setInterval(interval * 1000);
}

void MainWindow::getIps()
{
    QString str;
    qDebug() << "Pegando a lista de IPs de produtores...";

    if (socket->state() != QAbstractSocket::ConnectedState || !socket->isOpen())
    {
        return;
    }

    socket->write("list\r\n");
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    qDebug() << socket->bytesAvailable();
    ui->listIps->clear();

    while (socket->bytesAvailable())
    {
        str = socket->readLine().replace("\n", "").replace("\r", "");
        ui->listIps->addItem(str);
    }
}
