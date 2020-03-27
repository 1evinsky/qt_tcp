#include "client.h"
#include "core.h"
#include <iostream>
#include <QHostAddress>
#include <QDataStream>

Client::Client(QObject *parent) : QObject(parent)
{
    m_socket = std::make_shared<QTcpSocket>();
    connect(m_socket.get(), &QTcpSocket::readyRead, this, &Client::sockReady);
    connect(m_socket.get(), &QTcpSocket::disconnected, this, &Client::sockDisc);
    m_nNextBlockSize = 0;
}

void Client::start()
{
     qDebug() << Core::theCore()->settings()->value("address").toString();
     QHostAddress address{Core::theCore()->settings()->value("address").toString()};

     qDebug() << Core::theCore()->settings()->value("port").toUInt();
     auto port{Core::theCore()->settings()->value("port").toUInt()};

     qDebug() << Core::theCore()->settings()->value("value").toDouble();
     double value {Core::theCore()->settings()->value("value").toDouble()};

     m_socket->connectToHost(address, static_cast<quint16>(port));

     QByteArray block;
     QDataStream sendStream(&block, QIODevice::ReadWrite);

     //sendStream << quint16(0);
     sendStream << value;
     //sendStream.device()->seek(0);
     //sendStream << (quint16)(block.size() - sizeof(quint16));
     m_socket->write(block);
}

void Client::sockReady()
{
//    if(m_socket->waitForConnected(500))
//    {
//        m_socket->waitForReadyRead(500);
//    }
//    double buff;
//    QDataStream stream(m_socket.get());
//    stream.setFloatingPointPrecision(QDataStream::DoublePrecision);
//    qint16 m_msgSize = -1;
//    while(true) {
//      if (m_msgSize < 0) {
//         if (m_socket->bytesAvailable() < sizeof(int))
//           return;
//         stream >> m_msgSize;
//         qDebug() << "size: " << m_msgSize;
//      }
//      else {
//        if (m_socket->bytesAvailable() < m_msgSize)
//           return;
//        for(int i = 0; i < (m_msgSize/sizeof(double)); i++)
//        {
//            stream >> buff;
//            qDebug() << i << " data: " << buff;
//        }
//        m_msgSize = -1;
//      }
//    }
    //m_socket->waitForReadyRead();
    QDataStream in(m_socket.get());
    for(;;)
    {
        qDebug() << "m_socket->bytesAvailable() " << m_socket->bytesAvailable();
        if(!m_nNextBlockSize)
        {
            if (m_socket->bytesAvailable() < sizeof(quint32)) {
                break;
            }
            in >> m_nNextBlockSize;
            qDebug() << "size " << m_nNextBlockSize;
        }
        if (m_socket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        double value;
        int i = 0;
        while (m_nNextBlockSize != 0){
            in >> value;
            if(i%100000 == 0)
            {
                qDebug() << value;
            }
            i++;
            m_nNextBlockSize = m_nNextBlockSize - sizeof(double);
        }
    }
    Core::theCore()->exit(0);
}

void Client::sockDisc()
{
    m_socket->deleteLater();
}
