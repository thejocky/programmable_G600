#pragma once

#include <fstream>
#include <map>
#include <linux/input.h>
#include <linux/input-event-codes.h>

class EventMap {
    struct Node {
        int event;
        std::map<Node> children;
    };

    EventMap() {}
    ~EventMap() {}

    void loadEvent();

    int event()

};

class Device {

    public:

    struct EventDescriptor {
        uint16_t type;
        uint16_t code;
        int32_t value;

        // < operator for use in map class
        bool operator <(const EventDescriptor& other) const {
            return type < other.type ||
                type == other.type && code < other.code ||
                type == other.type && code == other.code && value < other.value;
        }
    };


    private:

    const char* eventPath_;
    int fd_;
    bool open_;
    // Maps input from event handler to event used by 
    std::map<EventDescriptor, size_t> eventMap_; 

    #define bufferSize_ 10
    struct input_event buffer_[bufferSize_];
    
    

    public:

    Device(const char* eventPath);
    Device(const char* eventPath, std::map<EventDescriptor, size_t> &eventMap);
    ~Device() {}

    void addEvent(size_t event, EventDescriptor ed);

    bool openHandler();
    bool isOpen() {return open_;}
    bool read(size_t &event);
    bool readRaw(input_event &event);
};