#pragma once

#include "Crocodile/s2d/Scene.h"
#include "Crocodile/ResourceManager.h"
#include "Crocodile/s2d/LevelParser.h"

#include "PostOffice.h"
#include "Player.h"
#include "deliveries/Seagull.h"
#include "deliveries/Clownfish.h"
#include "deliveries/Anglerfish.h"
#include "Fish.h"
#include "fish/Shark.h"
#include "fish/Jellyfish.h"

using namespace Crocodile;

class World
{

public:
    World(s2d::Scene *scene, ResourceManager *rm);
    ~World();

    void update(float dt, Player *player);
    void clear();

    std::vector<s2d::Object *> objects = {};
    std::vector<s2d::Object *> collideableObjects = {};
    float getDepth(float playerDepth);

    s2d::Object *oceanDepth = nullptr;

    PostOffice *postOffice = nullptr;
    s2d::Object *hospital = nullptr;
    Seagull *seagull = nullptr;
    Clownfish *clownfish = nullptr;
    Anglerfish *anglerfish = nullptr;
    s2d::Object *octopus = nullptr;
    s2d::Object *cave = nullptr;
    std::vector<Fish *> fish = {};

    s2d::LevelParser *lp = nullptr;

private:
    s2d::Scene *scene = nullptr;
    ResourceManager *rm = nullptr;

    s2d::Object *sky = nullptr;
    s2d::Object *seafloor = nullptr;
    float sceneTimer = 5.f;

    void cutToDeliveryTarget(Player *player, std::string currentDelivery);
    void createOcean();
    void addObjectsToScene();
    s2d::Object *parseSprite(Json::Value spr);
    std::vector<glm::vec2> parseMovementPath(Json::Value spr);
};