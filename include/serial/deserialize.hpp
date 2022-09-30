#pragma once

#include <serial/serial_zzz.hpp>

#include <map>

#include <zzzlib/zzz.hpp>
#include <cstring>
#include <cstdlib>
#include <utility>





// structures used for intermediary stages in desserialization

namespace serial {

    struct BaseInstruction {
        char* name;
        BaseInstruction* argv;
        int32_t argc;
    };

    struct EventKey {
        char* state;
        char* key;
        bool operator<(const EventKey& other) {
            if (int8_t val = strcmp(key, other.key))
                return val < 0;
            return strcmp(state, other.state) < 0;
        }
    };

    struct EventMap {
        std::map<EventKey, BaseInstruction> map;
    };

    struct BaseLayer {
        char* type;
        char* name;
        EventMap events;
    };

    struct BaseDevice {
        char* name;
        char* ID;
        char* deviceName;
        std::pair<std::string,uint16_t> events[7];
    };

}


