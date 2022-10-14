#pragma once

#include <zzzlib/zzz.hpp>

#include <serial/deserialize.hpp>

#include <iostream>

namespace zzz {
    
    void deserialize(serial::BaseInstruction& object, Node* objRoot) {
        if (!objRoot) return;
        object.name = strcpy(new char[objRoot->string().length()],
                            objRoot->string().c_str());
        object.argc = objRoot->childCount();
        object.argv = new serial::BaseInstruction[object.argc+1];
        for (int i = 0; i < object.argc; i++) {
            deserialize(object.argv[i], objRoot->child(i));
        }
    }

    void deserialize(serial::EventMap& object, Node* objRoot) {
        if (!objRoot) return;
        if (!objRoot->childCount()) {
            return; // TODO: Have warning in missing second key
        }
        for (Node* keyNodeA = objRoot->firstChild(); keyNodeA;
                keyNodeA = keyNodeA->sibling()) {
            
            if (!keyNodeA->childCount())
                break; // TODO: Have warning in missing second key  
            
            for (Node* keyNodeB = objRoot->firstChild(); keyNodeB;
                    keyNodeB = keyNodeB->sibling()) {
                        
                if (!keyNodeA->childCount())
                    break; // TODO: Have warning in missing second key 
                object.map.push_back(std::make_tuple(
                    keyNodeA->string(), keyNodeB->string(),
                    deserialize<serial::BaseInstruction>(keyNodeB->firstChild())
                ));
            }
        }
    }

}


