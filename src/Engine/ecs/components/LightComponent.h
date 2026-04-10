#pragma once
#include "../Component.h"

namespace OKGame {
    class LightComponent : public Component {
    public:
        std::string getTypeName() override { return "Light"; }
        float radius = 100.0f;
        int color = 0xffffff;
        float intensity = 1.0f;
        bool flicker = false;
        float flickerSpeed = 5.0f;
        float baseRadius = 100.0f;
    };
}
