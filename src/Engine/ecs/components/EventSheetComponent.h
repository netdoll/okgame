#pragma once
#include "../Component.h"
#include "../../shared/eventsheet/EventSheet.h"
#include <vector>
#include <map>
#include <any>

namespace OKGame {
    class EventSheetComponent : public Component {
    public:
        std::string getTypeName() override { return "EventSheet"; }
        std::vector<EventSheet> eventSheets;
        std::map<std::string, std::any> variables;
    };
}
