#ifndef WRITERESULTSTATE_H
#define WRITERESULTSTATE_H

#include "no_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/views/result_view.cpp"

namespace Murli
{
    class WriteResultState : public State
    {
        public:
            WriteResultState(bool success, const std::string& message) : _success(success)
            { 
                _resultView = std::make_shared<ResultView>();
                _resultView->setResult(success);
                _resultView->setText(message);
            }

            void run(MurliContext& context)
            {      
                context.getDisplay().setView(_resultView);
                
                if(_success) context.getLed().setColor(Murli::Green);
                else context.getLed().setColor(Murli::Red);

                if(_started + 2000 < millis())
                {
                    context.currentState = std::make_shared<NoModState>();
                }
            }
        
        private:
            boolean _success;
            uint64_t _started = millis();       
            std::shared_ptr<ResultView> _resultView;
    };
}

#endif // WRITERESULTSTATE_H