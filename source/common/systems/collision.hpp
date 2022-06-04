#pragma once

#include "../ecs/world.hpp"
#include "../components/collision-object.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    class CollisionSystem
    {
        std::vector<CollisionObject *> missles, enemies, boats;

    public:
        bool update(World *world, float deltaTime)
        {
            missles.clear();
            enemies.clear();
            boats.clear();
            for (const auto &entity : world->getEntities())
                if (auto missle = entity->getComponent<CollisionObject>(); missle && entity->name == "missle")
                    missles.push_back(missle);
                else if (auto enemy = entity->getComponent<CollisionObject>(); enemy && entity->name == "enemy")
                    enemies.push_back(enemy);
                else if (auto boat = entity->getComponent<CollisionObject>(); boat && entity->name == "boat")
                    boats.push_back(boat);

            for (auto missle : missles)
            {
                for (auto enemy : enemies)
                    if (checkCollision(missle, enemy))
                    {
                        MeshRendererComponent *Mesh = enemy->getOwner()->getComponent<MeshRendererComponent>();
                        world->markForRemoval(enemy->getOwner());
                        MovementComponent *movement = missle->getOwner()->getComponent<MovementComponent>();
                        movement->linearVelocity = {0, 0, 0};
                        missle->getOwner()->localTransform.position = {0, -1, -2};
                    }

                glm::vec3 missleCenter = missle->position + glm::vec3(missle->getOwner()->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

                if (missleCenter.x > 11 || missleCenter.x < -11 || missleCenter.y > 11 || missleCenter.y < -11 || missleCenter.z > 11 || missleCenter.z < -11)
                {
                    MovementComponent *movement = missle->getOwner()->getComponent<MovementComponent>();
                    movement->linearVelocity = {0, 0, 0};
                    missle->getOwner()->localTransform.position = {0, -1, -2};
                }
            }

            for (auto boat : boats)
            {
                for (auto enemy : enemies)
                    if (checkCollision(missle, enemy))
                        return true;
            }

            world->deleteMarkedEntities();

            return false;
        }

        bool checkCollision(CollisionObject *missleComponent, CollisionObject *enemyComponent)
        {
            auto missle = missleComponent->getOwner();
            auto enemy = enemyComponent->getOwner();

            glm::vec3 missleCenter = missleComponent->position + glm::vec3(bullet->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            glm::vec3 enemyCenter = enemyComponent->position + glm::vec3(enemy->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            glm::vec3 enemyLenghts = glm::vec3(enemyComponent->length, enemyComponent->length, enemyComponent->length) * enemy->localTransform.scale;

            glm::vec3 difference = missleCenter - enemyCenter;
            glm::vec3 clampedDifference = glm::clamp(difference, enemyLenghts * glm::vec3(-1.0f, -1.0f, -1.0f), enemyLenghts);

            glm::vec3 enemyPoint = enemyCenter + clampedDifference;
            difference = enemyPoint - missleCenter;
            float missleRadius = missleComponent->length * glm::length(missle->localTransform.scale);
            return glm::length(difference) < missleRadius;
        }
    };

}