#pragma once

#include <vector>
#include "Crocodile/s2d/Scene.h"
#include "Crocodile/s2d/Particles.h"
#include "Crocodile/s2d/LightSystem.h"

#include "Crocodile/ResourceManager.h"

using namespace Crocodile;

class Player
{
public:
    s2d::Object *sprite = nullptr;

    Player(s2d::Scene *scene, ResourceManager *rm);
    ~Player();

    void update(float dt, std::vector<s2d::Object *> collideableObjects, glm::vec2 hospital, float maxDepth);
    void die();

    float depth = 0.f;
    bool deliveringObject = false;
    bool canMove = true;
    bool alive = true;

private:
    s2d::Scene *scene = nullptr;
    ResourceManager *rm = nullptr;

    bool facingLeft = true;
    float speed = 40.f;

    s2d::ParticleGenerator *pg = nullptr;

    glm::vec2 calculateMovement(float dt);
    glm::vec2 calculateCollision(glm::vec2 movement, std::vector<s2d::Object *> collideables);
};