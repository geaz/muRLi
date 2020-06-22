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
        saveJsSet("setGroup", mjs_mk_foreign_func(_mjs, (mjs_func_ptr_t)mjsSetGroup), "Set 'setGroup' error!");
        saveJsSet("clearGroups", mjs_mk_foreign_func(_mjs, (mjs_func_ptr_t)mjsClearGroups), "Set 'clearGroups' error!");
        saveJsSet("setDelay", mjs_mk_foreign_func(_mjs, (mjs_func_ptr_t)mjsSetDelay), "Set 'setDelay' error!");

        saveJsSet("minF", mjs_mk_number(_mjs, MinFrequency), "Set 'minF' error!");
        saveJsSet("midF", mjs_mk_number(_mjs, MidFrequency), "Set 'midF' error!");
        saveJsSet("maxF", mjs_mk_number(_mjs, MaxFrequency), "Set 'maxF' error!");
        saveJsSet("nLedC", mjs_mk_number(_mjs, LED_COUNT), "Set 'nLedC' error!");

        saveJsExec(mod.c_str(), "MOD script error!");
        saveJsGet("init", _initFunc, "Get 'init' func error!");
        saveJsGet("update", _updateFunc, "Get 'update' func error!");
    }

    ScriptContext::~ScriptContext()
    {
        mjs_destroy(_mjs);
    }

    void ScriptContext::updateLedInfo(const uint16_t previousNodeCount, const uint32_t previousLedCount, const uint32_t meshLedCount)
    {
        if(isFaulted()) return;
        saveJsSet("pNodeC", mjs_mk_number(_mjs, previousNodeCount), "Set 'pNodeC' error!");
        saveJsSet("pLedC", mjs_mk_number(_mjs, previousLedCount), "Set 'pLedC' error!");
        saveJsSet("mLedC", mjs_mk_number(_mjs, meshLedCount), "Set 'mLedC' error!");
    }

    void ScriptContext::init()
    {  
        if(mjs_call(_mjs, NULL, _initFunc, _mjsGlobal, 0) != MJS_OK)
        {
            _faulted = true;
            Serial.println("Error on 'init'!");
        }
    }

    void ScriptContext::updateAnalyzerResult(const uint8_t volume, const uint16_t dominantFrequency)
    {
        if(isFaulted()) return;

        bool updateResult = true;
        std::tuple<uint8_t, uint16_t> result = std::make_tuple(volume, dominantFrequency);

        if(_delay > 0)
        {
            _resultDeque.push_back(result);
            if(_resultDeque.size() > _delay)
            {
                result = _resultDeque.front();
                _resultDeque.pop_front();
            }
            else updateResult = false;
        }

        if(updateResult)
        {
            saveJsSet("lVol", mjs_mk_number(_mjs, _lastVolume), "Set 'lVol' error!");
            saveJsSet("lFreq", mjs_mk_number(_mjs, _lastFrequency), "Set 'lFreq' error!");
            saveJsSet("vol", mjs_mk_number(_mjs, std::get<0>(result)), "Set 'vol' error!");
            saveJsSet("freq", mjs_mk_number(_mjs, std::get<1>(result)), "Set 'freq' error!");

            _lastVolume = std::get<0>(result);
            _lastFrequency = std::get<1>(result);
        }
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
    void ScriptContext::setDelay(uint32_t delay) { if(_delay == 0) _delay = delay; }
    LED& ScriptContext::getLed() { return _led; }
    
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