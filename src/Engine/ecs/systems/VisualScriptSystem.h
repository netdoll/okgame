#pragma once
#include "../System.h"
#include "../components/EventSheetComponent.h"
#include <iostream>

namespace OKGame {
    class VisualScriptSystem : public System {
    public:
        void update(float dt, std::map<EntityId, std::map<std::string, std::shared_ptr<Component>>>& entities) override {
            for (auto& [id, components] : entities) {
                if (components.count("EventSheet")) {
                    auto esComp = std::static_pointer_cast<EventSheetComponent>(components["EventSheet"]);
                    for (auto& sheet : esComp->eventSheets) {
                        for (auto& block : sheet.blocks) {
                            if (checkConditions(block, esComp)) {
                                runActions(block, esComp);
                            }
                        }
                    }
                }
            }
        }

    private:
        bool checkConditions(const EventBlock& block, std::shared_ptr<EventSheetComponent> comp) {
            for (const auto& cond : block.conditions) {
                if (cond.type == "Always") continue;
                // Add more logic
            }
            return true;
        }

        void runActions(const EventBlock& block, std::shared_ptr<EventSheetComponent> comp) {
            for (const auto& action : block.actions) {
                if (action.type == "Log") {
                    // Note: need a way to print any value
                    std::cout << "[ECS Event] Log Action" << std::endl;
                }
            }
        }
    };
}
