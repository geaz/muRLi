#ifndef RUNMODSTATE_H
#define RUNMODSTATE_H

#include "../state.hpp"
#include "../murli_context.hpp"
#include "../../display/run_mod_view.cpp"
#include "../../visualization/script_context.hpp"
#include "../../visualization/sound_visualization.hpp"

namespace Murli
{
    class RunModState : public State
    {
        public:
            RunModState()
            {
                _runModView = std::make_shared<RunModView>();
            }

            void run(MurliContext& context)
            {                         
                context.getLed().setColor(Murli::Yellow);
                context.getDisplay().setView(_runModView);
                _soundVisualization.getVolAndFreq();
            }

        private:
            SoundVisualization _soundVisualization;
            std::shared_ptr<RunModView> _runModView;
    };
}

#endif // RUNMODSTATE_H