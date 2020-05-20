#include "mod_control.hpp"

namespace Murli
{
    MODControl::MODControl() : rom(0x50) {}

    void MODControl::loop()
    {
        if(modInserted() && !_readMod && !_readError)
        {
            uint8_t result = rom.read((uint8_t*)_modContent, ModMemorySize);

            if(result == 0)
            {
                _readMod = true;
                Serial.println(_modContent);
            }
            else
            {
                _readError = true;
            }
        }
        else if (modInserted() && Serial.available() > 0)
        {
            if(_isReceivingMod && !_modReceived)
            {
                Serial.readBytesUntil('0', _receivedMod, 4000);
                _modReceived = true;

                sendSerialAck();
            }
            else
            {
                int incomingByte = Serial.read();
                if(incomingByte == 30)
                {
                    _isReceivingMod = true;
                    sendSerialAck();
                }
                else if(incomingByte == 31)
                {
                    _isReceivingMod = false;
                    _writeMod = true;                    
                    sendSerialAck();
                }
            }
        }
        else if(modInserted() && _writeMod)
        {
            rom.clear();
            Serial.write(rom.write((uint8_t*)_receivedMod, 3));

            _writeMod = false;
            _modReceived = false;

            _readError = false;
            _readMod = false;
        }
        else if(!modInserted())
        {
            _readError = false;
            _readMod = false;
        }
    }

    bool MODControl::inWriteMode() const
    {
        return _isReceivingMod || _writeMod;
    }

    bool MODControl::modInserted() const
    {
        return digitalRead(D6) == HIGH;
    }

    bool MODControl::isValidMod() const
    {
        return modInserted() && !_readError;
    }

    void MODControl::sendSerialAck() const
    {
        Serial.write(30);
    }
}
