#include "device.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <exception>

void EventMap::loadEvent(size_t event, uint16_t type) {
    if (!event) throw;
    map[type].event = event;
}
void EventMap::loadEvent(size_t event, uint16_t type, uint16_t code) {
    if (!event) throw;

    CodeMap* typeNode = &(map[type]);
    if (typeNode->event > 0) throw std::runtime_error("ERROR: Attempted overloaded type event with code event");
    *typeNode = CodeMap{-1};

    (*typeNode)[code].event = event;
}
void EventMap::loadEvent(size_t event, uint16_t type, uint16_t code, int32_t value) {
    if (!event) throw;

    CodeMap *codeMap = &(map[type]);
    if (codeMap->event > 0) throw std::runtime_error("ERROR: Attempted to overloaded type based event with value based event");
    codeMap->event = -1;

    ValueMap* valueMap = &(*codeMap)[code];
    if (valueMap->event > 0) throw std::runtime_error("ERROR: Attempted to overloaded code based event with a value based event");
    valueMap->event = -1;

    (*valueMap)[value] = event;
}


int EventMap::event(uint16_t type, uint16_t code, int32_t value) {
    auto typePair = map.find(type);
    if (typePair == map.end()) return 0;
    if (typePair->second.event > 0) return typePair->second.event;

    auto codePair = typePair->second.map.find(code);
    if (codePair == typePair->second.map.end()) return 0;
    if (codePair->second.event > 0) return codePair->second.event;

    auto valuePair = codePair->second.map.find(value);
    if (valuePair == codePair->second.map.end()) return 0;
    if (valuePair->second > 0) return valuePair->second;
    return valuePair->second;
}



Device::Device(const char* eventPath) :
    eventPath_(eventPath),
    fd_(0),
    open_(false),
    buffer_{}
{
    openHandler();
}

void Device::addEvent(size_t event, uint16_t type) {
    eventMap_.loadEvent(event, type);
}
void Device::addEvent(size_t event, uint16_t type, uint16_t code) {
    eventMap_.loadEvent(event, type, code);
}
void Device::addEvent(size_t event, uint16_t type, uint16_t code, int32_t value) {
    eventMap_.loadEvent(event, type, code, value);
}

bool Device::openHandler() {
    fd_ = open(eventPath_, O_RDONLY);
    if (fd_ < 0) return false;
    ioctl(fd_, EVIOCGRAB, 1);
    open_ = true;
    return true;
}

bool Device::read(size_t &event) {
    if (!open_) return false;
    input_event buffer;
    ssize_t n = ::read(fd_, &buffer, sizeof(buffer));
    if (n < sizeof(buffer)) return false;
    if (buffer.type == EV_SYN) return false;
    event = eventMap_.event(buffer.type, buffer.code, buffer.value);
    return true;
}

bool Device::readRaw(input_event &event) {
    if (open_) {
        ssize_t n = ::read(fd_, &event, sizeof(event));
        if (n < sizeof(event)) return false;
        return true;
    }
}