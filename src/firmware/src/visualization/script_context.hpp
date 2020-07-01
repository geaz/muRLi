#ifndef JSCONTEXT_H
#define JSCONTEXT_H

#include <tuple>
#include <deque>
#include <mjs.h>

#include "frequency_analyzer.hpp"
#include "../hardware/led/led.hpp"
#include "../network/websocket/commands/client_commands.hpp"

namespace Murli
{
    class ScriptContext
    {
        public:
            ScriptContext(LED& led, const std::string& mod);
            ~ScriptContext();
            
            void init();
            void run(const uint8_t delta);
            void updateLedInfo(const uint16_t previousNodeCount, const uint32_t previousLedCount, const uint32_t meshLedCount);
            void updateAnalyzerResult(const uint8_t volume, const uint16_t dominantFrequency);

            bool isFaulted();
            void setDelay(uint32_t delay);

            std::string getModName();
            LED& getLed();
            
            Client::AnalyzerSource source = Client::AnalyzerSource::Microphone;

        private:
            void saveJsExec(const char* script, const char* errMessage);
            void saveJsSet(const char* mjsVarName, mjs_val_t value, const char* errMessage);
            void saveJsGet(const char* mjsVarName, mjs_val_t& saveToVal, const char* errMessage);

            mjs* _mjs;
            mjs_val_t _mjsGlobal;
            mjs_val_t _updateFunc;
            mjs_val_t _initFunc;
            mjs_val_t _getNameFunc;
            bool _faulted = false;

            uint8_t _lastVolume = 0;
            uint16_t _lastFrequency = 0;

            uint32_t _delay = 0;
            std::deque<std::tuple<uint8_t, uint16_t>> _resultDeque;

            LED& _led;
    };

    // Pointer for some mJS Callbacks
    extern ScriptContext* ScriptContextPointer;
}

#endif // JSCONTEXT_H