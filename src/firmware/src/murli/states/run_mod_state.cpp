#ifndef RUNMODSTATE_H
#define RUNMODSTATE_H

#include "../state.hpp"
#include "../murli_context.hpp"
#include "invalid_mod_state.cpp"
#include "../../shared.hpp"
#include "../../views/run_mod_view.cpp"
#include "../../visualization/script_context.hpp"
#include "../../visualization/frequency_analyzer.hpp"

namespace Murli
{
    class RunModState : public State
    {
        public:
            RunModState(MurliContext& context, std::shared_ptr<ScriptContext> scriptContext) 
                : _context(context), _scriptContext(scriptContext)
            {
                _handleId = context.getSocketServer()
                    .serverCommandEvents
                    .addEventHandler([this](Server::Command command) { handleExternalSource(command); });
                _runModView = std::make_shared<RunModView>();
                _modName = _scriptContext->getModName();
            }

            ~RunModState()
            {
                _context.getSocketServer()
                    .serverCommandEvents
                    .removeEventHandler(_handleId);
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
                    setView(
                        result.decibel, 
                        result.volume, 
                        result.dominantFrequency, 
                        Murli::getFrequencyBars(&result.fftReal[0], result.volume));

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
                    Server::ExternalAnalyzerCommand externalCommand = command.externalAnalyzerCommand;
                    Client::AnalyzerCommand analyzerCommand = { externalCommand.volume, externalCommand.frequency };                
                    Client::Command command = { millis(), Client::ANALYZER_UPDATE };
                    command.analyzerCommand = analyzerCommand;

                    _context.getSocketServer().broadcast(command);
                    setView(
                        externalCommand.decibel, 
                        externalCommand.volume, 
                        externalCommand.frequency,
                        externalCommand.buckets);

                    uint32_t delta = millis() - _lastLedUpdate;
                    _lastLedUpdate = millis();

                    _scriptContext->updateAnalyzerResult(externalCommand.volume, externalCommand.frequency);
                    _scriptContext->run(delta);
                }
            }

        private:
            void setView(const float decibel, const uint16_t volume, const uint16_t dominantFrequency, const std::array<uint8_t, BAR_COUNT> buckets)
            {
                _runModView->decibel = decibel;
                if(volume > 0 && dominantFrequency > 0)
                {
                    _runModView->dominantFrequency = dominantFrequency;
                    _runModView->frequencyRange = buckets;
                }
                else
                {
                    _runModView->dominantFrequency = 0;
                    _runModView->fadeFrequencyRange();
                }
            }

            MurliContext& _context;
            std::shared_ptr<ScriptContext> _scriptContext;
            std::shared_ptr<RunModView> _runModView;
            uint64_t _handleId = 0;
            std::string _modName;

            FrequencyAnalyzer _frequencyAnalyzer;
            uint64_t _lastLedUpdate = millis();
    };
}

#endif // RUNMODSTATE_H