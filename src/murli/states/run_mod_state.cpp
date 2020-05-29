#ifndef RUNMODSTATE_H
#define RUNMODSTATE_H

#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/run_mod_view.cpp"
#include "../../visualization/script_context.hpp"
#include "../../visualization/frequency_analyzer.hpp"

namespace Murli
{
    class RunModState : public State
    {
        public:
            RunModState(std::shared_ptr<ScriptContext> scriptContext) : _scriptContext(scriptContext)
            {
                _runModView = std::make_shared<RunModView>();
                _runModView->modName = _scriptContext->getModName();
            }

            void run(MurliContext& context)
            {                         
                context.getLed().setColor(Murli::Yellow);
                context.getDisplay().setView(_runModView);
                
                // Add a delay of 50ms (Lower would be possible too maybe). Otherwise the WiFi connection will be unstable!
                if(_lastUpdate + 50 < millis())
                {
                    _frequencyAnalyzer.loop();
                    _runModView->volume = _frequencyAnalyzer.getVolume();
                    if(_frequencyAnalyzer.getVolume() > 0)
                    {
                        _runModView->dominantFrequency = _frequencyAnalyzer.getDominantFrequency();
                        _runModView->frequencyRange = _frequencyAnalyzer.getFrequencyRange(17, 9);
                    }
                    else
                    {
                        _runModView->fadeFrequencyRange();
                        _runModView->dominantFrequency = 0;
                    }                                    
                }
            }

        private:
            FrequencyAnalyzer _frequencyAnalyzer;
            std::shared_ptr<RunModView> _runModView;
            std::shared_ptr<ScriptContext> _scriptContext;

            uint64_t _lastUpdate = 0;
    };
}

#endif // RUNMODSTATE_H