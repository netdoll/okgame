#pragma once
#include "../Component.h"

namespace OKGame {
    class CombatComponent : public Component {
    public:
        std::string getTypeName() override { return "Combat"; }
        float hp = 100.0f;
        float maxHp = 100.0f;
        float atk = 10.0f;
        float def = 5.0f;
        bool isPlayer = false;
        bool isDead = false;
    };
}
