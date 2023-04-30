#pragma once

#include "Crocodile/s2d/Object.h"

using namespace Crocodile;

class Seagull
{

public:
    s2d::Object *sprite = nullptr;

    Seagull(s2d::Object *obj, std::vector<glm::vec2> movementPath);
    ~Seagull();

    void update(float dt);

    std::vector<glm::vec2> movementPath = {};
    unsigned int movementCounter = 1;

private:
    float speed = 5.f;
    bool facingLeft = true;
};