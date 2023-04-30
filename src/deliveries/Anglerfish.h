#pragma once

#include "Crocodile/s2d/Object.h"

using namespace Crocodile;

class Anglerfish
{
public:
    s2d::Object *sprite = nullptr;

    Anglerfish(s2d::Object *obj, std::vector<glm::vec2> movementPath);
    ~Anglerfish();

    void update(float dt);

    std::vector<glm::vec2> movementPath = {};
    unsigned int movementCounter = 1;

private:
    float speed = 10.f;

    bool facingLeft = true;
};