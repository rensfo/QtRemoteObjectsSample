#ifndef MAINFORMVIEWMODEL_H
#define MAINFORMVIEWMODEL_H

#include <QObject>
#include <QRemoteObjectNode>

class SensorObserver;

class MainFormViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<SensorObserver*> sensorObservers READ sensorObservers WRITE setSensorObservers NOTIFY
                   sensorObserversChanged)
    Q_PROPERTY(QList<QObject*> sensors READ sensors NOTIFY sensorsChanged)

public:
    explicit MainFormViewModel(QObject * parent = nullptr);

    QList<SensorObserver *> sensorObservers() const;
    void setSensorObservers(const QList<SensorObserver *> & value);

    QList<QObject *> sensors() const;

signals:
    void sensorObserversChanged(QList<SensorObserver *>);
    void sensorsChanged(QList<QObject *>);

public slots:

private slots:
    void onRemoteObjectAdded(const QRemoteObjectSourceLocation & entry);
    void onRemoteObjectRemoved(const QRemoteObjectSourceLocation & entry);
    void onRegistryInitialized();

private:
    SensorObserver * createObserver(const QString & hostName, const QString objectName) const;
    void sortSensors();
    void addToObservers(SensorObserver *observer);
    void addToNotInitialized(SensorObserver *observer);

private:
    QUrl m_urlRegistry;
    QSharedPointer<QRemoteObjectNode> m_clientNode;
    QList<SensorObserver *> m_sensorObservers;
    QHash<QString, QHash<QString, SensorObserver *>> m_sensorObserverRegistry;
    QList<SensorObserver *> m_notInisizlizedSensors;
    QList<QObject *> m_sensors;
};

#endif  // MAINFORMVIEWMODEL_H
