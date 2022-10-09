#pragma once



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

    // Contains 2 keys, one for key and one for state
    struct EventKey {
        char* keyA;
        char* keyB;
        bool operator<(const EventKey& other) {
            if (int8_t val = strcmp(keyA, other.keyA))
                return val < 0;
            return strcmp(keyB, other.keyB) < 0;
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


#include <serial/serial_zzz.hpp>