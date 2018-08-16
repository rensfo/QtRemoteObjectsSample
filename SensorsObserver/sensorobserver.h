#ifndef SENSOROBSERVER_H
#define SENSOROBSERVER_H

#include "rep_sensor_replica.h"

class SensorObserver : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString host READ serverName NOTIFY serverNameChanged)
    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(QString name READ sourceName NOTIFY sourceNameChanged)
    Q_PROPERTY(float humidity READ humidity NOTIFY humidityChanged)
    Q_PROPERTY(float temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(bool connection READ connectionState NOTIFY connectionStateChanged)

public:
    explicit SensorObserver(SensorReplica * replica, QObject * parent = nullptr);

    QString serverName() const;
    void setServerName(const QString & serverName);

    bool connectionState() const;
    void setConnectionState(bool connectionState);

    int id() const;
    float humidity() const;
    float temperature() const;

    QString sourceName() const;
    void setSourceName(const QString &sourceName);

    SensorReplica *replica() const;

signals:
    void serverNameChanged(const QString &);
    void sourceNameChanged(const QString &);
    void connectionStateChanged(bool);
    void idChanged(int);
    void humidityChanged(float);
    void temperatureChanged(float);

public slots:

private slots:
    void onStateChanged(QRemoteObjectReplica::State state, QRemoteObjectReplica::State oldState);

private:
    QString m_serverName;
    QString m_sourceName;
    bool m_connectionState;
    SensorReplica * m_replica;
};

//Q_DECLARE_METATYPE(SensorObserver *)
//Q_DECLARE_METATYPE(QList<SensorObserver *>)

#endif  // SENSOROBSERVER_H
