#pragma once

#include "../ecs/world.hpp"
#include "../components/collision-object.hpp"
#include "../components/movement.hpp"
#include "../components/mesh-renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

namespace our
{

    class CollisionSystem
    {
        std::vector<CollisionObject *> missles, enemies, boats;

    public:
        bool update(World *world, float deltaTime)
        {
            std::cout<< "entered collision function"<<std::endl;
            missles.clear();
            enemies.clear();
            boats.clear();
            for (auto entity : world->getEntities())
                { 
                CollisionObject *objct = entity->getComponent<CollisionObject>();

                    if (objct && entity->name == "missle")
                        missles.push_back(objct);
                    else if (objct && entity->name == "enemy")
                        enemies.push_back(objct);
                    else if (objct && entity->name == "boat")
                        boats.push_back(objct);
                }

            std::cout<< enemies.size()<<std::endl;



            

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
                    if (checkCollision(boat, enemy))
                        return true;
            }

            world->deleteMarkedEntities();

            return false;
        }

        bool checkCollision(CollisionObject *missleComponent, CollisionObject *enemyComponent)
        {
            std::cout<< "entered check position "<<std::endl;

            auto missle = missleComponent->getOwner();
            auto enemy = enemyComponent->getOwner();

            glm::vec3 missleCenter = missleComponent->position + glm::vec3(missle->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            glm::vec3 enemyCenter = enemyComponent->position + glm::vec3(enemy->getLocalToWorldMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
            glm::vec3 enemyLenghts = glm::vec3(enemyComponent->length, enemyComponent->length, enemyComponent->length) * enemy->localTransform.scale;

            glm::vec3 difference = missleCenter - enemyCenter;
            glm::vec3 clampedDifference = glm::clamp(difference, enemyLenghts * glm::vec3(-1.0f, -1.0f, -1.0f), enemyLenghts);

            glm::vec3 enemyPoint = enemyCenter + clampedDifference;

            std::cout<<"enemy pos: "<<enemyCenter.x<<" , "<<enemyCenter.z<<" missle pos: "<<missleCenter.x<<" , "<<missleCenter.z<<std::endl;
            difference = enemyPoint - missleCenter;
            float missleRadius = missleComponent->length * glm::length(missle->localTransform.scale);
            return glm::length(difference) < missleRadius;
        }
    };

}