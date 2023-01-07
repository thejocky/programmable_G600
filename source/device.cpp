#include "device.hpp"

#include <fcntl.h>
#include <unistd.h>


void EventMap::loadEvent(int event, uint16_t type) {
    if (!event) throw;
    map[type].event = event;
}
void EventMap::loadEvent(int event, uint16_t type, uint16_t code) {
    if (!event) throw;
    map[type][code].event = event;
}
void EventMap::loadEvent(int event, uint16_t type, uint16_t code, int32_t value) {
    if (!event) throw;
    map[type][code][value].event = event;
}


int EventMap::event(uint16_t type, uint16_t code, int32_t value) {
    Node *current = &map[type];
    if (current.event) return current.event;
    current = &(*current)[code];
    if (current.event) return current.event;
    current = &(*current)[code];
    if (current.event) return current.event;
    throw; // Error: map shouldn't under normal working conditions
                  // access a non-loaded event
}



Device::Device(const char* eventPath) :
    eventPath_(eventPath),
    fd_(0),
    open_(false),
    buffer_{}
{
    openHandler();
}

Device::Device(const char* eventPath, std::map<EventDescriptor, size_t> &eventMap) :
    eventPath_(eventPath),
    fd_(0),
    open_(false),
    buffer_{},
    eventMap_(eventMap)
{
    openHandler();
}

void Device::addEvent(size_t event, uint16_t type) {
    eventMap_.loadEvent(type);
}
void Device::addEvent(size_t event, uint16_t type, uint16_t code) {
    eventMap_.loadEvent(type, code);
}
void Device::addEvent(size_t event, uint16_t type, uint16_t code, int32_t value) {
    eventMap_.loadEvent(type, code, value);
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
    if (event.type == EV_SYN) return false;
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