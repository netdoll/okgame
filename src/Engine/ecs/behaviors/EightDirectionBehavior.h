#pragma once
#include "Behavior.h"
#include "../World.h"
#include "../components/TransformComponent.h"
#include "../../Utility/ControlsManager.h"
#include <cmath>

namespace OKGame {
    class EightDirectionBehavior : public Behavior {
    private:
        World* world;
        float speed = 200.0f;

    public:
        EightDirectionBehavior(World* w) : world(w) {}

        void onInit() override {}

        void onUpdate(float dt) override {
            auto transform = world->getComponent<TransformComponent>(entityId, "Transform");
            if (!transform) return;

            float dx = 0;
            float dy = 0;

            if (ControlsManager::isLeftHeld()) dx -= 1.0f;
            if (ControlsManager::isRightHeld()) dx += 1.0f;
            if (ControlsManager::isUpHeld()) dy -= 1.0f;
            if (ControlsManager::isDownHeld()) dy += 1.0f;

            if (dx != 0 || dy != 0) {
                float mag = std::sqrt(dx * dx + dy * dy);
                transform->x += (dx / mag) * speed * dt;
                transform->y += (dy / mag) * speed * dt;
            }
        }
    };
}
