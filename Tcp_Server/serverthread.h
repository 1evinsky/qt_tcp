#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QTcpSocket>
#include <QThread>
#include <memory>

class ServerThread : public QThread
{
    Q_OBJECT
public:
    ServerThread(const int ID, QObject *parent = nullptr);
    ~ServerThread() override;
    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);

public slots:
    void readyRead();
    void disconnected();

private:
    std::shared_ptr<QTcpSocket> m_socket;
    int m_socketDescription;
    double m_value;

    void sendArray();

};

#endif // SERVERTHREAD_H
