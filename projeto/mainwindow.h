#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <QSet>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void putData();
    void tcpConnect();
    void tcpDisconnect();
    void start();
    void stop();
    void valores();
    void intervalo();

private:
    Ui::MainWindow *ui;

    QTcpSocket *socket;
    QTimer *timing;
    QString ip;
    QSet<QString> ipSet;
};
#endif // MAINWINDOW_H
