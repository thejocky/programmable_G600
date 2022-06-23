#include <profile.hpp>

#include <string.h>

// Layer class Declaration ----------------------------------------------------



template<size_t layerSize_>
Profile<layerSize_>::Layer::Layer() :
    events_{},
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
    events_[usedEvents_].event.command = event;
    events_[usedEvents_].type = type;
    usedEvents_++;
    return true;
}

template<size_t layerSize_>
bool Profile<layerSize_>::Layer::
appendEvent(Link* layer, EventType type) {
    if (usedEvents_ >= layerSize_) return false;
    events_[usedEvents_].event.layer = layer;
    events_[usedEvents_].type = type;
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
    return system(cmd);
}

template<size_t layerSize_>
int Profile<layerSize_>::spawnCommand(const char* cmd) {
    return 1;
}

template<size_t layerSize_>
void Profile<layerSize_>::forwardLayer(Link* layer) {

    // If already at front do nothing and return
    if (layer == first_) return;

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
        return executeEvent(n, layer->next);

    switch(layer->data.event(n).type) {
        case EventType::ExecuteCommand:
            executeCommand(layer->data.event(n).event.command);
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
    if (first_)
        first_->back = layer;
    first_ = layer;
}


template<size_t layerSize_>
void Profile<layerSize_>::logProfile(std::ostream &stream) {
    Link* layer = first_;
    size_t layerIndex = 0;
    
    stream << "Profile:\n";
    while (layer) {
        stream <<"  Layer " << layerIndex << ":\n";
        for (size_t i = 0; i < layer->data.size(); i++) {
            stream << "    Event " << i << ": ";
            switch(layer->data.event(i).type) {
                case EventType::EmptyEvent:
                    stream << "Empty Event: \n";
                    break;
                case EventType::NoEvent:
                    stream << "No Event: \n";
                    break;
                case EventType::ExecuteCommand:
                    stream << "Execute Command: "
                            << layer->data.event(i).event.command << '\n';
                    break;
                case EventType::SpawnCommand:
                    stream << "Spawn Command: "
                            << layer->data.event(i).event.command << '\n';
                    break;
                case EventType::ForwardLayer:
                    stream << "Forward Layer: \n";
                    break;
                default: break;
            }
        }
        layer = layer->next;
        layerIndex++;
    }
    
}




int main() {

    Profile<3> profile;
    Profile<3>::Link* layer1 = new Profile<3>::Link{nullptr, Profile<3>::Layer(), nullptr};
    Profile<3>::Link* layer2 = new Profile<3>::Link{nullptr, Profile<3>::Layer(), nullptr};

    profile.logProfile(std::cout);
    std::cout << '\n';

    layer1->data.appendEvent("test1", EventType::SpawnCommand);
    layer1->data.appendEvent("data doesn't matter", EventType::NoEvent);
    layer1->data.appendEvent(layer2, EventType::ForwardLayer);

    layer2->data.appendEvent(layer1, EventType::ForwardLayer);
    layer2->data.appendEvent("event2 of layer 2", EventType::ExecuteCommand); 
    

    profile.appendLayer(layer1);
    profile.appendLayer(layer2);

    profile.logProfile(std::cout);
    std::cout << '\n';

    profile.executeEvent(1);
    profile.executeEvent(2);

    profile.logProfile(std::cout);
    std::cout << '\n';

    // profile.executeEvent(0);

    // profile.logProfile(std::cout);
    // std::cout << '\n';

    // profile.executeEvent(0);
    // profile.executeEvent(1);

    return 0;
}


