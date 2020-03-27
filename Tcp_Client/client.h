#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <memory>
#include <vector>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    std::shared_ptr<QTcpSocket> m_socket;
    int m_status = 2;
    QByteArray m_data;
    //std::vector<double> array;
    quint32 m_nNextBlockSize;
    void start();

signals:

public slots:
    void sockReady();
    void sockDisc();
};

#endif // CLIENT_H
