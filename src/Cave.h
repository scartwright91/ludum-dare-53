#pragma once

#include <vector>
#include "Crocodile/s2d/Scene.h"
#include "Crocodile/ResourceManager.h"
#include "Crocodile/s2d/LevelParser.h"

#include "Player.h"

class Cave
{
public:
    Cave(s2d::Scene *scene, ResourceManager *rm);
    ~Cave();

    void update(float dt, Player *player);
    float getDepth(float playerDepth);
    void clear();

    s2d::LevelParser *lp = nullptr;

    std::vector<s2d::Object *> objects = {};
    std::vector<s2d::Object *> collideableObjects = {};
    void addObjectsToScene();

    s2d::Object *oceanDepth = nullptr;
    s2d::Object *eyes = nullptr;
    s2d::Object *teeth = nullptr;

    unsigned int steps = 0;
    float timer = 1.f;
    bool finished = false;

private:
    s2d::Scene *scene = nullptr;
    ResourceManager *rm = nullptr;

    s2d::Object *parseSprite(Json::Value spr);
    std::vector<glm::vec2> parseMovementPath(Json::Value spr);
};