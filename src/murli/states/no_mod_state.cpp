#ifndef NOMODSTATE_H
#define NOMODSTATE_H

#include "read_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/views/insert_mod_view.cpp"

namespace Murli
{
    class NoModState : public State
    {
        public:
            NoModState()
            {
                _insertModView = std::make_shared<InsertModView>();
            }

            void run(MurliContext& context)
            {
                context.getLed().fadeLoop(Murli::Yellow);
                context.getDisplay().setView(_insertModView);

                if(context.isModInserted())
                {
                    context.getLed().stopFadeLoop();
                    context.currentState = std::make_shared<ReadModState>();
                }
            }

        private:
            std::shared_ptr<InsertModView> _insertModView;
    };
}

#endif // NOMODSTATE_H