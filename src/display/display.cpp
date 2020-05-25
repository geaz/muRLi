#include "display.hpp"

namespace Murli
{
    // Using Software SPI to be able to use D6 (MISO) as GPIO
    Display::Display() : ctrl(U8G2_R0,/*clock*/ D5,/*data*/ D7,/* cs=*/ D8,/* dc=*/ D0,/* reset=*/ D3) { }

    void Display::init()
    {
        ctrl.begin();
    }

    void Display::drawView()
    {
        ctrl.clearBuffer();
        _currentView->drawFrame(ctrl);
        ctrl.sendBuffer();
    }

    void Display::setView(View* view)
    {
        _currentView = view;
        drawView();
    }
}