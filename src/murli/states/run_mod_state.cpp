#ifndef RUNMODSTATE_H
#define RUNMODSTATE_H

#include "../state.hpp"
#include "../murli_context.hpp"
#include "invalid_mod_state.cpp"
#include "../../display/run_mod_view.cpp"
#include "../../visualization/script_context.hpp"
#include "../../visualization/frequency_analyzer.hpp"

namespace Murli
{
    static const int8_t MinDB = -20;

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
                
                // Add a delay of 50ms (Lower would be possible too maybe). Otherwise the WiFi connection will be unstable!
                if(_lastUpdate + 50 < millis() && !_scriptContext->isFaulted())
                {
                    AnalyzerResult result = _frequencyAnalyzer.loop();
                    _runModView->decibel = result.decibel;
                    if(result.decibel > MinDB)
                    {
                        _runModView->dominantFrequency = result.dominantFrequency;
                        _runModView->frequencyRange = _frequencyAnalyzer.getFrequencyRange(result, 17, 9);

                        uint8_t volume = map(result.decibel, MinDB, 0, 0, 100);
                        ColorFrame colorFrame = _scriptContext->run(volume, result.dominantFrequency);

                        Color newColor = colorFrame.first;
                        String serverMessage = "set " + String(newColor.Red) + "," + String(newColor.Green) + "," + String(newColor.Blue);

                        context.getDisplay().setRightStatus(serverMessage.c_str());
                        context.getSocketServer().broadcast(serverMessage);
                        context.getSocketServer().loop();
                        context.getLed().setColor(newColor);
                    }
                    else
                    {
                        _runModView->fadeFrequencyRange();
                        _runModView->dominantFrequency = 0;
                    }
                    _lastUpdate = millis();
                }
                else if(_scriptContext->isFaulted())
                {
                    context.currentState = std::make_shared<InvalidModState>();
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