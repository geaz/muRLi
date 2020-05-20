#ifndef MODULEVIEW_H
#define MODULEVIEW_H

#include "view.hpp"
#include "display.hpp"

namespace Murli
{
    class ModuleView : public View
    {
        public:
            void drawFrame(U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI& display) override
            {
                display.setFont(u8g2_font_maniac_tf);
                drawVHCenteredStr(display, "module");
            }
    };
}

#endif // MODULEVIEW_H