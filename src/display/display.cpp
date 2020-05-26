#include "display.hpp"

namespace Murli
{
    // Using Software SPI to be able to use D6 (MISO) as GPIO
    Display::Display() : _ctrl(U8G2_R0,/*clock*/ D5,/*data*/ D7,/* cs=*/ D8,/* dc=*/ D0,/* reset=*/ D3) { }

    void Display::init()
    {
        _ctrl.begin();
    }

    void Display::loop()
    {
        _ctrl.clearBuffer();
        _ctrl.drawHLine(0, _ctrl.getDisplayHeight() - 10, _ctrl.getDisplayWidth());
        
        _ctrl.setFontPosBottom();
        _ctrl.setFont(u8g2_font_tom_thumb_4x6_tf);
        _ctrl.drawStr(0, _ctrl.getDisplayHeight(), _leftStatusText.c_str());
       
        u8g2_uint_t width = _ctrl.getStrWidth(_rightStatusText.c_str());
        _ctrl.drawStr(_ctrl.getDisplayWidth() - width, _ctrl.getDisplayHeight(), _rightStatusText.c_str());

        _ctrl.setFontPosBaseline();
        _currentView->drawFrame(_ctrl);
        _ctrl.sendBuffer();
    }

    void Display::setView(std::shared_ptr<View> view)
    {
        _currentView = view;
    }

    void Display::setLeftStatus(const std::string& text)
    {
        _leftStatusText = text;
    }

    void Display::setRightStatus(const std::string& text)
    {
        _rightStatusText = text;
    }
}