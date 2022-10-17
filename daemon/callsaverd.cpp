#include <QCoreApplication>
#include "daemon.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    new daemon(&app);
    return app.exec();
}
