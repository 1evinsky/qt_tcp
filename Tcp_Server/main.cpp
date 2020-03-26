#include "core.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    static const QString ini_file{"/../server_settings/setting.ini"};

    Core core{argc, argv, ini_file};

    qDebug() << core.settings()->value("test").toString();

    return core.theCore()->exec();
}
