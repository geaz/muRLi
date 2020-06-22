#ifndef NOMODSTATE_H
#define NOMODSTATE_H

#include <memory>
#include "read_mod_state.cpp"
#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../led/patterns/led_fade_pattern.cpp"
#include "../../display/views/insert_mod_view.cpp"

namespace Murli
{
    class NoModState : public State
    {
        public:
            NoModState()
            {
                _insertModView = std::make_shared<InsertModView>();
                _fadePattern = std::make_shared<LedFadePattern>(Murli::Yellow);
            }

            void run(MurliContext& context)
            {
                context.getLed().setPattern(_fadePattern);
                context.getDisplay().setView(_insertModView);
                if(context.isModInserted())
                {
                    context.getLed().clearGroups();
                    context.getLed().setPattern(nullptr);
                    context.currentState = std::make_shared<ReadModState>();
                }
            }

        private:
            std::shared_ptr<LedFadePattern> _fadePattern;
            std::shared_ptr<InsertModView> _insertModView;
    };
}

#endif // NOMODSTATE_H