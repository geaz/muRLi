#ifndef SPLASHVIEW_H
#define SPLASHVIEW_H

#include "view.hpp"
#include "display.hpp"

namespace Murli
{
    class SplashView : public View
    {
        public:
            void drawFrame(U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI& display) override
            {
                display.setFont(u8g2_font_maniac_tf);
                drawVHCenteredStr(display, "muRLi");
            }
    };
}

#endif // SPLASHVIEW_H