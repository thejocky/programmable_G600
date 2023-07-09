#pragma once

#include <fstream>
#include <map>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <cstdint>

class EventMap {
    template<typename KEY, typename T>
    struct Node {
        int event;
        std::map<KEY, T> map;
        auto find(const KEY &key) {return map.find(key);}
        T& operator[](const KEY& key) {return map[key];}
    };

    public:

    #define ValueMap Node<int32_t, size_t>
    #define CodeMap Node<uint16_t, ValueMap>
    #define TypeMap std::map<uint16_t, CodeMap>

    TypeMap map;

    EventMap() {}
    ~EventMap() {}

    void loadEvent(size_t event, uint16_t type);
    void loadEvent(size_t event, uint16_t type, uint16_t code);
    void loadEvent(size_t event, uint16_t type, uint16_t code, int32_t value);

    int event(uint16_t type, uint16_t code, int32_t value);

};

class Device {
    const char* eventPath_;
    int fd_;
    bool open_;
    EventMap eventMap_;

    #define bufferSize_ 10
    struct input_event buffer_[bufferSize_];

    public:

    Device(const char* eventPath);
    ~Device() {}

    void addEvent(size_t event, uint16_t type);
    void addEvent(size_t event, uint16_t type, uint16_t code);
    void addEvent(size_t event, uint16_t type, uint16_t code, int32_t value);

    bool openHandler();
    bool isOpen() {return open_;}
    bool read(size_t &event);
    bool readRaw(input_event &event);
};