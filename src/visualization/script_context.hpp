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

    /**
     * The ScriptContext uses 'TinyScript' (https://github.com/totalspectrum/tinyscript) to execute the MODs.
     * TinyScript is a very minimalistic script language. Therefore it was necessary to provide some 'hacks'.
     * The TinyScript implementation of muRLi uses a 'outputFunc' instead of 'outchar'
     * 
     * The ScriptContext registers a outputFunc and a 'setColorFrameFunc'.
     * MODs have to call the setColorFrameFunc (in script 'sCF') to set the different colors.
     * muRLi supports up to three colors per frame/calculation.
     * If the first frame is set, but the second is 'black' (0x000000) the third color will be ignored, too.
     * This way it is possible to set only one color in the script per run.
     * 
     * Furthermore each script has to do a 'print' as the last statement. This print will be the MOD name.
     * 
     * A complete MOD which would never change the color could look like:
     * 
     * sCF(1, 0xFFFFFF)
     * print "muRLi Rox"
     * 
     * On each calculation muRLi injects the current volume, current dominant frequency
     * and the last dominant frequency into the script as variables.
     * Volume => v (value between 0 and 100)
     * Dominant frequency => f
     * Last Dominant frequency => lf
     * 
     * var minF=130\nvar midF=1046\nvar maxF=3140
     * 
     * Each script has access to these variables to calculate colors based on these values.
     * The variable names are minimalistic to provide more space for the actual script (limited by the EEPROM). 
     */
    class ScriptContext
    {
        public:
            ScriptContext(const std::string& mod);
            
            void run(uint8_t vol, uint16_t frequency);
            
            ColorFrame getColorFrame();
            std::string getModName();

            void receiveModName(char c);
            void setColorFrame(uint8_t frame, uint32_t hexColor);

            bool isFaulted();
            std::string getMessage();

            static void receiveModNameFunc(int c);
            static void setColorFrameFunc(uint8_t frame, uint32_t hexColor);

        private:
            const std::string& _mod;
            char _scriptArena[4096];

            std::string _modName;
            std::vector<char> _charBuffer;
            uint32_t _lastFrequency = 0;
            Color _colors[3] = { Color::fromHex(0x000000) };

            bool _faulted = false;
            std::string _message;
    };

    // Pointer for TinyScript Callbacks
    extern ScriptContext* ScriptContextPointer;
}

#endif // JSCONTEXT_H