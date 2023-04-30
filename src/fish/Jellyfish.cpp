#include "Jellyfish.h"

Jellyfish::Jellyfish(s2d::Object *obj, std::vector<glm::vec2> movementPath) : Fish(obj, movementPath)
{
}

Jellyfish::~Jellyfish()
{
}

void Jellyfish::update(float dt, Player *player)
{

    glm::vec2 targetPosition = movementPath[movementCounter];
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
        movementCounter = (movementCounter + 1) % movementPath.size();
    }

    float playerDistance = sprite->getDistanceFrom(player->sprite->getPosition());
    if (playerDistance <= 200.f)
    {
        player->die();
    }
}
