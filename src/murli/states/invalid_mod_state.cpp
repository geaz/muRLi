#ifndef INVALIDMODSTATE_H
#define INVALIDMODSTATE_H

#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/centeredTextView.cpp"

namespace Murli
{
    class InvalidModState : public State
    {
        public:
            InvalidModState()
            {
                _centeredTextView = std::make_shared<CenteredTextView>();
                _centeredTextView->setText("Invalid MOD ...");
            }

            void run(MurliContext& context)
            {
                context.getLed().setColor(Murli::Red);
                context.getDisplay().setView(_centeredTextView);
            }

        private:
            std::shared_ptr<CenteredTextView> _centeredTextView;
    };
}

#endif // INVALIDMODSTATE_H