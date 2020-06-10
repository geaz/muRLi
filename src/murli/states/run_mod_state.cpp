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
                
                // Add a delay of 50ms (Lower would be possible too maybe). Otherwise the WiFi connection will be unstable!
                if(_lastUpdate + 50 < millis() && !_scriptContext->isFaulted())
                {
                    AnalyzerResult result = _frequencyAnalyzer.loop();
                    MurliCommand command = getCommand(context, result);
                    Color newColor = command.getNewNodeColor(context.getLed().getColor(), true, context.hasConnectedNodes());
                    setView(result);

                    if(_lastColor != newColor)
                    {
                        if(command.colorFrame.mode == Instant)
                        {
                            context.getDisplay().setRightStatus("INST " + newColor.toString());
                            context.getSocketServer().broadcast(command);
                            context.getSocketServer().loop();
                            context.getLed().setColor(newColor);
                        }
                        else
                        {                            
                            //context.getLed().setColorSequential(newColor);
                            context.getDisplay().setRightStatus("SEQ " + newColor.toString());
                            context.getSocketServer().broadcast(command);
                            context.getSocketServer().loop();
                        }
                        _lastColor = newColor;
                    }
                    _lastDB= result.decibel;
                    _lastUpdate = millis();
                }
                else if(_scriptContext->isFaulted())
                {
                    context.currentState = std::make_shared<InvalidModState>();
                }
            }

        private:
            MurliCommand getCommand(MurliContext& context, AnalyzerResult& result)
            {
                MurliCommand command;

                uint8_t volume = map(result.decibel < MinDB ? MinDB : result.decibel, MinDB, 0, 0, 100);
                // If no sound was registered during the last 'MinTimeSilence' seconds,
                // run the mod with parameters = 0
                if(volume == 0 && _lastTimeVolumeThreshold + MinTimeSilence < millis())
                {
                    command.colorFrame = _scriptContext->run(0, 0);
                    command.command = SET;
                }
                // Otherwise, if the volume is above the last registered volume
                // run the mod with the calculated parameters
                else if(volume > 0 && result.decibel > _lastDB)
                {
                    command.colorFrame = _scriptContext->run(volume, result.dominantFrequency);
                    command.command = SET;
                    _lastTimeVolumeThreshold = millis();
                }
                // Otherwise just FADE the current color
                else
                {
                    command.command = FADE;
                }

                return command;
            }

            void setView(AnalyzerResult& result)
            {
                _runModView->decibel = result.decibel;
                if(result.decibel > MinDB && result.dominantFrequency > 0)
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

            float _lastDB;
            Color _lastColor;
            uint64_t _lastUpdate = 0;
            uint64_t _lastTimeVolumeThreshold = 0; 
    };
}

#endif // RUNMODSTATE_H