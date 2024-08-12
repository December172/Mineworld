#ifndef FILE_ENTITY_ENTITY_H
#define FILE_ENTITY_ENTITY_H

#include <string>

#include "world/World.h"
#include "data/DataTag.h"

namespace Mineworld {
    class World;
    class Entity {
    public:
        Entity(const std::string& id, World& world);
        Entity(const std::string& id, World& world, DataTag dataTag);
        virtual void tick() = 0;
        virtual void render() = 0;
        virtual ~Entity();
        Entity& opeator= (const Entity& other) = delete;
    protected:
    private:
        std::shared_ptr<DataTag> dataTag;
    };
}

#endif // FILE_ENTITY_ENTITY_H