#pragma once

#include <zzzlib/zzz.hpp>

#include <serial/deserialize.hpp>

#include <iostream>
#include <exception>

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
            throw; // TODO: Have warning in missing second key
        }
        for (Node* keyNodeA = objRoot->firstChild(); keyNodeA;
                keyNodeA = keyNodeA->sibling()) {
            
            if (!keyNodeA->childCount())
                break; // TODO: Have warning in missing second key  
            
            for (Node* keyNodeB = keyNodeA->firstChild(); keyNodeB;
                    keyNodeB = keyNodeB->sibling()) {
                        
                if (!keyNodeA->childCount())
                    break; // TODO: Have warning in missing second key 
                object.map.push_back(std::make_tuple(
                    keyNodeA->string(), keyNodeB->string(),
                    deserialize<serial::BaseInstruction>(keyNodeB->firstChild())
                ));
                // TODO: Add suport for multiple commands or have error
            }
        }
    }

    void deserialize(serial::BaseLayer& object, Node* objRoot) {
        if (!objRoot) return;
        if (objRoot->childCount() != 2) {
            throw; // TODO: Have warning incorrect number of arguments
        }
        // Read in layer name
        object.type = objRoot->string();
        object.name, objRoot->firstChild()->string();
        deserialize(object.events, objRoot->child(1));
    }

    void deserialize(serial::DeviceHandle& object, Node* objRoot) {
        if (!objRoot) throw;
        if (objRoot->childCount() == 0)  throw;
        object.name = objRoot->string();
        for (Node* node = objRoot->firstChild(); node = node->sibling(); node) {
            object.handledEvents.push_back(std::make_pair(
                node->string(),
                deserialize<std::vector<std::string>>(node)
            ));
        }
    }

    void deserialize(serial::DeviceHandleInstance& object, Node* objRoot) {
        if (!objRoot) throw;
        if (objRoot->childCount() < 2)  throw;
        object.handle = objRoot->string();
        object.handlePath = objRoot->firstChild()->string();
        for (auto ENV = objRoot->child(1); ENV = ENV->sibling(); ENV) {
            for (auto event = ENV->firstChild(); event = event->sibling(); event) {
                if (event->childCount() != 1)  throw;
                object.events.push_back(std::make_tuple(
                    ENV->string(), event->string(),
                    deserialize<unsigned int>(event)
                ));
            }
        } 
    }

    void deserialize(serial::DeviceInstance& object, Node* objRoot) {
        if (!objRoot) throw;
        object.name = objRoot->string();
        deserialize(object.handles, objRoot);
    }

    void deserialize(serial::BaseDevice& object, Node* objRoot) {
        if (!objRoot) throw;
        Node* node;

        if (!(node = objRoot->child("Handles")))  throw;
        deserialize(object.handles, node);

        if (!(node = objRoot->child("Instances")))  throw;
        deserialize(object.instances, node);
        
    }
}




