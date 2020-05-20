#ifndef CENTEREDTEXTVIEW_H
#define CENTEREDTEXTVIEW_H

#include "view.hpp"
#include "display.hpp"

namespace Murli
{
    class CenteredTextView : public View
    {
        public:
            void drawFrame(U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI& display) override
            {
                display.setFont(u8g2_font_5x8_tf);
                drawVHCenteredStr(display, _text);
            }

            void setText(const char* text) 
            {
                _text = text;
            }
        
        private:
            const char* _text;
    };
}

#endif // CENTEREDTEXTVIEW_H