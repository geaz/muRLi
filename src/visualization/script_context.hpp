#ifndef JSCONTEXT_H
#define JSCONTEXT_H

#include "../led/color.hpp"

namespace Murli
{
    struct ColorFrame
    {
        Color first;
        Color second;
        Color third;
    };

    class ScriptContext
    {
        public:
            ScriptContext(const std::string& mod);

            ColorFrame getColorFrame();
            std::string getModName();

            bool isFaulted();

        private:
            const std::string& _mod;
            char _scriptArena[4096];
            bool _faulted = false;
            std::string _message = "";
    };

    // Pointer for TinyScript Callbacks
    extern ScriptContext* ScriptContextPointer;
}

#endif // JSCONTEXT_H