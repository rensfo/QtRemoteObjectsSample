#include <QCoreApplication>
#include <QRemoteObjectHost>
#include <QTimer>

#include "sensor.h"

const int hostsCount = 10;
const QUrl urlRegistry("tcp://127.0.0.1:9900");
const QString urlTempale("local:host%1");
const QString nameTemplate("Sensor%1");

int main(int argc, char * argv[])
{
    QCoreApplication a(argc, argv);

    QList<QSharedPointer<Sensor>> sensors;
    QHash<QSharedPointer<Sensor>, bool> sensorRemoting;
    QHash<QSharedPointer<Sensor>, QSharedPointer<QRemoteObjectHost>> sensorsHosts;

    int id = 0;
    for (int h = 1; h <= hostsCount; ++h) {
        const QString hostName = urlTempale.arg(h);
        QSharedPointer<QRemoteObjectHost> newHost(new QRemoteObjectHost(hostName, urlRegistry));

        const int sensorsCount = qMax(qrand() % 3, 1);
        for(int s = 1; s <= sensorsCount; ++s) {
            QSharedPointer<Sensor> newSensor(new Sensor(++id));
            sensors << newSensor;
            sensorRemoting[newSensor] = false;
            sensorsHosts[newSensor] = newHost;
        }
    }

    QTimer t;
    t.setInterval(5000);
    t.setSingleShot(false);

    int currentSensor = 0;
    QObject::connect(&t, &QTimer::timeout, [&currentSensor, &sensors, &sensorRemoting, &sensorsHosts]() {
        auto sensor = sensors.value(currentSensor);
        sensor->setHumidity(static_cast<float>(qrand() % 100) + static_cast<float>(qrand() % 100) / 100);
        sensor->setTemperature(static_cast<float>(qrand() % 30) + static_cast<float>(qrand() % 100) / 100);

        auto host = sensorsHosts[sensor];
        bool & isRemotingEnabled = sensorRemoting[sensor];
        if (isRemotingEnabled) {
            if (qrand() % 2) {
                host->disableRemoting(sensor.data());
                isRemotingEnabled = false;
            } else {
                isRemotingEnabled = true;
            }
        } else {
            host->enableRemoting(sensor.data(), nameTemplate.arg(sensor->id()));
            isRemotingEnabled = true;
        }
        ++currentSensor;
        if (currentSensor >= sensors.count())
            currentSensor = 0;
    });
    t.start();
    return a.exec();
}
