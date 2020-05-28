#ifndef RUNMODVIEW_H
#define RUNMODVIEW_H

#include "view.hpp"
#include "display.hpp"

namespace Murli
{
    class RunModView : public View
    {
        public:
            void drawFrame(U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI& display) override
            {
                display.setFontPosCenter();
                uint8_t displayCenter = (display.getDisplayHeight() - 10) / 2;

                display.setFont(u8g2_font_open_iconic_thing_2x_t);
                display.drawGlyph(10, displayCenter, 74);
                
                display.setFont(u8g2_font_5x8_tf);
                display.drawStr(35, displayCenter, "Run MOD ...");

                display.setFontPosBaseline();
            }
    };
}

#endif // RUNMODVIEW_H