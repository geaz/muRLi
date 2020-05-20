#ifndef DISPLAY_H
#define DISPLAY_H

#include <U8g2lib.h>
#include "view.hpp"

namespace Murli
{
    class Display
    {
        public:
            Display();

            void init();
            void drawView();
            void setView(View* view);

            U8G2_SH1106_128X64_NONAME_F_4W_SW_SPI ctrl;

        private:
            View* _currentView;
    };
}

#endif // DISPLAY_H