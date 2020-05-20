#ifndef MODCONTROL_H
#define MODCONTROL_H

#include <Wire.h>
#include <Arduino.h>

#include "rom_24LC32A.hpp"

namespace Murli
{
    class MODControl
    {
        public:
            MODControl();

            void loop();
            bool inWriteMode() const;

            bool isValidMod() const;            
            bool modInserted() const;
            const char* modName() const;
            const char* modContent() const;

            void sendSerialAck() const;

        private:        
            Rom24LC32A rom;

            char _modContent[ModMemorySize];            
            bool _readError, _readMod = false;

            char _receivedMod[ModMemorySize];
            bool _isReceivingMod, _modReceived, _writeMod = false;
    };
}

#endif // MODCONTROL_H