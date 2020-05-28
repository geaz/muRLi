extern "C" {
    #include <tinyscript.h>
}
#include "script_context.hpp"

namespace Murli
{
    ScriptContext* ScriptContextPointer = nullptr;

    /* is static */ void ScriptContext::receiveModNameFunc(int c)
    {
        ScriptContextPointer->receiveModName((char)c);
    }

    /* is static */ void ScriptContext::setColorFrameFunc(uint8_t frame, uint32_t hexColor)
    {
        ScriptContextPointer->setColorFrame(frame, hexColor);
    }

    ScriptContext::ScriptContext(const std::string& mod) : _mod(mod)
    {
        ScriptContextPointer = this;

        if(TinyScript_Init(_scriptArena, sizeof(_scriptArena)) != 0)
        {
            _faulted = true;
            _message = "Init TS Error!";
            Serial.println("Init TS Error!");
        }

        TinyScript_SetOutput(ScriptContext::receiveModNameFunc);
        TinyScript_Define("sCF", CFUNC(2), (intptr_t)ScriptContext::setColorFrameFunc);
    }

    void ScriptContext::run(uint8_t vol, uint16_t frequency)
    {        
        if(!_faulted)
        {
            char volStr[3];
            char freqStr[10];
            char lastFreqStr[10];

            itoa(vol, volStr, 10);
            itoa(frequency, freqStr, 10);
            itoa(_lastFrequency, lastFreqStr, 10);

            std::string modWithParams = "var minF=130\nvar midF=1046\nvar maxF=3140";
            modWithParams = modWithParams + "\nvar v=" + volStr;
            modWithParams = modWithParams + "\nvar f=" + freqStr;
            modWithParams = modWithParams + "\nvar lf=" + lastFreqStr;
            modWithParams = modWithParams + "\n" + _mod;
            
            if(TinyScript_Run(modWithParams.c_str(), false, false) != 0)
            {
                _faulted = true;
                _message = "Script TS Error!";
                Serial.println("Script TS Error!");
            }
            _lastFrequency = frequency;
        }
    }

    void ScriptContext::receiveModName(char c)
    {
        if(c != '\n') _charBuffer.push_back(c);
        else
        {
            _modName = std::string(_charBuffer.data(), _charBuffer.size());
            _charBuffer.clear();
            _charBuffer.shrink_to_fit();
        }
    }

    void ScriptContext::setColorFrame(uint8_t frame, uint32_t hexColor)
    {
        if(frame >= 1 && frame <= 3)
        {
            _colors[frame] = Color::fromHex(hexColor);
        }
    }

    ColorFrame ScriptContext::getColorFrame() { return ColorFrame { _colors[0], _colors[1], _colors[2] }; }
    std::string ScriptContext::getModName() { return _modName; }
    bool ScriptContext::isFaulted() { return _faulted; }
}