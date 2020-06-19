#ifndef JSCONTEXT_H
#define JSCONTEXT_H

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
            
            void updateAnalyzerResult(const uint8_t volume, const uint16_t dominantFrequency, const uint8_t delta);
            void run(const uint16_t previousLedCount, const uint16_t meshLedCount);

            bool isFaulted();

        private:
            void saveJsExec(const char* script, const char* errMessage);
            void saveJsGetFunc(mjs_val_t& saveToVal, const char* funcName, const char* errMessage);

            mjs* _mjs;
            mjs_val_t _updateMeshLedCountFunc;
            mjs_val_t _updateAnalyzerValuesFunc;
            mjs_val_t _onAnalyzerUpdateFunc;
            mjs_val_t _updateLed;
            bool _faulted = false;

            LED& _led;
    };
}

#endif // JSCONTEXT_H