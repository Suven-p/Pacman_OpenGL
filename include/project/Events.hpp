#pragma once
#include <any>
#include <string>
#include <map>
#include <boost/signals2/signal.hpp>

class Events {
    public:
    template<typename T>
    using Event = boost::signals2::signal<T>;

    using RenderEvent = Event<void (std::string)>;
    RenderEvent render_event;
    using RenderEventSlot = RenderEvent::slot_type;
};
