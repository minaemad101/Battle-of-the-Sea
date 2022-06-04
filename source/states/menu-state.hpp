#pragma once

#include <application.hpp>
#include <systems/menu.hpp>

#include <asset-loader.hpp>

class Menustate : public our::State
{
    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MenuSystem menuSystem;

    void onInitialize() override
    {
        auto &config = getApp()->getConfig()["menu"];

        if (config.contains("assets"))
            our::deserializeAllAssets(config["assets"]);

        if (config.contains("world"))
            world.deserialize(config["world"]);
    }

    void onDraw(double deltaTime) override
    {
        if (menuSystem.update(&world, (float)deltaTime))
        {
            getApp()->changeState("game");
        }
    }

    void onDestroy() override
    {

        our::clearAllAssets();
    }
};