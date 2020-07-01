#ifndef STATE_H
#define STATE_H

#include <string>

namespace Murli
{
    class MurliContext;
    class State
    {
        public:
            virtual void run(MurliContext& context) = 0;
    };
}

#endif