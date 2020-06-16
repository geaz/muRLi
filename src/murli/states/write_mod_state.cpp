#ifndef WRITEMODSTATE_H
#define WRITEMODSTATE_H

#include "write_result_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/views/icon_text_view.cpp"

namespace Murli
{
    class WriteModState : public State
    {
        public:
            WriteModState(const std::vector<uint8_t> receivedMod) : _receivedMod(receivedMod)
            { 
                _writeModView = std::make_shared<IconTextView>("Writing MOD ...", u8g2_font_open_iconic_embedded_2x_t, 69);
            }

            void run(MurliContext& context)
            {      
                context.getDisplay().setView(_writeModView);
                context.getDisplay().loop();

                int clearResult = context.getRom().clear();
                int result = context.getRom().write(_receivedMod);

                if(clearResult == 0 && result == 0)
                {
                    Serial.write(30);
                    context.currentState = std::make_shared<WriteResultState>(true, "MOD saved!");
                }
                else
                {
                    Serial.write(31);
                    context.currentState = std::make_shared<WriteResultState>(false, "Error during write!");
                }
                context.writeRequested = false;
            }
        
        private:
            const std::vector<uint8_t> _receivedMod;
            std::shared_ptr<IconTextView> _writeModView;
    };
}

#endif // WRITEMODSTATE_H