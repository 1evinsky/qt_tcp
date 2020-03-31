#include "serverthread.h"

#include <QDataStream>
#include <QDebug>

constexpr unsigned int iteratorValue = 1000000;

ServerThread::ServerThread(const int ID, QObject *parent) :
    QThread{parent},
    m_socketDescription{ID}
{

}

ServerThread::~ServerThread()
{
    m_socket->close();
}

void ServerThread::run()
{
    qDebug() << "STARTING RUN: threadID=" << QThread::currentThread();
    m_socket = std::make_shared<QTcpSocket>();
    bool noError = m_socket->setSocketDescriptor(m_socketDescription);
    if(!noError)
    {
        emit error(m_socket->error());
        return;
    }

    connect(m_socket.get(),&QTcpSocket::readyRead,
            this, &ServerThread::readyRead, Qt::DirectConnection);
    connect(m_socket.get(),&QTcpSocket::disconnected,
            this, &ServerThread::disconnected, Qt::DirectConnection);

    qDebug() << m_socketDescription << " Client Connected";

    exec();
}

void ServerThread::readyRead()
{
    QByteArray data = m_socket->readAll();
    QDataStream data_s(&data, QIODevice::ReadOnly);
    data_s.setFloatingPointPrecision(QDataStream::DoublePrecision);
    data_s >> m_value;
    qDebug() << m_socketDescription << "Start value" << m_value;

    sendArray();
}

void ServerThread::disconnected()
{
    qDebug() << "disconnected";
    m_socket->disconnectFromHost();
    this->quit();
}

void ServerThread::sendArray()
{
    QByteArray block;
    QDataStream sendStream(&block, QIODevice::ReadWrite);
    sendStream << quint32(0);
    for(auto i = 0; i < iteratorValue; i++)
    {
        sendStream << m_value++;
    }
    sendStream.device()->seek(0);
    sendStream << (quint32)(block.size() - sizeof(quint32));
    qDebug() << "write to socket size : " << block.size();
    m_socket->write(block);
}
