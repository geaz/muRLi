#include "display.hpp"

namespace Murli
{
    Display::Display() : _ctrl(U8G2_R0,/* cs=*/ D8,/* dc=*/ D0,/* reset=*/ D3) { }

    void Display::init()
    {
        _ctrl.begin();
    }

    void Display::loop()
    {
        _ctrl.clearBuffer();
        _currentView->drawFrame(_ctrl);
        _ctrl.sendBuffer();
    }

    void Display::setView(const std::shared_ptr<View> view) { _currentView = view; }
}