#ifndef EEPROM_H
#define EEPROM_H

#include <Wire.h>
#include <Arduino.h>

namespace Murli
{
    const uint16_t ModMemorySize = 0xFA0; // 4000 Byte

    class Rom24LC32A
    {
        public:
            Rom24LC32A(const uint8_t deviceAddress);
            uint8_t clear();
            uint8_t read(std::vector<uint8_t>& data, const uint16_t length);
            uint8_t write(const std::vector<uint8_t>& data);

        private:
            void waitReady() const;

        private:
            uint8_t _pageSize = 32;
            uint8_t _deviceAddress;
    };
}

#endif // EEPROM_H