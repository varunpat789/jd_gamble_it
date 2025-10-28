#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include "Wire.h"

class IMU
{
private:
    static const int MPU_ADDR = 0x68;

    int16_t accel_x, accel_y, accel_z;

    float prev_accel_magnitude;
    float shake_threshold;
    long last_shake_time;
    unsigned long debouce_interval;

    float get_accel_mag();

public:
    IMU(float shake_threshold, unsigned long debounce);

    bool begin();
    void update();
    bool is_shaken();
    void log();
};

#endif // IMU_H