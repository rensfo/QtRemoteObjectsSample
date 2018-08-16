#include "sensorobserver.h"

SensorObserver::SensorObserver(SensorReplica * replica, QObject * parent)
    : QObject(parent)
    , m_connectionState(false)
    , m_replica(replica)
{
    connect(m_replica, &SensorReplica::idChanged, this, &SensorObserver::idChanged);
    connect(m_replica, &SensorReplica::humidityChanged, this, &SensorObserver::humidityChanged);
    connect(m_replica, &SensorReplica::temperatureChanged, this, &SensorObserver::temperatureChanged);
    connect(m_replica, &SensorReplica::stateChanged, this, &SensorObserver::onStateChanged);
}

QString SensorObserver::serverName() const
{
    return m_serverName;
}

void SensorObserver::setServerName(const QString & serverName)
{
    m_serverName = serverName;
    emit serverNameChanged(m_serverName);
}

bool SensorObserver::connectionState() const
{
    return m_connectionState;
}

void SensorObserver::setConnectionState(bool connectionState)
{
    m_connectionState = connectionState;
    emit connectionStateChanged(m_connectionState);
}

int SensorObserver::id() const
{
    return m_replica->id();
}

float SensorObserver::humidity() const
{
    return m_replica->humidity();
}

float SensorObserver::temperature() const
{
    return m_replica->temperature();
}

QString SensorObserver::sourceName() const
{
    return m_sourceName;
}

void SensorObserver::setSourceName(const QString & objectName)
{
    m_sourceName = objectName;
    emit sourceNameChanged(m_sourceName);
}

void SensorObserver::onStateChanged(QRemoteObjectReplica::State state, QRemoteObjectReplica::State oldState)
{
    Q_UNUSED(oldState)

    setConnectionState(state == QRemoteObjectReplica::Valid);
//    qDebug() << m_sourceName << "onStateChanged" << state;
}

SensorReplica *SensorObserver::replica() const
{
    return m_replica;
}
