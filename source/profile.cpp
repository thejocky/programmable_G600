#include <profile.hpp>

// Layer class Declaration ----------------------------------------------------



template<size_t layerSize_>
Profile<layerSize_>::Layer::Layer() :
    events_({}),
    usedEvents_(0)
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
    return events_[n].type;
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


// Profile Class Declaration --------------------------------------------------


template<size_t layerSize_>
Profile<layerSize_>::Profile() :
    first_(nullptr),
    last_(nullptr)
{}

template<size_t layerSize_>
Profile<layerSize_>::~Profile() {
    // Pop and delete first element of linkedlist until empty
    Link* tmp;
    while (first_) {
        tmp = first_;
        first_ = first_->next;
        delete tmp;
    }
    last_ = nullptr;
}

template<size_t layerSize_>
int Profile<layerSize_>::executeCommand(const char* cmd) {
    return 1;
}

template<size_t layerSize_>
int Profile<layerSize_>::spawnCommand(const char* cmd) {
    return 1;
}

template<size_t layerSize_>
void Profile<layerSize_>::forwardLayer(Link* layer) {

    // If already at front do nothing and return
    if (layer = first_) return;

    // Break layer out of list
    layer->back->next = layer->next;
    if (layer->next)
        layer->next->back = layer->back;

    // Push to begining of list
    layer->next = first_;
    layer->back = nullptr;
    first_->back = layer;
    first_ = layer;
}

template<size_t layerSize_>
int Profile<layerSize_>::executeEvent(size_t n, Link* layer) {

    if (!layer) return 0;
    if (layer->data.event(n).type == EventType::EmptyEvent)
        return executeEvent(n, layer);

    switch(layer->data.event(n).type) {
        case EventType::ExecuteCommand:
            executeEvent(layer->data.event(n).event.command);
            break;
        
        case EventType::SpawnCommand:
            spawnCommand(layer->data.event(n).event.command);
            break;

        case EventType::ForwardLayer:
            forwardLayer(layer->data.event(n).event.layer);
            break;
        
        default: break;
    }
    return 0;
}

template<size_t layerSize_>
void Profile<layerSize_>::appendLayer(Link* layer) {
    layer->back = nullptr;
    layer->next = first_;
    first_->back = layer;
    first_ = layer;
}
