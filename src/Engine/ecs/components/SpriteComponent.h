#pragma once
#include "../Component.h"
#include "../../entity/Sprite.h"
#include <memory>

namespace OKGame {
    class SpriteComponent : public Component {
    public:
        std::shared_ptr<Sprite> sprite = nullptr;
        std::string assetId = "";
        bool visible = true;
        float alpha = 1.0f;

        std::string getTypeName() override { return "Sprite"; }
    };
}
