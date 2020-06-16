#ifndef WRITERESULTSTATE_H
#define WRITERESULTSTATE_H

#include "no_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/views/icon_text_view.cpp"

namespace Murli
{
    class WriteResultState : public State
    {
        public:
            WriteResultState(bool success, std::string text) : _success(success)
            { 
                if(success)
                {
                    _resultView = std::make_shared<IconTextView>(text, u8g2_font_open_iconic_check_2x_t, 65);
                }
                else
                {
                    _resultView = std::make_shared<IconTextView>(text, u8g2_font_open_iconic_check_2x_t, 66);
                }
            }

            void run(MurliContext& context)
            {      
                context.getDisplay().setView(_resultView);
                
                if(_success) context.getLed().setAllLeds(Murli::Green);
                else context.getLed().setAllLeds(Murli::Red);

                if(_started + 2000 < millis())
                {
                    context.currentState = std::make_shared<NoModState>();
                }
            }
        
        private:
            boolean _success;
            uint32_t _started = millis();       
            std::shared_ptr<IconTextView> _resultView;
    };
}

#endif // WRITERESULTSTATE_H