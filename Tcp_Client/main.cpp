#include "core.h"
#include "client.h"

int main(int argc, char *argv[])
{
    static const QString ini_file{"/../client_settings/setting.ini"};

    Core core{argc, argv, ini_file};

    Client client;
    client.start();

    return core.theCore()->exec();
}
