// CODE BASED FROM https://www.instructables.com/MP3-Player-With-Arduino-Using-DF-Player-Mini/

#ifndef SPEAKER_H
#define SPEAKER_H

#include <Arduino.h>
#include <HardwareSerial.h>

class Speaker
{
private:
    uint8_t tx_pin;
    uint8_t rx_pin;

    static const uint8_t start_byte = 0x7E;
    static const uint8_t version_byte = 0xFF;
    static const uint8_t command_length = 0x06;
    static const uint8_t end_byte = 0xEF;
    static const uint8_t acknowledge = 0x00;

    int volume;

public:
    Speaker(uint8_t _tx_pin, uint8_t _rx_pin);

    void play_first();
    void pause();
    void play();
    void play_next();
    void play_previous();
    void set_volume(int _volume);

private:
    void execute_CMD(byte CMD, byte Par1, byte Par2);
};

#endif // SPEAKER_H