#pragma once

namespace OKGame {
    typedef int EntityId;

    class Entity {
    public:
        const EntityId id;
        static EntityId nextId;

        Entity() : id(++nextId) {}
    };
}
