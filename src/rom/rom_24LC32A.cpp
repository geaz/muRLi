#include "rom_24LC32A.hpp"

namespace Murli
{
    Rom24LC32A::Rom24LC32A(const uint8_t deviceAddress) : _deviceAddress(deviceAddress) {}

    uint8_t Rom24LC32A::clear()
    {
        std::vector<uint8_t> zeroData(ModMemorySize, 0);
        return write(zeroData);
    }

    uint8_t Rom24LC32A::write(const std::vector<uint8_t>& data)
    {
        uint8_t transmissionStatus = 0;
        uint16_t pagesToWrite = ceil(data.size() / (double)_pageSize);
        for(uint8_t i = 0; i < pagesToWrite; i++)
        {
            waitReady();

            // Start transmission on rom address (i * _pageSize)
            // First Page = 0
            // Second Page = 32 etc.
            Wire.beginTransmission(_deviceAddress);
            Wire.write((i * _pageSize) >> 8);
            Wire.write((i * _pageSize) & 0xFF);

            for(uint8_t j = 0; j < _pageSize && (i * _pageSize) + j < data.size(); j++)
            {
                Wire.write(data[(i * _pageSize) + j]);
            }
            transmissionStatus = Wire.endTransmission();
            if(transmissionStatus != 0) break;
        }
        return transmissionStatus;
    }

    uint8_t Rom24LC32A::read(std::vector<uint8_t>& data, const uint16_t length)
    {
        waitReady();

        uint8_t transmissionStatus = 0;
        uint16_t pagesToRead = ceil(length / (double)_pageSize);
        
        for(uint16_t i = 0; i < pagesToRead; i++)
        {
            Wire.beginTransmission(_deviceAddress);
            Wire.write((i * _pageSize) >> 8);
            Wire.write((i * _pageSize) & 0xFF);
            transmissionStatus = Wire.endTransmission();
            if(transmissionStatus != 0) break;

            uint8_t byteCount = Wire.requestFrom(_deviceAddress, _pageSize);
            if(Wire.available() != byteCount || byteCount == 0)
            {
                transmissionStatus = -1;
                break;
            }
            
            for(uint8_t j = 0; j < byteCount && (i * _pageSize) + j < length; j++)
            {
                data.push_back(Wire.read());
            }            
        }
        
        return transmissionStatus;
    }

    void Rom24LC32A::waitReady() const
    {
        uint8_t transmissionStatus = -1;
        while(transmissionStatus != 0)
        {
            Wire.beginTransmission(_deviceAddress);
            transmissionStatus = Wire.endTransmission();
        }
    }
}