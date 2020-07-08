#ifndef RUNMODSTATE_H
#define RUNMODSTATE_H

#include "../state.hpp"
#include "../murli_context.hpp"
#include "invalid_mod_state.cpp"
#include "../../views/run_mod_view.cpp"
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
                _modName = _scriptContext->getModName();
            }

            void run(MurliContext& context)
            {                         
                context.getDisplay().setView(_runModView);
                context.getDisplay().setLeftStatus(_modName);
                if(_scriptContext->isFaulted())
                {
                    context.currentState = std::make_shared<InvalidModState>();
                    return;
                }
                handleMicrophoneSource(context.getSocketServer());
            }

            void handleMicrophoneSource(SocketServer& socketServer)
            {
                if(_scriptContext->source == Client::AnalyzerSource::Microphone)
                {
                    AnalyzerResult result = _frequencyAnalyzer.loop();

                    Client::AnalyzerCommand analyzerCommand = { result.volume, result.dominantFrequency };                
                    Client::Command command = { millis(), Client::ANALYZER_UPDATE };
                    command.analyzerCommand = analyzerCommand;

                    socketServer.broadcast(command);
                    setView(result);

                    uint32_t delta = millis() - _lastLedUpdate;
                    _lastLedUpdate = millis();

                    _scriptContext->updateAnalyzerResult(result.volume, result.dominantFrequency);
                    _scriptContext->run(delta);
                }
            }

            void handleExternalSource(Server::Command command)
            {
                if(_scriptContext->source != Client::AnalyzerSource::Microphone
                && command.commandType == Server::CommandType::EXTERNAL_ANALYZER)
                {
                    
                }
            }

        private:
            void setView(AnalyzerResult& result)
            {
                _runModView->decibel = result.decibel;
                if(result.volume > 0 && result.dominantFrequency > 0)
                {
                    _runModView->dominantFrequency = result.dominantFrequency;
                    _runModView->frequencyRange = _frequencyAnalyzer.getFrequencyRange(result);
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
            std::string _modName;
    };
}

#endif // RUNMODSTATE_H