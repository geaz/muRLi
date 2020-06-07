#ifndef JSCONTEXT_H
#define JSCONTEXT_H

#include "../led/color.hpp"

namespace Murli
{
    class ScriptContext
    {
        public:
            ScriptContext(const std::string mod);
            
            ColorFrame run(uint8_t volume, uint16_t frequency);
            void setColorFrame(uint8_t frame, uint32_t hexColor);

            bool isFaulted();

            static void receiveScriptOutput(int c);
            static void setColorFrameFunc(uint8_t frame, uint32_t hexColor);

        private:
            const std::string _mod;
            std::array<char, 2048> _scriptArena;

            std::array<Color, 2> _colors;
            uint32_t _lastFrequency = 0;

            bool _faulted = false;
    };

    // Pointer for TinyScript Callbacks
    extern ScriptContext* ScriptContextPointer;
}

#endif // JSCONTEXT_H