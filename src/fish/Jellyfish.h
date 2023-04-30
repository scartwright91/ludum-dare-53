#pragma once

#include "../Fish.h"

class Jellyfish : public Fish
{
public:
    Jellyfish(s2d::Object *obj, std::vector<glm::vec2> movementPath);
    ~Jellyfish();

    void update(float dt, Player *player) override;
};