#ifndef RUNMODSTATE_H
#define RUNMODSTATE_H

#include "../state.hpp"
#include "../murli_context.hpp"
#include "invalid_mod_state.cpp"
#include "../../display/views/run_mod_view.cpp"
#include "../../visualization/script_context.hpp"
#include "../../visualization/frequency_analyzer.hpp"

namespace Murli
{
    static const int32_t MinTimeSilence = 5000;

    class RunModState : public State
    {
        public:
            RunModState(std::shared_ptr<ScriptContext> scriptContext) : _scriptContext(scriptContext)
            {
                _runModView = std::make_shared<RunModView>();
            }

            void run(MurliContext& context)
            {                         
                context.getDisplay().setView(_runModView);
                if(_scriptContext->isFaulted())
                {
                    context.currentState = std::make_shared<InvalidModState>();
                    return;
                }

                AnalyzerResult result = _frequencyAnalyzer.loop();
                setView(result);

                _scriptContext->updateAnalyzerResult(result);
                _scriptContext->run(0, context.getMeshLedCount(), millis() - _lastLedUpdate);
                _lastLedUpdate = millis();
            }

        private:
            void setView(AnalyzerResult& result)
            {
                _runModView->decibel = result.decibel;
                if(result.volume > 0 && result.dominantFrequency > 0)
                {
                    _runModView->dominantFrequency = result.dominantFrequency;
                    _runModView->frequencyRange = _frequencyAnalyzer.getFrequencyRange(result, 17, 9);
                }
                else
                {
                    _runModView->dominantFrequency = 0;
                    _runModView->fadeFrequencyRange();
                }
            }

            FrequencyAnalyzer _frequencyAnalyzer;
            std::shared_ptr<RunModView> _runModView;
            std::shared_ptr<ScriptContext> _scriptContext;
            uint64_t _lastLedUpdate = millis();
    };
}

#endif // RUNMODSTATE_H