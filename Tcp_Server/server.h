#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <memory>

#include "serverthread.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    ~Server() override;

//    std::shared_ptr<QTcpSocket> m_socket;
//    QByteArray m_data;

public slots:
    void startServer();
    void incomingConnection(qintptr handle) override;
    void finish();
//    void sockReady();
//    void sockDisck();

private:
    ServerThread *sThread;
//    void sendArray();
//    double m_value = 0;
};

#endif // SERVER_H
