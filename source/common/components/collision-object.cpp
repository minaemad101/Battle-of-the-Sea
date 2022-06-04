#include "collision-object.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our
{
    void CollisionObject::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        position = data.value("position", glm::vec3(0.0f, 0.0f, 0.0f));
        length = data.value("length", 0.0f);
    }
}