#include "server.h"
#include "core.h"
#include <QDataStream>

Server::Server(QObject *parent) :QTcpServer{parent}
{

}

Server::~Server()
{

}

void Server::startServer()
{
    qDebug() << Core::theCore()->settings()->value("address").toString();
    QHostAddress address{Core::theCore()->settings()->value("address").toString()};

    qDebug() << Core::theCore()->settings()->value("port").toUInt();
    auto port{Core::theCore()->settings()->value("port").toUInt()};

    if(this->listen(address, static_cast<quint16>(port)))
    {
        qDebug() << "Listetning";
    } else {
        qDebug() << "not listeting";
    }
}

void Server::incomingConnection(qintptr handle)
{
    qDebug() << "Have new connection " << handle;
    sThread = new ServerThread(handle, this);
    connect(sThread, &ServerThread::finished,
            sThread, &ServerThread::deleteLater);
    connect(sThread, &ServerThread::finished,
            this, &Server::finish);
    sThread->start();
}

void Server::finish()
{
    qDebug() << "Stop";
}
