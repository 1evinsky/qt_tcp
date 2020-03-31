#include "clientworker.h"

#include "core.h"

#include <QHostAddress>
#include <QDataStream>

ClientWorker::ClientWorker(QObject *parent) : QObject(parent)
{

}

void ClientWorker::process()
{
    m_socket = std::make_shared<QTcpSocket>();
    connect(m_socket.get(), &QTcpSocket::readyRead, this, &ClientWorker::sockReady);
    connect(m_socket.get(), &QTcpSocket::disconnected, this, &ClientWorker::sockDisc);
    m_allSize = 0;

    qDebug() << Core::theCore()->settings()->value("address").toString();
    QHostAddress address{Core::theCore()->settings()->value("address").toString()};

    qDebug() << Core::theCore()->settings()->value("port").toUInt();
    auto port{Core::theCore()->settings()->value("port").toUInt()};

    qDebug() << Core::theCore()->settings()->value("value").toDouble();
    double value {Core::theCore()->settings()->value("value").toDouble()};

    m_socket->connectToHost(address, static_cast<quint16>(port));

    QByteArray block;
    QDataStream sendStream(&block, QIODevice::ReadWrite);

    sendStream << value;
    m_socket->write(block);
}

void ClientWorker::sockReady()
{
    QDataStream in(m_socket.get());
    for(;;)
    {
        qDebug() << "m_socket->bytesAvailable() " << m_socket->bytesAvailable();
        if(!m_allSize)
        {
            if (m_socket->bytesAvailable() < sizeof(quint32)) {
                break;
            }
            in >> m_allSize;
            qDebug() << "size " << m_allSize;
        }
        if (m_socket->bytesAvailable() < m_allSize) {
            break;
        }
        while (m_allSize != 0){
            emit writeToFile(m_socket->readAll());
            m_allSize = 0;
        }
    }
    if(m_allSize == 0)
    {
        emit finished();
    }
}

void ClientWorker::sockDisc()
{
    m_socket->deleteLater();
}
