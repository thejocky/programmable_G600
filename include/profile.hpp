#include <stdlib.h>
#include <stdexcept>
#include <vector>
#include <exception>


enum class EventType {
    EmptyEvent = 0, // No event in layer, fall to and execute next layer
    NoEvent,        // No event in layer, don't execute anything
    ExecuteCommand, // Execute shell command
    SpawnCommand,   // Execute shell command then break from command
    ForwardLayer,   // Move layer to front of execution stack
};

template<size_t layerSize_>
class Profile {

    // Forward declair Event Layer and Link types
    struct Event;
    public:
    class Layer;
    struct Link;

    private:

    struct Event {
        union EventUnion {
            char* command;
            Link* layer;
        } event;
        EventType type;
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
        bool appendEvent(Link* layer, EventType type);


        protected:

        Event event(size_t n) {return events_[n];}

    };

    // link structure used to store layers in linked list
    struct Link {
        Link* back;
        Layer data;
        Link* next;
    };

    private:


    // Execution list
    // In case of execution, first layer with filled event will be executed
    Link* first_;
    Link* last_; 


    public:

    Profile();
    ~Profile();

    private:

    // Execute given shell command
    int executeCommand(const char* cmd);
    // Execute given shell command then break from it
    int spawnCommand(const char* cmd);
    // Move Layer to front of execution list
    void forwardLayer(Link* layer);

    // Starting from layer crawl throuhg list untill event reached
    int executeEvent(size_t event, Link* layer);

    public:

    int executeEvent(size_t event) {
        return executeEvent(event, first_);
    }

    // push layer link to internal execution list
    void appendLayer(Link* layer);

    bool compair(size_t layerCount, size_t layerSize, Event**);

};