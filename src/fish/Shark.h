#pragma once

#include "../Fish.h"

class Shark : public Fish
{
public:
    Shark(s2d::Object *obj, std::vector<glm::vec2> movementPath);
    ~Shark();

    void update(float dt, Player *player) override;

    bool playerDetected = false;
};