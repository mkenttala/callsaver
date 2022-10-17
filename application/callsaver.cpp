#include <QGuiApplication>
#include <QQuickView>

#include "callsavermodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<CallSaverModel>("fi.kende.callsavermodel", 1, 0, "CallSaverModel");
    QQuickView *view = new QQuickView;
    view->setSource(QUrl("qrc:/main.qml"));
    view->show();
    return app.exec();
}
