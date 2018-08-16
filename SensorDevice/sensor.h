#ifndef SENSOR_H
#define SENSOR_H

#include "rep_sensor_source.h"

class Sensor : public SensorSource
{
    Q_OBJECT
public:
    explicit Sensor(int id, QObject * parent = nullptr);

public:
    virtual int id() const override;
    virtual float humidity() const override;
    virtual float temperature() const override;

    void setHumidity(float humidity);
    void setTemperature(float temperature);

private:
    int m_id;
    float m_humidity = 0.0;
    float m_temperature = 0.0;
};

#endif  // SENSOR_H
