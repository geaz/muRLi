#include "script_context.hpp"
#include "mjs_funcs.hpp"

namespace Murli
{
    ScriptContext::ScriptContext(LED& led, const std::string& mod) : _led(led)
    {
        _mjs = mjs_create();
        
        mjs_val_t global = mjs_get_global(_mjs);
        mjs_set(_mjs, global, "map", ~0, mjs_mk_foreign_func(_mjs, (mjs_func_ptr_t)doubleMap));
        mjs_set(_mjs, global, "xrgb", ~0, mjs_mk_foreign_func(_mjs, (mjs_func_ptr_t)xrgb));
        mjs_set(_mjs, global, "xhsv", ~0, mjs_mk_foreign_func(_mjs, (mjs_func_ptr_t)xhsv));

        saveJsExec("let minF=130; let midF=1046; let maxF=3140; let mLedC=0; let lVol=0;let lFreq=0;let vol=0;let freq=0;", "Set variables error!");         
        saveJsExec("function updateMeshLedCount(meshLedCount){ mLedC=meshLedCount; }", "Set 'updateMeshLedCount' error!");
        saveJsExec("function updateAnalyzerValues(newVol, newFreq){ lVol=vol;lFreq=freq;vol=newVol;freq=newFreq; }", "Set 'updateAnalyzerValues' error!");
        saveJsExec(mod.c_str(), "MOD script error!");

        saveJsGetFunc(_updateMeshLedCountFunc, "updateMeshLedCount", "Get 'updateMeshLedCount' func error!");
        saveJsGetFunc(_updateAnalyzerValuesFunc, "updateAnalyzerValues", "Get 'updateAnalyzerValues' func error!");
        saveJsGetFunc(_onAnalyzerUpdateFunc, "onAnalyzerUpdate", "Get 'onAnalyzerUpdate' func error!");
        saveJsGetFunc(_onLedUpdateFunc, "onLedUpdate", "Get 'onLedUpdate' func error!");
        saveJsGetFunc(_updateLed, "updateLed", "Get 'updateLed' func error!");
        
        run(0, LED_COUNT, 0);
    }

    ScriptContext::~ScriptContext()
    {
        mjs_destroy(_mjs);
    }

    void ScriptContext::updateAnalyzerResult(AnalyzerResult result)
    {
        mjs_val_t global = mjs_get_global(_mjs);
        mjs_val_t a1 = mjs_mk_number(_mjs, result.volume);
        mjs_val_t a2 = mjs_mk_number(_mjs, result.dominantFrequency);
        if(mjs_call(_mjs, NULL, _updateAnalyzerValuesFunc, global, 2, a1, a2) != MJS_OK)
        {
            _faulted = true;
            Serial.println("Error on 'updateAnalyzerValues'!");
            return;
        }

        if(mjs_call(_mjs, NULL, _onAnalyzerUpdateFunc, global, 0) != MJS_OK)
        {
            _faulted = true;
            Serial.println("Error on 'onAnalyzerUpdate'!");
        }
    }

    void ScriptContext::run(uint16_t previousLedCount, uint16_t meshLedCount, uint32_t delta)
    {  
        mjs_val_t global = mjs_get_global(_mjs);
        mjs_own(_mjs, &global);

        mjs_val_t a1 = mjs_mk_number(_mjs, meshLedCount);
        if(mjs_call(_mjs, NULL, _updateMeshLedCountFunc, global, 1, a1) != MJS_OK)
        {
            _faulted = true;
            Serial.println("Error on 'updateMeshLedCount'!");            
            mjs_disown(_mjs, &global);
            return;
        }

        a1 = mjs_mk_number(_mjs, delta);
        if(mjs_call(_mjs, NULL, _onLedUpdateFunc, global, 1, a1) != MJS_OK)
        {
            _faulted = true;
            Serial.println("Error on 'onLedUpdate'!");            
            mjs_disown(_mjs, &global);
            return;
        }

        for(uint16_t index = previousLedCount; index < previousLedCount + LED_COUNT; index++)
        {
            mjs_val_t res;
            a1 = mjs_mk_number(_mjs, index);
            if(mjs_call(_mjs, &res, _updateLed, global, 1, a1) != MJS_OK)
            {
                _faulted = true;
                Serial.println("Error on 'updateLed'!");
                break;
            }
            else
            {
                uint32_t hexColor = mjs_get_int32(_mjs, res);
                _led.setLed(index, Color::fromHex(hexColor));
            }
        }
        mjs_disown(_mjs, &global);
    }

    bool ScriptContext::isFaulted() { return _faulted; }
    
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

    void ScriptContext::saveJsGetFunc(mjs_val_t& saveToVal, const char* funcName, const char* errMessage)
    {
        if(!_faulted)
        {
            mjs_val_t global = mjs_get_global(_mjs);
            saveToVal = mjs_get(_mjs, global, funcName, ~0);
            if(mjs_is_undefined(saveToVal))
            {
                _faulted = true;
                Serial.println(errMessage);
            }
        }        
    }
}