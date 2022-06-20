#include <stdlib.h>
#include <stdexcept>
#include <vector>
#include <exception>


enum class EventType {
    EmptyEvent = 0, // No event in layer, fall to and execute next layer
    NoEvent,        // No event in layer, don't execute anything
    ExecuteCommand, // Execute shell command
    SwitchLayer,    // Switch to new layer
    PushOverlay,    // Add overlay to execution list
};

template<size_t layerSize_>
class Profile {

    struct Event {
        union EventUnion {
            char* command;
            Event* layer;
        } event;
        eventType type;
    };

    public:

    class Layer {
        Event events_[layerSize_]; // Layer represented as arrays of events
        size_t usedEvents_;   // Number of events in use   

        public:

        Layer();
        ~Layer() {}
        
        size_t size() {return layerSize_;}
        EventType setEvent(size_t n, const char* event, EventType type);
        EventType setEvent(size_t n, Layer* layer, EventType type);
        EventType eventNType(size_t n);

        // Used in creating layer. Will add events starting from 0 going to size_-1
        // After appending event size_-1 no further events will be added,
        // use set and delete event to modify layer further
        bool appendEvent(const char* event, EventType type);
        bool appendEvent(Layer* layer, EventType type);


        protected:

        Event event(size_t n) {return events_[n];}

    };

    private:


    size_t layerCount_;          // Number of layers in profile
    std::vector<Layer<layerSize_>*> layers_; // vector used to store layers in profile
    Layer<layerSize_>* workingLayer_;        // Current operating layer

    public:

    Profile();
    ~Profile();


    int executeCommand(const char* cmd);
    int spawnCommand(const char* cmd);
    void setLayer(Layer* layer);
    void addOverlay(Layer* later);

    int executeEvent(size_t n);

    void appendLayer(Layer* layer);

};