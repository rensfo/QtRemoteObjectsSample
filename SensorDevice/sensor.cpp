#include "sensor.h"

Sensor::Sensor(int id, QObject * parent)
    : SensorSource(parent)
    , m_id(id)
{}

int Sensor::id() const
{
    return m_id;
}

float Sensor::humidity() const
{
    return m_humidity;
}

float Sensor::temperature() const
{
    return m_temperature;
}

void Sensor::setHumidity(float humidity)
{
    m_humidity = humidity;
    emit humidityChanged(m_humidity);
}

void Sensor::setTemperature(float temperature)
{
    m_temperature = temperature;
    emit temperatureChanged(m_temperature);
}
