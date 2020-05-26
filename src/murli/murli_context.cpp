#include <Arduino.h>

#include "murli_context.hpp"
#include "states/no_mod_state.cpp"
#include "states/receive_write_state.cpp"

namespace Murli
{
    MurliContext::MurliContext(LED& led, Display& display) :
        _led(led),
        _display(display)
    {
        _currentState = std::make_shared<NoModState>();
    }

    void MurliContext::loop()
    {
        if(Serial.available() > 0
            && !writeRequested
            && isModInserted())
        {
            _currentState = std::make_shared<ReceiveWriteState>();
        }
        // Reset state, if no MOD inserted
        else if(!isModInserted())
        {
            _currentState = std::make_shared<NoModState>();
        }
        
        _currentState->run(*this);
    }

    bool MurliContext::isModInserted() const
    {
        return digitalRead(D6) == HIGH;
    }

    LED& MurliContext::getLed() { return _led; }
    Rom24LC32A& MurliContext::getRom() { return _rom; }
    Display& MurliContext::getDisplay() { return _display; }
    void MurliContext::setState(std::shared_ptr<State> state) { _currentState = state; }
}