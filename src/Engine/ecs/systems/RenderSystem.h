#pragma once
#include "../System.h"
#include "../components/SpriteComponent.h"
#include "../components/TransformComponent.h"

namespace OKGame {
    class RenderSystem : public System {
    public:
        void update(float dt, std::map<EntityId, std::map<std::string, std::shared_ptr<Component>>>& entities) override {
            for (auto& [id, components] : entities) {
                if (components.count("Transform") && components.count("Sprite")) {
                    auto transform = std::static_pointer_cast<TransformComponent>(components["Transform"]);
                    auto spriteComp = std::static_pointer_cast<SpriteComponent>(components["Sprite"]);

                    if (spriteComp->sprite && spriteComp->visible) {
                        spriteComp->sprite->setX(transform->x);
                        spriteComp->sprite->setY(transform->y);
                        spriteComp->sprite->setRotation(transform->rotation);
                        spriteComp->sprite->setScaleX(transform->scaleX);
                        spriteComp->sprite->setScaleY(transform->scaleY);
                        spriteComp->sprite->setAlpha(spriteComp->alpha);
                        
                        // Note: actual drawing would happen in the engine's main render loop
                        // but this updates the sprite's internal state.
                    }
                }
            }
        }
    };
}
