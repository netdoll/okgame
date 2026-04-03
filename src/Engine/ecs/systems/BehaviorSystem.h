#pragma once
#include "../System.h"
#include "../components/BehaviorComponent.h"

namespace OKGame {
    class BehaviorSystem : public System {
    public:
        void update(float dt, std::map<EntityId, std::map<std::string, std::shared_ptr<Component>>>& entities) override {
            for (auto& [id, components] : entities) {
                if (components.count("Behavior")) {
                    auto behaviorComp = std::static_pointer_cast<BehaviorComponent>(components["Behavior"]);
                    for (auto& behavior : behaviorComp->behaviors) {
                        behavior->onUpdate(dt);
                    }
                }
            }
        }
    };
}
