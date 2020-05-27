#ifndef WRITEMODSTATE_H
#define WRITEMODSTATE_H

#include "no_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/centeredTextView.cpp"

namespace Murli
{
    class WriteModState : public State
    {
        public:
            WriteModState(uint8_t* receivedMod, uint16_t modSize) : 
                _receivedMod(receivedMod),
                 _modSize(modSize)
            { }

            void run(MurliContext& context)
            {      
                int clearResult = context.getRom().clear();
                int result = context.getRom().write(_receivedMod, _modSize);
                if(clearResult == 0 && result == 0)
                {
                    Serial.write(30);

                   /* _centeredTextView->setText("MOD saved!");
                    context.getDisplay().setView(&_centeredTextView);
                    context.getLed().blink(Murli::Green);*/
                }
                else
                {
                    Serial.write(31);

                   /* _centeredTextView.setText("Error during write!");
                    context.display.setView(&_centeredTextView);
                    context.led.blink(Murli::Red);*/
                }
                context.writeRequested = false;
                context.currentState = std::make_shared<NoModState>();
            }
        
        private:
            uint8_t* _receivedMod;
            uint16_t _modSize;
            std::shared_ptr<CenteredTextView> _centeredTextView;
    };
}

#endif // WRITEMODSTATE_H