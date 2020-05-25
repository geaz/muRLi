#include "rom_24LC32A.hpp"

namespace Murli
{
    Rom24LC32A::Rom24LC32A(const uint8_t deviceAddress) : _deviceAddress(deviceAddress) {}

    int Rom24LC32A::clear()
    {
        // Allocate the Array on the heap
        // Stack allocation of this size would overrun it (forces reset)
        uint8_t* zeroData = new uint8_t[ModMemorySize];
        memset(zeroData, 0, ModMemorySize);
        int result = write(zeroData, ModMemorySize);
        delete zeroData;

        return result;
    }

    int Rom24LC32A::write(const uint8_t* buffer, unsigned short length)
    {
        char transmissionStatus = 0;
        unsigned short pagesToWrite = ceil(length / (double)_pageSize);
        for(char i = 0; i < pagesToWrite; i++)
        {
            waitReady();

            // Start transmission on rom address (i * _pageSize)
            // First Page = 0
            // Second Page = 32 etc.
            Wire.beginTransmission(_deviceAddress);
            Wire.write((byte)((i * _pageSize) >> 8));
            Wire.write((byte)((i * _pageSize) & 0xFF));

            for(char j = 0; j < _pageSize && (i * _pageSize) + j < length; j++)
            {
                Wire.write(buffer[(i * _pageSize) + j]);
            }
            transmissionStatus = Wire.endTransmission();
            if(transmissionStatus != 0) break;
        }
        return transmissionStatus;
    }

    int Rom24LC32A::read(uint8_t* buffer, unsigned short length)
    {
        waitReady();

        uint8_t transmissionStatus = 0;
        unsigned short pagesToRead = ceil(length / (double)_pageSize);
        
        for(unsigned short i = 0; i < pagesToRead; i++)
        {
            Wire.beginTransmission(_deviceAddress);
            Wire.write((byte)((i * _pageSize) >> 8));
            Wire.write((byte)((i * _pageSize) & 0xFF));
            transmissionStatus = Wire.endTransmission();
            if(transmissionStatus != 0) break;

            uint8_t byteCount = Wire.requestFrom(_deviceAddress, _pageSize);
            if(Wire.available() != byteCount || byteCount == 0)
            {
                transmissionStatus = -1;
                break;
            }
            
            for(char j = 0; j < byteCount && (i * _pageSize) + j < length; j++)
            {
                buffer[(i * _pageSize) + j] = Wire.read();
            }            
        }
        
        return transmissionStatus;
    }

    void Rom24LC32A::waitReady() const
    {
        char transmissionStatus = -1;
        while(transmissionStatus != 0)
        {
            Wire.beginTransmission(_deviceAddress);
            transmissionStatus = Wire.endTransmission();
        }
    }
}