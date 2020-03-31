#include "client.h"

#include "core.h"

#include <QDateTime>

Client::Client(QObject *parent) : QObject(parent)
{
    m_thread = new QThread;
    m_worker = new ClientWorker(this);
    m_file = new QFile;
}

Client::~Client()
{
    if(m_thread->isRunning()){
        m_thread->exit(0);
    }
    if(m_file->isOpen()){
        m_file->close();
    }
}

void Client::start()
{
     m_worker->moveToThread(m_thread);
     connect(m_worker, &ClientWorker::error, this, &Client::errorString);
     connect(m_worker, &ClientWorker::writeToFile, this, &Client::writeToFile);
     connect(m_thread, &QThread::started, m_worker, &ClientWorker::process);
     connect(m_worker, &ClientWorker::finished, m_thread, &QThread::quit);
     connect(m_worker, &ClientWorker::finished, m_worker, &ClientWorker::deleteLater);
     connect(m_thread, &QThread::finished, m_thread, &Client::deleteLater);
     connect(m_thread, &QThread::finished, this, &Client::finished);
     m_thread->start();
}

void Client::errorString(QString error)
{
    qDebug() << error;
}

void Client::writeToFile(QByteArray data)
{
    const QString filename = "data_" + QDateTime::currentDateTime().toString("hh_mm_ss_zzz");
    m_file->setFileName(filename);
    if(!m_file->open(QFile::ReadWrite))
    {
        qDebug() << "no open";
        return;
    }
    m_file->write(data.data(),data.size());
    m_file->waitForBytesWritten(3000);
    m_file->close();
    qDebug() << "Write to File. File size is: " << m_file->size();
}

void Client::finished()
{
    Core::theCore()->exit(0);
}
