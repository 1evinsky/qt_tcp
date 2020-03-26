#include "core.h"
#include <QDir>
#include <QDebug>

Core::Core(int &argc, char **argv, const QString &ini_file)
    : QCoreApplication{argc, argv}
{
    auto path = QDir::currentPath();
    const QString ini_file_name = path + ini_file;

    m_Settings = std::make_shared<QSettings>(ini_file_name,
                                             QSettings::IniFormat, this);
    if(m_Settings->status() == QSettings::FormatError)
    {
        qDebug() << "CoreSettings: Format error";
    }
}

QCoreApplication *Core::theCore()
{
    return QCoreApplication::instance();
}

QSettings *Core::settings()
{
    return m_Settings.get();
}
