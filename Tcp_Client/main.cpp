#include "core.h"
#include "client.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    static const QString ini_file{"/../client_settings/setting.ini"};

    Core core{argc, argv, ini_file};

    Client client;
    client.start();

    qDebug() << core.settings()->value("test").toString();

    return core.theCore()->exec();
}
