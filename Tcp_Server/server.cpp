#include "server.h"
#include "core.h"
#include <QDataStream>

Server::Server(QObject *parent) :QTcpServer{parent}
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
    m_socket = std::make_shared<QTcpSocket>(this);
    m_socket->setSocketDescriptor(handle);

    connect(m_socket.get(), &QTcpSocket::readyRead, this, &Server::sockReady);
    connect(m_socket.get(), &QTcpSocket::disconnected, this, &Server::sockDisck);

    //m_socket->write("Connection client!");
    qDebug() << "Have new connection " << handle;
}

void Server::sockReady()
{
    QByteArray data = m_socket->readAll();
    qDebug() << data;
    QDataStream data_s(&data, QIODevice::ReadOnly);
    data_s.setFloatingPointPrecision(QDataStream::DoublePrecision);
    data_s >> m_value;
    qDebug() << m_value;

    sendArray();
}

void Server::sockDisck()
{
    qDebug() << "Disconnect client";
    m_socket->deleteLater();
}
void Server::sendArray()
{
    qDebug() << "create block";
    QByteArray block;
    QDataStream sendStream(&block, QIODevice::ReadWrite);
    sendStream << quint32(0);
    for(auto i = 0; i < 1000000; i++)
    {
        sendStream << m_value++;
    }
    sendStream.device()->seek(0);
    sendStream << (quint32)(block.size() - sizeof(quint32));
    qDebug() << "write to socket size : " << block.size();
    m_socket->write(block);
    //m_socket->waitForBytesWritten();
}
