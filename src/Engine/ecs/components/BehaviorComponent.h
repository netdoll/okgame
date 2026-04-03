#pragma once
#include "../Component.h"
#include "../behaviors/Behavior.h"
#include <vector>
#include <memory>

namespace OKGame {
    class BehaviorComponent : public Component {
    public:
        std::vector<std::shared_ptr<Behavior>> behaviors;
        std::string getTypeName() override { return "Behavior"; }
    };
}
