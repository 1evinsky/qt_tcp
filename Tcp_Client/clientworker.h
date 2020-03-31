#ifndef CLIENTWORKER_H
#define CLIENTWORKER_H

#include <QObject>
#include <QTcpSocket>

#include <memory>

class ClientWorker : public QObject
{
    Q_OBJECT
public:
    explicit ClientWorker(QObject *parent = nullptr);

signals:
    void finished();
    void error(QString err);
    void writeToFile(QByteArray byte);

public slots:
    void process();

    void sockReady();
    void sockDisc();

private:
    std::shared_ptr<QTcpSocket> m_socket;
    quint32 m_allSize;
};

#endif // CLIENTWORKER_H
