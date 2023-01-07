#include "device.hpp"
#include "profile.hpp"

#include <zzzlib/zzz.hpp>
#include <serial/deserialize.hpp>


void printDeleteInst(serial::BaseInstruction* inst, int depth=0) {
    for (int i = 0; i < depth; i++) std::cout << "  ";
    std::cout << inst->name << "\n";
    for (int i = 0; i < inst->argc; i++) 
        printDeleteInst(&(inst->argv[i]), depth+1);
    delete[] inst->argv;
    inst->argc = 0;
}


void printDeleteMap(serial::EventMap& events, int depth=0) {
    std::cout << events.map.size() << "\n";
    for (int i = 0; i < events.map.size(); i++) {
        std::cout << std::get<0>(events.map[i]) << ": "
            << std::get<1>(events.map[i]) << "\n";
        printDeleteInst(&std::get<2>(events.map[i]), 1);
    }
}


int main() {

    Device g600Device("/dev/input/by-id/usb-Logitech_Gaming_Mouse_G600_A660D9B71EF20017-if01-event-kbd");

    // Button 1
    g600Device.addEvent(0, {1, 2, 1}); // Press
    g600Device.addEvent(1, {1, 2, 0}); // Release
    g600Device.addEvent(2, {1, 2, 2}); // Repeat

    // Button 2
    g600Device.addEvent(3, {1, 3, 1}); // Press
    g600Device.addEvent(4, {1, 3, 0}); // Release
    g600Device.addEvent(5, {1, 3, 2}); // Repeat

    // Button 3
    g600Device.addEvent(6, {1, 4, 1}); // Press
    g600Device.addEvent(7, {1, 4, 0}); // Release
    g600Device.addEvent(8, {1, 4, 2}); // Repeat

    // Button 4
    g600Device.addEvent(9, {1, 5, 1}); // Press
    g600Device.addEvent(10, {1, 5, 0}); // Release
    g600Device.addEvent(11, {1, 5, 2}); // Repeat   
    
    // Button 5
    g600Device.addEvent(12, {1, 6, 1}); // Press
    g600Device.addEvent(13, {1, 6, 0}); // Release
    g600Device.addEvent(14, {1, 6, 2}); // Repeat

    // Button 6
    g600Device.addEvent(15, {1, 7, 1}); // Press
    g600Device.addEvent(16, {1, 7, 0}); // Release
    g600Device.addEvent(17, {1, 7, 2}); // Repeat



    Profile g600Profile;
    LayerLink* layer1 = LayerLink::generateLayerLink(18);
    LayerLink* layer2 = LayerLink::generateLayerLink(18);
    LayerLink* layer3 = LayerLink::generateLayerLink(18);
    LayerLink* layer4 = LayerLink::generateLayerLink(18);
    LayerLink* layer5 = LayerLink::generateLayerLink(18);

    // Layer 1 ----------------------------------------------------------------

    // Layer 1 Button 1 press, release, and repeat
    layer1->setEvent(0, "xdotool key a",         EventType::ExecuteCommand);
    layer1->setEvent(1, "xdotool key c",         EventType::ExecuteCommand);
    layer1->setEvent(2, "xdotool key b",         EventType::ExecuteCommand);

    // Layer 1 Button 2 press, release, and repeat
    layer1->setEvent(3, "xdotool key b",         EventType::ExecuteCommand);
    layer1->setEvent(4, "",                      EventType::NoEvent);
    layer1->setEvent(5, "",                      EventType::NoEvent);

    // Layer 1 Button 3 press, release, and repeat
    layer1->setEvent(6, "xdotool key c",         EventType::ExecuteCommand);
    layer1->setEvent(7, "",                      EventType::NoEvent);
    layer1->setEvent(8, "",                      EventType::NoEvent);

    // Layer 1 Button 4 press, release, and repeat
    layer1->setEvent(9, "xdotool keydown shift", EventType::ExecuteCommand);
    layer1->setEvent(10, layer3,                 EventType::ForwardLayer);
    layer1->setEvent(11, "",                     EventType::NoEvent);

    // Layer 1 Button 5 press, release, and repeat
    layer1->setEvent(12, "xdotool keydown d",    EventType::ExecuteCommand);
    layer1->setEvent(13, "xdotool keyup d",      EventType::ExecuteCommand);
    layer1->setEvent(14, "",                     EventType::NoEvent);

    // Layer 1 Button 6 press, release, and repeat
    layer1->setEvent(15, layer2,                 EventType::ForwardLayer);
    layer1->setEvent(16, "",                     EventType::NoEvent);
    layer1->setEvent(17, "",                     EventType::NoEvent);

    // layer 2 ----------------------------------------------------------------

    // Layer 2 Button 1 press, release, and repeat
    layer2->setEvent(0, layer1,                  EventType::ForwardLayer);
    layer2->setEvent(1, "",                      EventType::NoEvent);
    layer2->setEvent(2, "",                      EventType::NoEvent);
    
    // Layer 2 Button 2 press, release, and repeat
    layer2->setEvent(3, "xdotool keydown e",     EventType::ExecuteCommand);
    layer2->setEvent(4, "xdotool keyup e",       EventType::ExecuteCommand);
    layer2->setEvent(5, "",                      EventType::NoEvent);

    // Layer 2 Button 3 press, release, and repeat
    layer2->setEvent(6, "",                      EventType::NoEvent);
    layer2->setEvent(7, "xdotool key f",         EventType::ExecuteCommand);
    layer2->setEvent(8, "",                      EventType::NoEvent);

    // Layer 2 Button 4 press, release, and repeat
    layer2->setEvent(9, "",                     EventType::NoEvent);
    layer2->setEvent(10, "",                     EventType::NoEvent);
    layer2->setEvent(11, "xdotool key g",        EventType::ExecuteCommand);

    // Layer 2 Button 5 press, release, and repeat
    layer2->setEvent(12, "xdotool key h",        EventType::ExecuteCommand);
    layer2->setEvent(13, "xdotool key g",        EventType::ExecuteCommand);
    layer2->setEvent(14, "xdotool key i",        EventType::ExecuteCommand);

    // Layer 2 Button 6 press, release, and repeat
    layer2->setEvent(15, "xdotool key a",        EventType::ExecuteCommand);
    layer2->setEvent(16, layer4,                 EventType::ForwardLayer);
    layer2->setEvent(17, "",                     EventType::NoEvent);

    // Layer 3 ----------------------------------------------------------------

    // Layer 3 Button 1 press, release, and repeat
    layer3->setEvent(0, "",                      EventType::EmptyEvent);
    layer3->setEvent(1, "",                      EventType::EmptyEvent);
    layer3->setEvent(2, "",                      EventType::EmptyEvent);

    // Layer 3 Button 2 press, release, and repeat
    layer3->setEvent(3, "",                      EventType::EmptyEvent);
    layer3->setEvent(4, "",                      EventType::EmptyEvent);
    layer3->setEvent(5, "",                      EventType::EmptyEvent);

    // Layer 3 Button 3 press, release, and repeat
    layer3->setEvent(6, "",                      EventType::EmptyEvent);
    layer3->setEvent(7, "",                      EventType::EmptyEvent);
    layer3->setEvent(8, "",                      EventType::EmptyEvent);

    // Layer 3 Button 4 press, release, and repeat
    layer3->setEvent(9, "xdotool keyup shift",   EventType::ExecuteCommand);
    layer3->setEvent(10, layer1,                 EventType::ForwardLayer);
    layer3->setEvent(11, "",                     EventType::NoEvent);

    // Layer 3 Button 5 press, release, and repeat
    layer3->setEvent(12, "",                     EventType::EmptyEvent);
    layer3->setEvent(13, "",                     EventType::EmptyEvent);
    layer3->setEvent(14, "",                     EventType::EmptyEvent);

    // Layer 3 Button 6 press, release, and repeat
    layer3->setEvent(15, "",                     EventType::EmptyEvent);
    layer3->setEvent(16, "",                     EventType::EmptyEvent);
    layer3->setEvent(17, "",                     EventType::EmptyEvent);

    // Layer 4 ----------------------------------------------------------------

    // Layer 4 Button 1 press, release, and repeat
    layer4->setEvent(0, "",                     EventType::EmptyEvent);
    layer4->setEvent(1, "",                     EventType::EmptyEvent);
    layer4->setEvent(2, "",                     EventType::EmptyEvent);

    // Layer 4 Button 2 press, release, and repeat
    layer4->setEvent(3, "",                     EventType::EmptyEvent);
    layer4->setEvent(4, "",                     EventType::EmptyEvent);
    layer4->setEvent(5, "",                     EventType::EmptyEvent);

    // Layer 4 Button 3 press, release, and repeat
    layer4->setEvent(6, "",                     EventType::EmptyEvent);
    layer4->setEvent(7, "",                     EventType::EmptyEvent);
    layer4->setEvent(8, "",                     EventType::EmptyEvent);

    // Layer 4 Button 4 press, release, and repeat
    layer4->setEvent(9, "",                     EventType::EmptyEvent);
    layer4->setEvent(10, "",                    EventType::EmptyEvent);
    layer4->setEvent(11, "",                    EventType::EmptyEvent);

    // Layer 4 Button 5 press, release, and repeat
    layer4->setEvent(12, "",                    EventType::EmptyEvent);
    layer4->setEvent(13, "",                    EventType::EmptyEvent);
    layer4->setEvent(14, "",                    EventType::EmptyEvent);

    // Layer 4 Button 6 press, release, and repeat
    layer4->setEvent(15, "xdotool key b",       EventType::ExecuteCommand);
    layer4->setEvent(16, layer2,                EventType::ForwardLayer);
    layer4->setEvent(17, "",                    EventType::EmptyEvent);

    g600Profile.appendLayer(layer4);
    g600Profile.appendLayer(layer3);
    g600Profile.appendLayer(layer2);
    g600Profile.appendLayer(layer1);

    // size_t event;
    // while (true) {
    //     if (g600Device.read(event)) {
    //         std::cout << "Event Read: " << event << "\n";
    //         g600Profile.executeEvent(event);
    //     }
    // }


    char* zzzInstruction = 
    "# Home layer for quickly swapping between layers\n"
    "Layer: homeLayerA:\n"
    "Events:\n"
    "    GShift:press : push:homeLayerB\n"
        
    "    G7:press     : pop:homeLayerB\n"
    "    release    : pop:homeLayerA\n"

    "    press:\n"
    "    G9  : push: stellaris\n"
    "    G10 : push: Factorio\n"
    "    G11 : push: minecraft\n"
    "    G12 : push: media\n";


    serial::BaseLayer layerTest = {
        "Layer", "homeLayerA",
        {{
            {"GShift", "press", {"push", new serial::BaseInstruction{"HomeLayerB", nullptr, 0}, 1}},
            {"G7", "press", {"pop", new serial::BaseInstruction{"HomeLayerB", nullptr, 0}, 1}},
            {"G7", "press", {"pop", new serial::BaseInstruction{"HomeLayerA", nullptr, 0}, 1}},
            {"press", "G9", {"push", new serial::BaseInstruction{"stellaris", nullptr, 0}, 1}},
            {"press", "G10", {"push", new serial::BaseInstruction{"Factorio", nullptr, 0}, 1}},
            {"press", "G11", {"push", new serial::BaseInstruction{"minecraft", nullptr, 0}, 1}},
            {"press", "G12", {"push", new serial::BaseInstruction{"media", nullptr, 0}, 1}}
        }}
    };
    
    zzz::Node* root = new zzz::Node("", nullptr);
    zzz::Parser parser(root);
    std::stringstream stream(zzzInstruction);
    parser.parse(stream);

    zzz::printNode(root);
    

    // deserialize(map, root->lastChild());
    // printDeleteMap(map);
    

    return 0;
}

