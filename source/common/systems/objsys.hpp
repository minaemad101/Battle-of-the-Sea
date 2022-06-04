#pragma once

#include "../ecs/world.hpp"
#include "../components/objmove.hpp"
#include "../components/objmove.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/movement.hpp"
    class objsys {
    public:
     Application* app; 
     
void enter(Application* app){
            this->app = app;
        }
        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
            // For each entity in the world

        //    CameraComponent* camera = nullptr;
            objmove *objm=nullptr;
            
            for(auto entity : world->getEntities()){
                objm = entity->getComponent<objmove>();
                if(objm) break;
            }

            if(objm){

            Entity * entity=objm->getOwner();
            glm :: mat4 matrix=entity->localTransform.toMat4();
            glm :: vec3 right=glm ::vec3 (matrix*glm :: vec4(1,0,0,0)); 
           
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            
            

            std::cout<<"ay 7aga e7na gowa el moving obj"<<std::endl;

            if(app->getKeyboard().isPressed(GLFW_KEY_D)) entity->localTransform.position  +=  deltaTime * objm->linearVelocity;
            if(app->getKeyboard().isPressed(GLFW_KEY_A)) entity->localTransform.position -=  deltaTime * objm->linearVelocity;                    


            }

}

     

    };

}
