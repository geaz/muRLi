#ifndef JSCONTEXT_H
#define JSCONTEXT_H

#include <tuple>
#include <deque>
#include <mjs.h>
#include "../led/led.hpp"
#include "frequency_analyzer.hpp"

namespace Murli
{
    class ScriptContext
    {
        public:
            ScriptContext(LED& led, const std::string& mod);
            ~ScriptContext();
            
            void updateLedInfo(const uint16_t previousNodeCount, const uint32_t previousLedCount, const uint32_t meshLedCount);
            void init();
            std::string getModName();
            void updateAnalyzerResult(const uint8_t volume, const uint16_t dominantFrequency);
            void run(const uint8_t delta);

            bool isFaulted();
            void setDelay(uint32_t delay);

            LED& getLed();

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