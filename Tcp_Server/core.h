#ifndef CORE_H
#define CORE_H


#include <QCoreApplication>
#include <QSettings>
#include <memory>

class Core : public QCoreApplication
{
public:
    Core(int &argc, char **argv, const QString &ini_file);

    static QCoreApplication *theCore();

    QSettings *settings();

private:
    std::shared_ptr<QSettings> m_Settings;
};

#endif // CORE_H
