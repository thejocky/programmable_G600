#include <profile.hpp>

#include <string.h>

// Layer class Declaration ----------------------------------------------------



LayerLink* LayerLink::generateLayerLink(size_t layerSize) {
    LayerLink* layer =  reinterpret_cast<LayerLink*>
        (new uint8_t[sizeof(LayerLink) + sizeof(LayerLink::Event)*layerSize]);
    layer->setSize(layerSize);
    return layer;
}

EventType LayerLink::setEvent(size_t n, const char* cmd, EventType type) {
    EventType tmp = events_[n].type;
    events_[n] = Event{cmd, type};
    return tmp;
}

EventType LayerLink::setEvent(size_t n, LayerLink* layer, EventType type) {
    EventType tmp = events_[n].type;
    // Reinterpret_cast is needed as the union requires and const char* in initializer
    // even though union can hold LayerLink*
    events_[n] = Event{reinterpret_cast<const char*>(layer), type};
    return tmp;
}

EventType LayerLink::eventNType(size_t n) {
    return events_[n].type;
}

// Profile Class Declaration --------------------------------------------------


Profile::Profile() :
    first_(nullptr),
    last_(nullptr)
{}

Profile::~Profile() {
    // Pop and delete first element of linkedlist until empty
    LayerLink* tmp;
    while (first_) {
        tmp = first_;
        first_ = first_->next();
        delete[] reinterpret_cast<uint8_t*>(tmp);
    }
    last_ = nullptr;
}

int Profile::executeCommand(const char* cmd) {
    return system(cmd);
}

int Profile::spawnCommand(const char* cmd) {
    return 1;
}

void Profile::forwardLayer(LayerLink* layer) {

    // If already at front do nothing and return
    if (layer == first_) return;

    // Break layer out of list
    layer->back()->setNext(layer->next());
    if (layer->next())
        layer->next()->setBack(layer->back());

    // Push to begining of list
    layer->setNext(first_);
    layer->setBack(nullptr);
    first_->setBack(layer);
    first_ = layer;
}

int Profile::executeEvent(size_t n, LayerLink* layer) {

    if (!layer) return 0;
    if (layer->event(n).type == EventType::EmptyEvent)
        return executeEvent(n, layer->next());

    switch(layer->event(n).type) {
        case EventType::ExecuteCommand:
            executeCommand(layer->event(n).event.command);
            break;
        
        case EventType::SpawnCommand:
            spawnCommand(layer->event(n).event.command);
            break;

        case EventType::ForwardLayer:
            forwardLayer(layer->event(n).event.layer);
            break;
        
        default: break;
    }
    return 0;
}

void Profile::appendLayer(LayerLink* layer) {
    layer->setBack(nullptr);
    layer->setNext(first_);
    if (first_)
        first_->setBack(layer);
    first_ = layer;
}


void Profile::logProfile(std::ostream &stream) {
    LayerLink* layer = first_;
    size_t layerIndex = 0;
    
    stream << "Profile:\n";
    while (layer) {
        stream <<"  Layer " << layerIndex << ":\n";
        for (size_t i = 0; i < layer->size(); i++) {
            stream << "    Event " << i << ": ";
            switch(layer->event(i).type) {
                case EventType::EmptyEvent:
                    stream << "Empty Event: \n";
                    break;
                case EventType::NoEvent:
                    stream << "No Event: \n";
                    break;
                case EventType::ExecuteCommand:
                    stream << "Execute Command: "
                            << layer->event(i).event.command << '\n';
                    break;
                case EventType::SpawnCommand:
                    stream << "Spawn Command: "
                            << layer->event(i).event.command << '\n';
                    break;
                case EventType::ForwardLayer:
                    stream << "Forward Layer: \n";
                    break;
                default: break;
            }
        }
        layer = layer->next();
        layerIndex++;
    }
    
}




int main() {

    Profile profile;
    LayerLink* layer1 = LayerLink::generateLayerLink(4);
    LayerLink* layer2 = LayerLink::generateLayerLink(4);

    profile.logProfile(std::cout);
    std::cout << '\n';

    layer1->setEvent(0, "test1", EventType::SpawnCommand);
    layer1->setEvent(1, "data doesn't matter", EventType::NoEvent);
    layer1->setEvent(2, layer2, EventType::ForwardLayer);

    layer2->setEvent(0, layer1, EventType::ForwardLayer);
    layer2->setEvent(1, "echo Command Execution Success!", EventType::ExecuteCommand); 
    

    profile.appendLayer(layer1);
    profile.appendLayer(layer2);

    profile.logProfile(std::cout);
    std::cout << '\n';

    profile.executeEvent(0);

    profile.logProfile(std::cout);
    std::cout << '\n';

    return 0;
}


