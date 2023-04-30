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

    struct EventMap {
        std::vector<std::tuple<std::string,
            std::string,BaseInstruction>> map;
    };

    struct BaseLayer {
        std::string type;
        std::string name;
        EventMap events;
    };

    struct BaseProfile {
        std::string name;
        std::vector<std::string> devices;
        std::vector<BaseInstruction> init;
    };

    struct DeviceHandle {
        std::string name;
        std::vector<std::pair<std::string, std::vector<std::string>>> handledEvents;
    };

    struct DeviceHandleInstance {
        std::string handle;
        std::string handlePath;
        std::vector<std::tuple<std::string, std::string, unsigned short>> events;
    };

    struct DeviceInstance {
        std::string name;
        std::vector<DeviceHandleInstance> handles;
        
    };

    struct BaseDevice {
        char* name;
        std::vector<DeviceHandle> handles;
        std::vector<DeviceInstance> instances;
    };

}

// Data Format specific serialization files
#include <serial/serial_zzz.hpp>


// namespace serial {

//     // Layer Deserialization and constructor using generic data format

//     class ProfileConstructor {
//         std::vector<BaseLayer> layers_;
//         BaseProfile profile_;

//         std::map<std::string, BaseLayer> layers_;

//         static map<std::string, > validInstructions;

//         public:

//         BaseLayer &getLayer();
//         BaseProfile &getProfile() {return profile_;}

//         ProfileConstructor(const char* profileFilePath);

//         Profile finalizeProfile();


//     };

// }