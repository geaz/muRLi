#ifndef INSERTMODVIEW_H
#define INSERTMODVIEW_H

#include "view.hpp"
#include "display.hpp"

namespace Murli
{
    class InsertModView : public View
    {
        public:
            void drawFrame(DisplayCtrl& display) override
            {
                animateGlyph(display);

                display.setFontPosCenter();
                display.setFont(u8g2_font_5x8_tf);
                display.drawStr(35, (display.getDisplayHeight() - 10) / 2, "Insert MOD ...");

                display.setFontPosBaseline();                
            }

            void animateGlyph(DisplayCtrl& display) 
            {
                if(_lastAnimate + 50 < millis())
                {
                    if(_glyphY == (display.getDisplayHeight() - 30) && _lastAnimate + 250 < millis())
                    {
                        _glyphY = 25;
                        _lastAnimate = millis();
                    }
                    else if(_glyphY < (display.getDisplayHeight() - 30))
                    {
                        _glyphY++;
                        _lastAnimate = millis();
                    }
                }

                display.setFontPosBottom();
                display.setFont(u8g2_font_open_iconic_arrow_2x_t);
                display.drawGlyph(10, _glyphY, 75);
            }
        
        private:
            uint8_t _glyphY = 25;
            uint64_t _lastAnimate = 0;
    };
}

#endif // INSERTMODVIEW_H