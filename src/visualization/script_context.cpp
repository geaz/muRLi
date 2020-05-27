#include "script_context.hpp"
extern "C" {
    #include <tinyscript.h>
}

static void test(uint8_t frame, uint32 hexColor){ Serial.println(hexColor); }
void outchar(int c) { Serial.print((char)c); }

namespace Murli
{
    ScriptContext* ScriptContextPointer = nullptr;

    ScriptContext::ScriptContext(const std::string& mod) : _mod(mod)
    {
        ScriptContextPointer = this;

        if(TinyScript_Init(_scriptArena, sizeof(_scriptArena)) != 0)
        {
            _faulted = true;
            _message = "Init TS Error!";
            Serial.println("Init TS Error!");
        }

        TinyScript_Define("sCF", CFUNC(2), (intptr_t)test);        
        if(TinyScript_Run(_mod.c_str(), false, false) != 0)
        {
            _faulted = true;
            _message = "Script TS Error!";
            Serial.println("Script TS Error!");
        }
    }

    std::string ScriptContext::getModName()
    {
        std::string modName = "";
       /* duk_get_global_string(_dukContext, "gMN");
        if (duk_pcall(_dukContext, 0) != 0) 
        {
            _faulted = true;
            _message = "MOD name error!";
            Serial.println(duk_safe_to_string(_dukContext, -1));
        }
        else
        {
            modName = duk_safe_to_string(_dukContext, -1);
        }
        duk_pop(_dukContext);*/

        return modName;
    }

    bool ScriptContext::isFaulted()
    {
        return _faulted;
    }
}