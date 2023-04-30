#pragma once

#include "Crocodile/s2d/Object.h"

#include "Player.h"

using namespace Crocodile;

class Fish
{
public:
    s2d::Object *sprite = nullptr;

    Fish(s2d::Object *obj, std::vector<glm::vec2> movementPath);
    ~Fish();

    virtual void update(float dt, Player *player);

    std::vector<glm::vec2> movementPath = {};
    unsigned int movementCounter = 1;

    float speed = 10.f;

    bool facingLeft = true;
};
