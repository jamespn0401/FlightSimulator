#ifndef SENSOR_HPP
#define SENSOR_HPP

struct SensorData
{
    double altitude;
    double velocity;
    double acceleration;
    const double gravity = 9.81; // m/s^2
};

#endif // SENSOR_HPP