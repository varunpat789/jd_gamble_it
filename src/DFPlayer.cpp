#include "DFPlayer.h"

DFPlayer::DFPlayer(HardwareSerial &serialPort, uint8_t rxPin, uint8_t txPin)
{
    this->serial = &serialPort;
    this->rxPin = rxPin;
    this->txPin = txPin;
    this->currentVolume = 20;
}

void DFPlayer::begin(int volume)
{
    // Initialize hardware serial with custom pins
    serial->begin(9600, SERIAL_8N1, rxPin, txPin);
    delay(1000); // Wait for DFPlayer to initialize
    
    currentVolume = volume;
}

void DFPlayer::execute_CMD(byte CMD, byte Par1, byte Par2)
{
    // Calculate the checksum (2 bytes)
    word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
    
    // Build the command line
    byte Command_line[10] = { 
        Start_Byte, 
        Version_Byte, 
        Command_Length, 
        CMD, 
        Acknowledge,
        Par1, 
        Par2, 
        highByte(checksum), 
        lowByte(checksum), 
        End_Byte
    };

    // Send the command line to the module
    for (byte k = 0; k < 10; k++)
    {
        serial->write(Command_line[k]);
    }
}

void DFPlayer::playFirst()
{
    execute_CMD(0x3F, 0, 0);
    delay(2000);
    setVolume(20);
    delay(500);
    execute_CMD(0x11, 0, 1);
    delay(500);
}

void DFPlayer::pausing()
{
    execute_CMD(0x0E, 0, 0);
    delay(500);
}

void DFPlayer::play()
{
    execute_CMD(0x0D, 0, 1);
    delay(500);
}

void DFPlayer::playNext()
{
    execute_CMD(0x01, 0, 1);
    delay(500);
}

void DFPlayer::playPrevious()
{
    execute_CMD(0x02, 0, 1);
    delay(500);
}

void DFPlayer::setVolume(int volume)
{
    execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
    delay(2000);
}