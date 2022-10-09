#pragma once

#include <zzzlib/zzz.hpp>

#include <serial/deserialize.hpp>


namespace zzz {
    
    void deserialize(serial::BaseInstruction& object, Node* objRoot) {
        object.name = strcpy(new char[objRoot->string().length()],
                            objRoot->string().c_str());
        object.argc = objRoot->childCount();
        object.argv = new serial::BaseInstruction[object.argc+1];
        for (int i = 0; i < object.argc; i++) {
            deserialize(object.argv[i], objRoot->child(i));
        }
    }

    void deserialize() {
        
    }

}


