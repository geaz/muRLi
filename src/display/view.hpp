#ifndef VIEW_H
#define VIEW_H

#include <U8g2lib.h>

namespace Murli
{
    typedef U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI DisplayCtrl;
    
    class View
    {
        public:
            virtual void drawFrame(DisplayCtrl& display) = 0;

            void drawVHCenteredStr(DisplayCtrl& display, const std::string& text)
            {
                display.setFontPosCenter();
                u8g2_uint_t width = display.getStrWidth(text.c_str());
                display.drawStr((display.getWidth() / 2) - (width / 2), display.getHeight() / 2, text.c_str());
                display.setFontPosBaseline();
            }
    };
}

#endif // VIEW_H