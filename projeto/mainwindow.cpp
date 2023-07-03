#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cstdlib>
#include <QTcpSocket>
#include <QDateTime>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow){

    ui->setupUi(this);

    socket = new QTcpSocket(this);

    timer = new QTimer(this);
    timer->setInterval(1000);

    tcpConnect();
    valores();
    intervalo();

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
            SLOT(intervalo()));

    connect(ui->pushButtonStart,
            SIGNAL(clicked(bool)),
            this,
            SLOT(start()));

    connect(ui->pushButtonStop,
            SIGNAL(clicked(bool)),
            this,
            SLOT(stop()));


}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::tcpConnect()
{
    ip = ui->lineEditIP->displayText();
    socket->connectToHost(ip,1234);

    if(socket->waitForConnected()){
        qDebug() << "Conectado";
        ui->textBrowser->setText("Conectado\n");
    } else{
        qDebug() << "Falha ao conectar";
        ui->textBrowser->setText("Desconectado\n");
    }
}

void MainWindow::tcpDisconnect()
{
    socket->disconnectFromHost();
    qDebug() << "Desconectado\n";
    ui->textBrowser->setText("Desconectado\n");
}

void MainWindow::putData()
{
    QDateTime datetime;
    QString str;
    qint64 msecdate;

    if(socket->state()== QAbstractSocket::ConnectedState){

        msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
        str = "set "+ QString::number(msecdate) + " " +
              QString::number(rand()%35)+"\r\n";

        qDebug() << str;
        qDebug() << socket->write(str.toStdString().c_str())
                 << " bytes written";
        if(socket->waitForBytesWritten(3000)){
            qDebug() << "wrote";
        }
    }
}

void MainWindow::valores()
{
    // Definindo os valores iniciais dos sliders
    int min = QRandomGenerator::global()->bounded(0, 100); // Valor mínimo aleatório entre 0 e 99
    int max = QRandomGenerator::global()->bounded(min, 100); // Valor máximo aleatório entre minValue e 99

    ui->horizontalSliderEventoMin->setValue(min);
    ui->horizontalSliderEventoMax->setValue(max);

    // Atualizando os lcdNumbers
    ui->lcdNumberEventoMin->display(min);
    ui->lcdNumberEventoMax->display(max);
}

void MainWindow::intervalo()
{
    int interval = ui->horizontalSliderTiming->value();
    timer->setInterval(interval * 1000);
}

void MainWindow::start()
{
    connect(timer, &QTimer::timeout, this, &MainWindow::putData);
    ui->textBrowser->setText("starting\n");
    timer->start();
}

void MainWindow::stop()
{
    ui->textBrowser->setText("stopping\n");
    timer->stop();
}
