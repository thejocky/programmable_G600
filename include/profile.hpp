#pragma once

#include <stdlib.h>
#include <stdexcept>
#include <vector>
#include <exception>
#include <iostream>


enum class EventType {
    EmptyEvent = 0, // No event in layer, fall to and execute next layer
    NoEvent,        // No event in layer, don't execute anything
    ExecuteCommand, // Execute shell command
    SpawnCommand,   // Execute shell command then break from command
    ForwardLayer,   // Move layer to front of execution stack
};


// // Forward declair Event Layer Profile and Link types
// struct Event;
// class Layer;
// struct Link;
class Profile;





// class Layer {

//     struct Event {
//         union EventUnion {
//             const char* command;
//             Link* layer;
//         } event;
//         EventType type;
//     };

//     size_t usedEvents_; // Number of events in use
//     size_t layerSize_;  // Number of events in layer
//     Event events_[];    // Layer represented as arrays of events

//     public:

//     Layer() = delete;
//     ~Layer() {}

//     size_t size() {return layerSize_;}
//     EventType setEvent(size_t n, const char* event, EventType type);
//     EventType setEvent(size_t n, Layer* layer, EventType type);
//     EventType eventNType(size_t n);

//     // Used in creating layer. Will add events starting from 0 going to size_-1
//     // After appending event size_-1 no further events will be added,
//     // use set and delete event to modify layer further
//     bool appendEvent(const char* event, EventType type);
//     bool appendEvent(Link* layer, EventType type);


//     protected:

//     Event event(size_t n) {return events_[n];}

//     friend Profile;
// };

// // link structure used to store layers in linked list
// struct Link {
//     Link* back;
//     Link* next;
//     Layer data;
// };

// class Link {
//     Link* back_;
//     Link* next_;
//     Layer data_;

//     public:

//     Link() = delete;
//     static Link* generateLink(size_t layerSize);

//     Link* back() {return back_;}
//     Link* next() {return next_;}
//     Layer& data() {return data_;}



// };


class LayerLink {

    struct Event {
        union EventUnion {
            const char* command;
            LayerLink* layer;
        } event;
        EventType type;
    };

    LayerLink* back_;
    LayerLink* next_;
    size_t usedEvents_; // Number of events in use
    size_t layerSize_;  // Number of events in layer
    Event events_[];    // Layer represented as arrays of events

    public:

    LayerLink() = delete;
    ~LayerLink() {}

    static LayerLink* generateLayerLink(size_t layerSize);

    LayerLink* next() {return next_;}
    LayerLink* back() {return back_;}
    size_t size() {return layerSize_;}

    void setNext(LayerLink* next) {next_ = next;}
    void setBack(LayerLink* back) {back_ = back;}
    EventType setEvent(size_t n, const char* event, EventType type);
    EventType setEvent(size_t n, LayerLink* layer, EventType type);
    EventType eventNType(size_t n);


    protected:

    Event event(size_t n) {return events_[n];}

    friend Profile;
};



class Profile {


    // Execution list
    // In case of execution, first layer with filled event will be executed
    LayerLink* first_;
    LayerLink* last_; 


    public:

    Profile(size_t layerSize);
    ~Profile();

    private:

    // Execute given shell command
    int executeCommand(const char* cmd);
    // Execute given shell command then break from it
    int spawnCommand(const char* cmd);
    // Move Layer to front of execution list
    void forwardLayer(LayerLink* layer);

    // Starting from layer crawl throuhg list untill event reached
    int executeEvent(size_t event, LayerLink* layer);

    public:

    int executeEvent(size_t event) {
        return executeEvent(event, first_);
    }

    // push layer link to internal execution list
    void appendLayer(LayerLink* layer);
  
    // For testing purposes
    // pushes to stream the current state of profile in zzz format
    void logProfile(std::ostream &stream);

};


