#pragma once
#include "../Component.h"

namespace OKGame {
    class Behavior : public Component {
    public:
        virtual void onInit() = 0;
        virtual void onUpdate(float dt) = 0;
        std::string getTypeName() override { return "Behavior"; }
    };
}
