#ifndef RECEIVEMODSTATE_H
#define RECEIVEMODSTATE_H

#include "write_result_state.cpp"
#include "write_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../views/icon_text_view.cpp"

namespace Murli
{
    class ReceiveModState : public State
    {
        public:
            ReceiveModState(const uint16_t modSize) : _modSize(modSize)
            {
                _writeModView = std::make_shared<IconTextView>("Receiving MOD ...", u8g2_font_open_iconic_embedded_2x_t, 69);
            }

            void run(MurliContext& context)
            {
                context.getDisplay().setView(_writeModView);
                context.getDisplay().loop();

                if(_receivedMod.size() != _modSize && Serial.available())
                {
                    while(Serial.available() > 0)
                    {
                        _receivedMod.push_back(Serial.read());
                    }
                    // ACK chunk
                    if(_receivedMod.size() % 128 == 0 || _receivedMod.size() == _modSize)
                    {
                        Serial.write(30);
                    }
                }
                else if(_receivedMod.size() == _modSize)
                {
                    context.currentState = std::make_shared<WriteModState>(_receivedMod);
                }
                else
                {
                    Serial.write(31);
                    context.writeRequested = false;                    
                    context.currentState = std::make_shared<WriteResultState>(false, "Error receiving mod!");
                } 
            }
        
        private:
            uint16_t _modSize;
            std::vector<uint8_t> _receivedMod;
            std::shared_ptr<IconTextView> _writeModView;
    };
}

#endif // RECEIVEMODSTATE_H