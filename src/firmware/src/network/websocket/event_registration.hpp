#ifndef EVENTREGISTRATION_H
#define EVENTREGISTRATION_H

#include <map>

namespace Murli
{
    template<class T>
    class EventRegistration 
    {
        public:
            uint64_t addEventHandler(const T event)
            {
                uint64_t id = _events.size();
                _events[id] = event;
                return id;
            }

            void removeEventHandler(const uint64_t id)
            {
                _events.erase(id);
            }

            std::map<uint64_t, T> getEventHandlers() const
            {
                return _events;
            }

        private:
            std::map<uint64_t, T> _events;
    };
}

#endif // EVENTREGISTRATION_H