#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>

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
    void getData();
    void timming();
    void tcpConnect();
    void tcpDisconnect();
    void start();
    void stop();
    void update();
    void interval();
    void listIps();

private:
    Ui::MainWindow *ui;

    QTcpSocket *socket;
    QTimer *timing;
    QString ip;
};
#endif // MAINWINDOW_H
