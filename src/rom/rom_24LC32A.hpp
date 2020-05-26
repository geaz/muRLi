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
            uint8_t read(uint8_t* buffer, const uint16_t length);
            uint8_t write(const uint8_t* buffer, const uint16_t length);

        private:
            void waitReady() const;

        private:
            uint8_t _pageSize = 32;
            uint8_t _deviceAddress;
    };
}

#endif // EEPROM_H