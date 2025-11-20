// CODE BASED FROM https://www.instructables.com/MP3-Player-With-Arduino-Using-DF-Player-Mini/
#include "speaker.h"

HardwareSerial MP3Serial(2);

Speaker::Speaker(uint8_t _tx_pin, uint8_t _rx_pin) : tx_pin(_tx_pin), rx_pin(_rx_pin), volume(20)
{
    MP3Serial.begin(9600, SERIAL_8N1, rx_pin, tx_pin);
    delay(1000);
}

void Speaker::execute_CMD(byte CMD, byte par1, byte par2)
{
    // Calculate the checksum (2 bytes)
    word checksum = -(version_byte + command_length + CMD + acknowledge + par1 + par2);

    // Build the command line
    byte command_line[10] = {
        start_byte,
        version_byte,
        command_length,
        CMD,
        acknowledge,
        par1,
        par2,
        highByte(checksum),
        lowByte(checksum),
        end_byte};

    // Send the command line to the module
    for (byte k = 0; k < 10; k++)
    {
        MP3Serial.write(command_line[k]);
    }
}

void Speaker::play_first()
{
    execute_CMD(0x3F, 0, 0);
    delay(2000);
    set_volume(20);
    delay(500);
    execute_CMD(0x11, 0, 1);
    delay(500);
}

void Speaker::pause()
{
    execute_CMD(0x0E, 0, 0);
    delay(500);
}

void Speaker::play()
{
    execute_CMD(0x0D, 0, 1);
    delay(500);
}

void Speaker::play_next()
{
    execute_CMD(0x01, 0, 1);
    delay(500);
}

void Speaker::play_previous()
{
    execute_CMD(0x02, 0, 1);
    delay(500);
}

void Speaker::set_volume(int _volume)
{
    execute_CMD(0x06, 0, _volume);
    delay(2000);
}