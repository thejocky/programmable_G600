
#include <deserialize.hpp>

namespace serial {

    BaseLayer &ProfileConstructor::getLayer() {
        layers_.resize(layers_.size()+1);
        return layers_.back();
    }

    ProfileConstructor::ProfileConstructor(const char* profileFilePath) {

    }

}