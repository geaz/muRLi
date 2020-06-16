#ifndef LEDPATTERN_H
#define LEDPATTERN_H

namespace Murli
{
    class LED;
    class LedPattern
    {
        public:
            virtual void loop(LED& led) = 0;
    };
}

#endif // LEDPATTERN_H