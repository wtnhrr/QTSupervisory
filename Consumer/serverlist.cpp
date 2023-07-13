#include "serverlist.h"

#include <QNetworkInterface>

serverList::serverList()
{

}

QStringList serverList::getIPList()
{
    if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        iplist << address.toString();
    }

    return iplist;
}
