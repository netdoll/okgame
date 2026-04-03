#pragma once
#include "../Component.h"

namespace OKGame {
    class TransformComponent : public Component {
    public:
        float x = 0;
        float y = 0;
        float rotation = 0;
        float scaleX = 1;
        float scaleY = 1;

        std::string getTypeName() override { return "Transform"; }
    };
}
