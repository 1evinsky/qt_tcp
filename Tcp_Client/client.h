#ifndef CLIENT_H
#define CLIENT_H

#include "clientworker.h"

#include <QThread>
#include <QObject>
#include <QFile>

#include <memory>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();
    void start();

signals:

public slots:
    void errorString(QString error);
    void writeToFile(QByteArray data);
    void finished();

private:
    QFile *m_file = nullptr;
    QThread *m_thread = nullptr;
    ClientWorker *m_worker = nullptr;
};

#endif // CLIENT_H
