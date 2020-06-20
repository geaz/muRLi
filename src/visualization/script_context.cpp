#include "script_context.hpp"
#include "mjs_funcs.hpp"

namespace Murli
{
    ScriptContext* ScriptContextPointer = nullptr;

    ScriptContext::ScriptContext(LED& led, const std::string& mod) : _led(led)
    {
        ScriptContextPointer = this;
        _mjs = mjs_create();
        _mjsGlobal = mjs_get_global(_mjs);

        saveJsSet("map", mjs_mk_foreign_func(_mjs, (mjs_func_ptr_t)mjsMap), "Set 'map' error!");
        saveJsSet("round", mjs_mk_foreign_func(_mjs, (mjs_func_ptr_t)mjsRound), "Set 'round' error!");
        saveJsSet("xrgb", mjs_mk_foreign_func(_mjs, (mjs_func_ptr_t)xrgb), "Set 'xrgb' error!");
        saveJsSet("xhsv", mjs_mk_foreign_func(_mjs, (mjs_func_ptr_t)xhsv), "Set 'xhsv' error!");
        saveJsSet("setLed", mjs_mk_foreign_func(_mjs, (mjs_func_ptr_t)mjsSetLed), "Set 'setLed' error!");

        saveJsSet(MjsMinFrequency, mjs_mk_number(_mjs, MinFrequency), "Set 'minF' error!");
        saveJsSet(MjsMidFrequency, mjs_mk_number(_mjs, MidFrequency), "Set 'midF' error!");
        saveJsSet(MjsMaxFrequency, mjs_mk_number(_mjs, MaxFrequency), "Set 'maxF' error!");
        saveJsSet(MjsNodeLedCount, mjs_mk_number(_mjs, LED_COUNT), "Set 'nLedC' error!");

        saveJsExec(mod.c_str(), "MOD script error!");
        saveJsGet("update", _updateFunc, "Get 'update' func error!");
        
        // Testrun the script
        updateLedInfo(0, LED_COUNT);
        updateAnalyzerResult(0, 0);
        run(0);
    }

    ScriptContext::~ScriptContext()
    {
        mjs_destroy(_mjs);
    }

    void ScriptContext::updateLedInfo(const uint16_t previousLedCount, const uint16_t meshLedCount)
    {
        if(isFaulted()) return;
        saveJsSet(MjsPreviosLedCount, mjs_mk_number(_mjs, previousLedCount), "Set 'pLedC' error!");
        saveJsSet(MjsMeshLedCount, mjs_mk_number(_mjs, meshLedCount), "Set 'mLedC' error!");
    }

    void ScriptContext::updateAnalyzerResult(const uint8_t volume, const uint16_t dominantFrequency)
    {
        if(isFaulted()) return;

        saveJsSet(MjsLastVolume, mjs_mk_number(_mjs, _lastVolume), "Set 'lVol' error!");
        saveJsSet(MjsLastFrequency, mjs_mk_number(_mjs, _lastFrequency), "Set 'lFreq' error!");
        saveJsSet(MjsVolume, mjs_mk_number(_mjs, volume), "Set 'vol' error!");
        saveJsSet(MjsFrequency, mjs_mk_number(_mjs, dominantFrequency), "Set 'freq' error!");

        _lastVolume = volume;
        _lastFrequency = dominantFrequency;
    }

    void ScriptContext::run(const uint8_t delta)
    {  
        mjs_val_t a1 = mjs_mk_number(_mjs, delta);
        if(mjs_call(_mjs, NULL, _updateFunc, _mjsGlobal, 1, a1) != MJS_OK)
        {
            _faulted = true;
            Serial.println("Error on 'update'!");
        }
    }

    bool ScriptContext::isFaulted() { return _faulted; }
    void ScriptContext::setLed(uint32_t index, Color color) { _led.setLed(index, color); }
    
    void ScriptContext::saveJsExec(const char* script, const char* errMessage)
    {
        if(!_faulted)
        {
            if(mjs_exec(_mjs, script, NULL) != MJS_OK)
            {
                _faulted = true;
                Serial.println(errMessage);
            }
        }        
    }

    void ScriptContext::saveJsGet(const char* mjsVarName, mjs_val_t& saveToVal, const char* errMessage)
    {
        if(!_faulted)
        {
            saveToVal = mjs_get(_mjs, _mjsGlobal, mjsVarName, ~0);
            if(mjs_is_undefined(saveToVal))
            {
                _faulted = true;
                Serial.println(errMessage);
            }
        }        
    }

    void ScriptContext::saveJsSet(const char* mjsVarName, mjs_val_t value, const char* errMessage)
    {
        if(!_faulted)
        {
            if(mjs_set(_mjs, _mjsGlobal, mjsVarName, ~0, value) != MJS_OK)
            {
                _faulted = true;
                Serial.println(errMessage);
            }
        }        
    }
}