#pragma once
#include "../Component.h"
#include <string>
#include <vector>

namespace OKGame {
    struct Quest {
        std::string id;
        std::string title;
        std::string description;
        std::string status; // "inactive", "active", "completed"
        int objectiveCount;
        int currentObjective;
    };

    class QuestComponent : public Component {
    public:
        std::string getTypeName() override { return "Quest"; }
        std::vector<Quest> quests;
    };
}
