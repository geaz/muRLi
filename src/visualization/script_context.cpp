extern "C" {
    #include <tinyscript.h>
}
#include "script_context.hpp"

namespace Murli
{
    ScriptContext* ScriptContextPointer = nullptr;

    /* is static */ void ScriptContext::receiveScriptOutput(int c)
    {
        Serial.print((char)c);
    }

    /* is static */ void ScriptContext::setColorFrameFunc(uint8_t frame, uint32_t hexColor)
    {
        ScriptContextPointer->setColorFrame(frame, hexColor);
    }

    ScriptContext::ScriptContext(const std::string mod) : _mod(mod)
    {
        ScriptContextPointer = this;

        if(TinyScript_Init(_scriptArena.begin(), sizeof(_scriptArena)) != 0)
        {
            _faulted = true;
            Serial.println("Init TS Error!");
        }

        TinyScript_SetOutput(ScriptContext::receiveScriptOutput);
        TinyScript_Define("sCF", CFUNC(2), (intptr_t)ScriptContext::setColorFrameFunc);

        // Run Once with random values to 'test' the loaded script
        run(50, 758);
    }

    ColorFrame ScriptContext::run(uint8_t volume, uint16_t frequency)
    {        
        _colors.fill(Color::fromHex(0x000000));
        if(!_faulted)
        {
            char volStr[10];
            char freqStr[20];
            char lastFreqStr[20];

            std::sprintf(volStr, "\nvar v=%i", volume);
            std::sprintf(freqStr, "\nvar f=%i", frequency);
            std::sprintf(lastFreqStr, "\nvar lf=%i", _lastFrequency);

            std::string modWithParams = "var minF=130\nvar midF=1046\nvar maxF=3140";
            modWithParams = modWithParams + volStr;
            modWithParams = modWithParams + freqStr;
            modWithParams = modWithParams + lastFreqStr;
            modWithParams = modWithParams + "\n" + _mod;
            
            if(TinyScript_Run(modWithParams.c_str(), false, false) != 0)
            {
                _faulted = true;
                Serial.println("Script TS Error!");
            }
            _lastFrequency = frequency;
        }
        return ColorFrame { _colors[0], _colors[1] };
    }

    void ScriptContext::setColorFrame(uint8_t frame, uint32_t hexColor)
    {
        if(frame >= 0 && frame <= 1)
        {
            _colors[frame] = Color::fromHex(hexColor);
        }
    }

    bool ScriptContext::isFaulted() { return _faulted; }
}