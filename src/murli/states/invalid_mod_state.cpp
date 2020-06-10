#ifndef INVALIDMODSTATE_H
#define INVALIDMODSTATE_H

#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/views/invalid_mod_view.cpp"

namespace Murli
{
    class InvalidModState : public State
    {
        public:
            InvalidModState()
            {
                _invalidModView = std::make_shared<InvalidModView>();
            }

            void run(MurliContext& context)
            {
                context.getLed().setColor(Murli::Red);
                context.getDisplay().setView(_invalidModView);
            }

        private:
            std::shared_ptr<InvalidModView> _invalidModView;
    };
}

#endif // INVALIDMODSTATE_H