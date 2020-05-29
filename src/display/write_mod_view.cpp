#ifndef WRITEMODVIEW_H
#define WRITEMODVIEW_H

#include "view.hpp"
#include "display.hpp"

namespace Murli
{
    class WriteModView : public View
    {
        public:
            void drawFrame(DisplayCtrl& display) override
            {
                display.setFontPosCenter();
                uint8_t displayCenter = (display.getDisplayHeight() - 10) / 2;

                display.setFont(u8g2_font_open_iconic_embedded_2x_t);
                display.drawGlyph(10, displayCenter, 69);
                
                display.setFont(u8g2_font_5x8_tf);
                display.drawStr(35, displayCenter, _text.c_str());

                display.setFontPosBaseline();                
            }

            void setText(const std::string& text) 
            {
                _text = text;
            }
        
        private:
            std::string _text;
    };
}

#endif // WRITEMODVIEW_H