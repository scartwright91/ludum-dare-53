#include "Shark.h"

Shark::Shark(s2d::Object *obj, std::vector<glm::vec2> movementPath) : Fish(obj, movementPath)
{
}

Shark::~Shark()
{
}

void Shark::update(float dt, Player *player)
{
    glm::vec2 targetPosition = glm::vec2(0.f);
    if ((sprite->getDistanceFrom(player->sprite->getPosition()) <= 1500.f) && player->alive)
    {
        targetPosition = player->sprite->getPosition();
        playerDetected = true;
        speed = 25.f;
    }
    else
    {
        playerDetected = false;
        targetPosition = movementPath[movementCounter];
        speed = 10.f;
    }

    glm::vec2 position = sprite->getPosition();
    float theta = std::atan2(targetPosition.y - position.y, targetPosition.x - position.x);
    float dx = std::cos(theta) * speed;
    float dy = std::sin(theta) * speed;
    sprite->move(dx, dy);
    if (facingLeft)
    {
        if (dx > 0)
        {
            facingLeft = false;
            sprite->flipX = true;
        }
    }
    else
    {
        if (dx < 0)
        {
            facingLeft = true;
            sprite->flipX = false;
        }
    }
    float d = sprite->getDistanceFrom(targetPosition);
    if (d <= 200.f)
    {
        if (playerDetected && player->alive)
            player->die();
        else
            movementCounter = (movementCounter + 1) % movementPath.size();
    }
}
