#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "mainformviewmodel.h"
#include "sensorobserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

//    qRegisterMetaType<SensorObserver*>("SensorObserver*");
    qRegisterMetaType<QList<SensorObserver*>>("QList<SensorObserver*>");
    qmlRegisterType<MainFormViewModel>("my.sensors", 1, 0, "MainFormViewModel");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
