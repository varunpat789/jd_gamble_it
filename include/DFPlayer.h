#ifndef DFPLAYER_H
#define DFPLAYER_H

#include <Arduino.h>
#include <HardwareSerial.h>

class DFPlayer
{
private:
    HardwareSerial *serial;
    uint8_t rxPin;
    uint8_t txPin;
    
    static const uint8_t Start_Byte = 0x7E;
    static const uint8_t Version_Byte = 0xFF;
    static const uint8_t Command_Length = 0x06;
    static const uint8_t End_Byte = 0xEF;
    static const uint8_t Acknowledge = 0x00;
    
    int currentVolume;

public:
    // Constructor - uses hardware serial (UART1 or UART2)
    DFPlayer(HardwareSerial &serialPort, uint8_t rxPin, uint8_t txPin);
    
    // Initialize the DFPlayer
    void begin(int volume = 20);
    
    // Original playback controls from your code
    void playFirst();
    void pausing();
    void play();
    void playNext();
    void playPrevious();
    void setVolume(int volume);
    
private:
    void execute_CMD(byte CMD, byte Par1, byte Par2);
};

#endif // DFPLAYER_H