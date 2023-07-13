#ifndef SERVERLIST_H
#define SERVERLIST_H

#include <QTcpServer>
#include <QDebug>
#include <QStringList>

class serverList : public QTcpServer
{
    Q_OBJECT
public:
    serverList();

    QStringList getIPList();

private:
    QStringList iplist;
};

#endif // SERVERLIST_H
