#include <stdlib.h>
#include <stdexcept>
#include <vector>


enum class EventType {
    NoEvent,
    ExecuteCommand,
    SwitchLayer,
};

template<size_t eventCount_>
class Profile {

    struct Event {
        union EventUnion {
            char* command;
            Event* layer;
        } event;
        eventType type;
    };

    public:

    template<size_t size_>
    class Layer {
        Event events_[size_]; // Layer represented as arrays of events
        size_t usedEvents_;   // Number of events in use   

        public:

        Layer(size_t size);
        ~Layer() {}
        
        size_t size() {return layerSize_;}
        EventType setEvent(size_t n, const char* event, EventType type);
        EventType setEvent(size_t n, Layer*, EventType type);
        bool deleteEvent(size_t n);
        EventType eventNType(size_t n);

        // Used in creating layer. Will add events starting from 0 going  to size_-1
        // After reaching appending event size_-1, use set and delete event to modify layer further
        bool appendEvent(const char* event, EventType type);
        bool appendEvent(Layer* event, EventType type);


        protected:

        Event event(size_t n) {return events_[n];}

    };

    private:


    size_t layerCount_;          // Number of layers in profile
    std::vector<Layer<eventCount_>*> layers_; // vector used to store layers in profile
    Layer<eventCount_>* workingLayer_;        // Current operating layer

    public:

    Profile(size_t eventCount);
    ~Profile();


    int executeCommand(const char* cmd);
    int spawnCommand(const char* cmd);
    void setLayer(Layer* layer);
    void addOverlay(Layer* later);

    int executeEvent(size_t n);

    void appendLayer(Layer* layer);

};