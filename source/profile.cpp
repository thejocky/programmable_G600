#include <profile.hpp>

// Layer class Declaration ----------------------------------------------------



template<size_t layerSize_>
Profile<layerSize_>::Layer::Layer(size_t size) :
    events_({}),
    usedEvents_(0),
{}

template<size_t layerSize_>
EventType Profile<layerSize_>::Layer::
setEvent(size_t n, const char* cmd, EventType type) {
    EventType tmp = events_[n];
    events_[n] = {cmd, type};
    return tmp;
}

template<size_t layerSize_>
EventType Profile<layerSize_>::Layer::
setEvent(size_t n, Layer* layer, EventType type) {
    EventType tmp = events_[n];
    events_[n] = {layer, type};
    return tmp;
}

template<size_t layerSize_>
EventType Profile<layerSize_>::Layer::eventNType(size_t n) {
    return events[n].type;
}


template<size_t layerSize_>
bool Profile<layerSize_>::Layer::
appendEvent(const char* event, EventType type) {
    if (usedEvents_ >= layerSize_) return false;
    events_[usedEvents_] = {event, type};
    usedEvents_++;
    return true;
}

template<size_t layerSize_>
bool Profile<layerSize_>::Layer::
appendEvent(Layer* layer, EventType type) {
    if (usedEvents_ >= layerSize_) return false;
    events_[usedEvents_] = {layer, type};
    usedEvents_++;
    return true;
}


// Profile Class Declaration
