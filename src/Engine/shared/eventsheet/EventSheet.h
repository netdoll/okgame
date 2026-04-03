#pragma once
#include <string>
#include <vector>
#include <map>
#include <any>

namespace OKGame {
    struct EventCondition {
        std::string type;
        std::map<std::string, std::any> params;
    };

    struct EventAction {
        std::string type;
        std::map<std::string, std::any> params;
    };

    struct EventBlock {
        std::vector<EventCondition> conditions;
        std::vector<EventAction> actions;
        std::vector<EventBlock> subEvents;
    };

    struct EventSheet {
        std::string name = "New Event Sheet";
        std::vector<EventBlock> blocks;
    };
}
