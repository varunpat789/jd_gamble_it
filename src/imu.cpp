#include "imu.h"

IMU::IMU(float shake_threshold, unsigned long debounce)
{
    this->shake_threshold = shake_threshold;
    this->debouce_interval = debounce;
    this->prev_accel_magnitude = 0;
    this->last_shake_time = 0;

    accel_x = 0;
    accel_y = 0;
    accel_z = 0;
}

bool IMU::begin()
{
    Wire.begin();
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    byte error = Wire.endTransmission(true);

    if (error == 0)
    {
        update();
        prev_accel_magnitude = getAccelMagnitude();
        return true;
    }
    return false;
}

void IMU::update()
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14, true);

    accel_x = Wire.read() << 8 | Wire.read();
    accel_y = Wire.read() << 8 | Wire.read();
    accel_z = Wire.read() << 8 | Wire.read();
}

float IMU::get_accel_mag()
{
    return sqrt(
        (float)accel_x * accel_x +
        (float)accel_y * accel_y +
        (float)accel_z * accel_z);
}

bool IMU::is_shaken()
{
    unsigned long current_time = millis();

    // debounce multiple shakes
    if (current_time - last_shake_time < debouce_interval)
    {
        return false;
    }

    // get motion
    float current_magnitude = get_accel_mag();
    float magnitude_change = abs(current_magnitude - prev_accel_magnitude);

    prev_accel_magnitude = current_magnitude;

    // Check for shake if enough motion
    if (magnitude_change > shake_threshold)
    {
        last_shake_time = current_time;
        return true;
    }

    return false;
}

void IMU::log()
{
    Serial.println("a_x = " + String(accel_x) + ", a_y = " + String(accel_y) + ", a_z = " + String(accel_z));
}