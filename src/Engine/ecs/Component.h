#pragma once
#include <string>

namespace OKGame {
    class Component {
    public:
        int entityId = -1;
        virtual std::string getTypeName() = 0;
        virtual ~Component() {}
    };
}
