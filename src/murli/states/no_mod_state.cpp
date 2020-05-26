#ifndef NOMODSTATE_H
#define NOMODSTATE_H

#include "read_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/centeredTextView.cpp"

namespace Murli
{
    class NoModState : public State
    {
        public:
            NoModState()
            {
                _centeredTextView = std::make_shared<CenteredTextView>();
                _centeredTextView->setText("Insert MOD ...");
            }

            void run(MurliContext& context)
            {
                context.getLed().fadeLoop(Murli::Yellow);
                context.getDisplay().setView(_centeredTextView);

                if(context.isModInserted())
                {
                    context.getLed().stopFadeLoop();
                    context.setState(std::make_shared<ReadModState>());
                }
            }

        private:
            std::shared_ptr<CenteredTextView> _centeredTextView;
    };
}

#endif // NOMODSTATE_H