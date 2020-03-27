#include "core.h"
#include "server.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    static const QString ini_file{"/../server_settings/setting.ini"};

    Core core{argc, argv, ini_file};

    Server server;
    server.startServer();

    core.theCore()->quit();
    return core.theCore()->exec();
}
