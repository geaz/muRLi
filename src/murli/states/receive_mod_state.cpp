#ifndef RECEIVEMODSTATE_H
#define RECEIVEMODSTATE_H

#include "no_mod_state.cpp"
#include "write_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/centeredTextView.cpp"

namespace Murli
{
    class ReceiveModState : public State
    {
        public:
            ReceiveModState(uint16_t modSize) : _modSize(modSize)
            {
                _centeredTextView = std::make_shared<CenteredTextView>();
                _centeredTextView->setText("Receiving MOD ...");
            }

            void run(MurliContext& context)
            {
                if(_receivingIndex != _modSize && Serial.available())
                {
                    while(Serial.available() > 0)
                    {
                        _receivedMod[_receivingIndex++] = Serial.read();
                    }
                    // ACK chunk
                    if(_receivingIndex % 128 == 0 || _receivingIndex == _modSize)
                    {
                        Serial.write(30);
                    }
                }
                else if(_receivingIndex == _modSize)
                {
                    context.setState(std::make_shared<WriteModState>(_receivedMod, _modSize));
                }
                else
                {
                    Serial.write(31);
                    /*context.writeRequested = false;

                    _centeredTextView.setText("Error receiving mod (M)!");
                    context.display.setView(&_centeredTextView);
                    context.led.blink(Murli::Red);*/
                } 
            }
        
        private:
            uint16_t _modSize = 0;
            uint16_t _receivingIndex = 0;
            uint8_t _receivedMod[ModMemorySize] = { 0 };
            std::shared_ptr<CenteredTextView> _centeredTextView;
    };
}

#endif // RECEIVEMODSTATE_H