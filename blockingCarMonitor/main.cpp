#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "map.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Map& myMap = Map::getInstance();
    auto rootContext = engine.rootContext();
    rootContext->setContextProperty("MapClass",&myMap);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();

}
