#include "device.hpp"

#include <fcntl.h>
#include <unistd.h>

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

void Device::addEvent(size_t event, EventDescriptor ed) {
    eventMap_[ed] = event;
}

bool Device::openHandler() {
    fd_ = open(eventPath_, O_RDONLY);
    if (fd_ < 0) return false;
    ioctl(fd_, EVIOCGRAB, 1);
    open_ = true;
    return true;
}

bool Device::read(size_t &event) {
    if (open_) {
        input_event buffer;
        ssize_t n = ::read(fd_, &buffer, sizeof(buffer));
        if (n < sizeof(buffer)) return false;
        EventDescriptor ed{buffer.type, buffer.code, buffer.value};
        if (ed.type != 1) return false;
        event = eventMap_[ed];
        return true;
    }
}

bool Device::readRaw(input_event &event) {
    if (open_) {
        ssize_t n = ::read(fd_, &event, sizeof(event));
        if (n < sizeof(event)) return false;
        return true;
    }
}