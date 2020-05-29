#ifndef READMODVIEW_H
#define READMODVIEW_H

#include "view.hpp"
#include "display.hpp"

namespace Murli
{
    class ReadModView : public View
    {
        public:
            void drawFrame(DisplayCtrl& display) override
            {
                display.setFontPosCenter();
                uint8_t displayCenter = (display.getDisplayHeight() - 10) / 2;

                display.setFont(u8g2_font_open_iconic_thing_2x_t);
                display.drawGlyph(10, displayCenter, 74);
                
                display.setFont(u8g2_font_5x8_tf);
                display.drawStr(35, displayCenter, "Reading MOD ...");

                display.setFontPosBaseline();                
            }
    };
}

#endif // READMODVIEW_H