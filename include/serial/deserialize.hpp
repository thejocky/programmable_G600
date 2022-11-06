#pragma once




#include <cstring>
#include <cstdlib>
#include <utility>
#include <map>
#include <string>
// Including Data Format specific serialization headers below struct declarations




// structures used for intermediary stages in desserialization

namespace serial {

    struct BaseInstruction {
        char* name;
        BaseInstruction* argv;
        int32_t argc;
    };

    // // Contains 2 keys, one for key and one for state
    // struct EventKey {
    //     std::string keyA;
    //     std::string keyB;
    //     bool operator<(const EventKey& other) {
    //         if (int8_t val = keyA.compare(other.keyA))
    //             return val < 0;
    //         return keyB.compare(other.keyB) < 0;
    //     }
    // };

    struct EventMap {
        std::vector<std::tuple<std::string,
            std::string,BaseInstruction>> map;
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

// Data Format specific serialization files
#include <serial/serial_zzz.hpp>


