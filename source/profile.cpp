#include <profile.hpp>

// Layer class Declaration ----------------------------------------------------



template<size_t layerSize_>
Profile<layerSize_>::Layer::Layer() :
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


// Profile Class Declaration --------------------------------------------------


template<size_t layerSize_>
Profile<layerSize_>::Profile() :
    layerCount_(0),
    workingLayer_(nullptr),
{}

template<size_t layerSize_>
Profile<layerSize_>::~Profile() {
    for (auto layer : layers_) {
        delete layer;
    }
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
void Profile<layerSize_>::setLayer(Layer* layer) {

}

template<size_t layerSize_>
void Profile<layerSize_>::addOverlay(Layer* later) {

}

template<size_t layerSize_>
int Profile<layerSize_>::executeEvent(size_t n) {
    switch(workingLayer_->event(n).type) {
        case EventType::ExecuteCommand:
            executeEvent(workingLayer_->event(n).event.command);
            break;
        
        case EventType::SpawnCommand:
            spawnCommand(workingLayer_->event(n).event.command);
            break;

        case EventType::SwitchLayer:
            setLayer(workingLayer_->event(n).event.layer);
            break;
        
        case EventType::PushOverlay:
            addOverlay(workingLayer_->event(n).event.layer);
            break;
        
        default: break:
    }
    return 0;
}

template<size_t layerSize_>
void Profile<layerSize_>::appendLayer(Layer* layer) {
    layers.push_back(new Layer());
}
