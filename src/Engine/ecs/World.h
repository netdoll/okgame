#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "Entity.h"
#include "Component.h"
#include "System.h"

namespace OKGame {
    class World {
    private:
        std::map<EntityId, std::map<std::string, std::shared_ptr<Component>>> entities;
        std::vector<std::shared_ptr<System>> systems;

    public:
        EntityId createEntity() {
            Entity entity;
            entities[entity.id] = std::map<std::string, std::shared_ptr<Component>>();
            return entity.id;
        }

        void addComponent(EntityId entityId, std::shared_ptr<Component> component) {
            if (entities.count(entityId)) {
                component->entityId = entityId;
                entities[entityId][component->getTypeName()] = component;
            }
        }

        template<typename T>
        std::shared_ptr<T> getComponent(EntityId entityId, const std::string& typeName) {
            if (entities.count(entityId) && entities[entityId].count(typeName)) {
                return std::static_pointer_cast<T>(entities[entityId][typeName]);
            }
            return nullptr;
        }

        void addSystem(std::shared_ptr<System> system) {
            systems.push_back(system);
        }

        void update(float dt) {
            for (auto& system : systems) {
                system->update(dt, entities);
            }
        }
    };
}
