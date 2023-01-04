#pragma once

#include <fstream>
#include <map>
#include <linux/input.h>
#include <linux/input-event-codes.h>

class EventMap {
    template<KEY, class T>
    struct Node {
        int event;
        std::map<KEY, T> map;
        T& operator[](const KEY& key) {return map[key];}
    };

    std::map<uint16_t, Node<uint16_t code, Node<int32_t, int>>> map;

    EventMap() {}
    ~EventMap() {}

    void loadEvent(int event, uint16_t type);
    void loadEvent(int event, uint16_t type, uint16_t code);
    void loadEvent(int event, uint16_t type, uint16_t code, int32_t value);

    int event(uint16_t type, uint16_t code, int32_t value);

};

class Device {

    public:

    // struct EventDescriptor {
    //     uint16_t type;
    //     uint16_t code;
    //     int32_t value;

    //     // < operator for use in map class
    //     bool operator <(const EventDescriptor& other) const {
    //         return type < other.type ||
    //             type == other.type && code < other.code ||
    //             type == other.type && code == other.code && value < other.value;
    //     }
    // };


    private:

    const char* eventPath_;
    int fd_;
    bool open_;
    // Maps input from event handler to event used by 
    // std::map<EventDescriptor, size_t> eventMap_; 
    EventMap eventMap_;


    #define bufferSize_ 10
    struct input_event buffer_[bufferSize_];
    
    

    public:

    Device(const char* eventPath);
    Device(const char* eventPath, std::map<EventDescriptor, size_t> &eventMap);
    ~Device() {}

    void addEvent(size_t event, uint16_t type);
    void addEvent(size_t event, uint16_t type, uint16_t code);
    void addEvent(size_t event, uint16_t type, uint16_t code, int32_t value);

    bool openHandler();
    bool isOpen() {return open_;}
    bool read(size_t &event);
    bool readRaw(input_event &event);
};