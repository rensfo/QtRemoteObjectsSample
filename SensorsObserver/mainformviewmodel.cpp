#include "mainformviewmodel.h"
#include "rep_sensor_replica.h"
#include "sensorobserver.h"

#include <QDebug>

MainFormViewModel::MainFormViewModel(QObject * parent)
    : QObject(parent)
    , m_urlRegistry("tcp://127.0.0.1:9900")
    , m_clientNode(new QRemoteObjectNode(m_urlRegistry))
{
    auto registry = m_clientNode->registry();
    connect(registry, &QRemoteObjectRegistry::remoteObjectAdded, this, &MainFormViewModel::onRemoteObjectAdded);
    connect(registry, &QRemoteObjectRegistry::remoteObjectRemoved, this, &MainFormViewModel::onRemoteObjectRemoved);
    connect(registry, &QRemoteObjectRegistry::initialized, this, &MainFormViewModel::onRegistryInitialized);
}

QList<SensorObserver *> MainFormViewModel::sensorObservers() const
{
    return m_sensorObservers;
}

void MainFormViewModel::setSensorObservers(const QList<SensorObserver *> & value)
{
    m_sensorObservers = value;
    emit sensorObserversChanged(m_sensorObservers);
}

void MainFormViewModel::onRemoteObjectAdded(const QRemoteObjectSourceLocation & entry)
{
    if (!m_sensorObserverRegistry.contains(entry.second.hostUrl.toString()) or
        !m_sensorObserverRegistry[entry.second.hostUrl.toString()].contains(entry.first)) {
        auto observer = createObserver(entry.second.hostUrl.toString(), entry.first);
        if (observer->replica()->isInitialized()) {
            addToObservers(observer);
            emit sensorObserversChanged(m_sensorObservers);
            emit sensorsChanged(m_sensors);
        } else {
            addToNotInitialized(observer);
        }
        //        qDebug() << Q_FUNC_INFO << entry.first;
    }
}

void MainFormViewModel::onRemoteObjectRemoved(const QRemoteObjectSourceLocation & entry)
{
    if (m_sensorObserverRegistry.contains(entry.second.hostUrl.toString()) and
        m_sensorObserverRegistry[entry.second.hostUrl.toString()].contains(entry.first)) {
        m_sensorObserverRegistry[entry.second.hostUrl.toString()][entry.first]->setConnectionState(false);
        //        qDebug() << Q_FUNC_INFO << entry.first;
    }
}

void MainFormViewModel::onRegistryInitialized()
{
    m_sensorObservers.clear();

    QHashIterator<QString, QRemoteObjectSourceLocationInfo> it(m_clientNode->registry()->sourceLocations());
    bool isOneInitialized = false;
    while (it.hasNext()) {
        it.next();
        QString hostName = it.value().hostUrl.toString();
        SensorObserver * observer = createObserver(hostName, it.key());
        if (observer->replica()->isInitialized()) {
            addToObservers(observer);
            isOneInitialized = true;
        } else {
            addToNotInitialized(observer);
        }
    }
    if (isOneInitialized) {
        sortSensors();
        emit sensorObserversChanged(m_sensorObservers);
        emit sensorsChanged(m_sensors);
    }
}

SensorObserver * MainFormViewModel::createObserver(const QString & hostName, const QString objectName) const
{
    auto seplica = m_clientNode->acquire<SensorReplica>(objectName);
    SensorObserver * observer(new SensorObserver(seplica));
    observer->setServerName(hostName);
    observer->setSourceName(objectName);
    observer->setConnectionState(true);

    return observer;
}

void MainFormViewModel::sortSensors()
{
    std::sort(m_sensorObservers.begin(), m_sensorObservers.end(), [](SensorObserver * first, SensorObserver * second) {
        return first->id() < second->id();
    });
//    QList<int> ids;
//    for (auto o : m_sensorObservers) {
//        ids << o->id();
//    }
//    qDebug() << ids;
    m_sensors.clear();
    std::copy(m_sensorObservers.begin(), m_sensorObservers.end(), std::back_inserter(m_sensors));
}

void MainFormViewModel::addToObservers(SensorObserver * observer)
{
    m_sensorObservers << observer;
    m_sensorObserverRegistry[observer->serverName()][observer->sourceName()] = observer;
    sortSensors();
}

void MainFormViewModel::addToNotInitialized(SensorObserver * observer)
{
    m_notInisizlizedSensors << observer;
    connect(observer->replica(), &SensorReplica::initialized, [observer, this]() {
        if (m_notInisizlizedSensors.contains(observer)) {
            addToObservers(observer);
            m_notInisizlizedSensors.removeOne(observer);
            emit sensorObserversChanged(m_sensorObservers);
            emit sensorsChanged(m_sensors);
        }
    });
}

QList<QObject *> MainFormViewModel::sensors() const
{
    return m_sensors;
}
