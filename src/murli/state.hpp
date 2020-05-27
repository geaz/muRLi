#ifndef STATE_H
#define STATE_H

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