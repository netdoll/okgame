#pragma once
#include <map>
#include <string>
#include <memory>
#include "Entity.h"
#include "Component.h"

namespace OKGame {
    class System {
    public:
        virtual void update(float dt, std::map<EntityId, std::map<std::string, std::shared_ptr<Component>>>& entities) = 0;
        virtual ~System() {}
    };
}
