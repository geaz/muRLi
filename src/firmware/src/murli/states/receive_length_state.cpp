#ifndef RECEIVELENGTHSTATE_H
#define RECEIVELENGTHSTATE_H

#include "write_result_state.cpp"
#include "receive_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/views/icon_text_view.cpp"

namespace Murli
{
    class ReceiveLengthState : public State
    {
        public:
            ReceiveLengthState()
            {
                _writeModView = std::make_shared<IconTextView>("Receiving MOD ...", u8g2_font_open_iconic_embedded_2x_t, 69);
            }

            void run(MurliContext& context)
            {          
                context.getLed().setAllLeds(Murli::Cyan);
                context.getDisplay().setView(_writeModView);
                context.getDisplay().loop();
                                
                std::array<byte, sizeof(int)> buffer = { { 0 } };
                Serial.readBytes(buffer.data(), sizeof(int));

                uint16_t length = 0;
                length  = buffer[3] << 24;
                length += buffer[2] << 16;
                length += buffer[1] << 8;
                length += buffer[0];

                if(length <= ModMemorySize)
                {
                    Serial.write(30);
                    context.currentState = std::make_shared<ReceiveModState>(length);
                }
                else
                {
                    Serial.write(31);
                    context.writeRequested = false;
                    context.currentState = std::make_shared<WriteResultState>(false, "MOD too big!");
                }
            }
        
        private:
            std::shared_ptr<IconTextView> _writeModView;
    };
}

#endif // RECEIVELENGTHSTATE_H