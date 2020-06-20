#ifndef JSCONTEXT_H
#define JSCONTEXT_H

#include <mjs.h>
#include "../led/led.hpp"
#include "frequency_analyzer.hpp"

namespace Murli
{
    static const char* MjsMinFrequency = "minF";
    static const char* MjsMidFrequency = "midF";
    static const char* MjsMaxFrequency = "maxF";
    static const char* MjsPreviosLedCount = "pLedC";
    static const char* MjsNodeLedCount = "nLedC";
    static const char* MjsMeshLedCount = "mLedC";
    static const char* MjsLastVolume = "lVol";
    static const char* MjsLastFrequency = "lFreq";
    static const char* MjsVolume = "vol";
    static const char* MjsFrequency = "freq";

    class ScriptContext
    {
        public:
            ScriptContext(LED& led, const std::string& mod);
            ~ScriptContext();
            
            void updateLedInfo(const uint16_t previousLedCount, const uint16_t meshLedCount);
            void updateAnalyzerResult(const uint8_t volume, const uint16_t dominantFrequency);
            void run(const uint8_t delta);

            bool isFaulted();
            void setLed(uint32_t index, Color color);

        private:
            void saveJsExec(const char* script, const char* errMessage);
            void saveJsSet(const char* mjsVarName, mjs_val_t value, const char* errMessage);
            void saveJsGet(const char* mjsVarName, mjs_val_t& saveToVal, const char* errMessage);

            mjs* _mjs;
            mjs_val_t _mjsGlobal;
            mjs_val_t _updateFunc;
            uint8_t _lastVolume = 0;
            uint16_t _lastFrequency = 0;
            bool _faulted = false;

            LED& _led;
    };

    // Pointer for some mJS Callbacks
    extern ScriptContext* ScriptContextPointer;
}

#endif // JSCONTEXT_H