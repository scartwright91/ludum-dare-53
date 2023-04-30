#include "Player.h"

Player::Player(s2d::Scene *scene, ResourceManager *rm)
{
    this->scene = scene;
    this->rm = rm;
    sprite = new s2d::Object(1000.f, 500.f);
    ResourceManager::TextureData texture = rm->getTexture("turtle");
    sprite->size = glm::vec2(texture.width, texture.height);
    sprite->setTexture(texture.textureID);

    pg = new s2d::ParticleGenerator(1000.f, 500.f, 20.f, 50.f, 1.f);

    this->scene->addChild(sprite, "entities");
    this->scene->addChild(pg, "foreground");
}

Player::~Player()
{
    delete sprite;
}

void Player::update(float dt, std::vector<s2d::Object *> collideableObjects, glm::vec2 hospital, float maxDepth)
{
    pg->update(dt);
    depth = sprite->getPosition().y;

    if (!canMove)
        return;

    if (!alive)
    {
        if (scene->isEndSceneTransitionFinished())
        {
            scene->beginSceneTransition();
            sprite->setPosition(hospital);
        }
        if (scene->isBeginSceneTransitionFinished())
            alive = true;
        return;
    }

    glm::vec2 movementBeforeCollision = calculateMovement(dt);
    glm::vec2 movementAfterCollision = calculateCollision(movementBeforeCollision, collideableObjects);
    sprite->move(movementAfterCollision.x, movementAfterCollision.y);

    if (depth <= 0)
        sprite->move(0.f, speed);
    if (depth >= maxDepth)
        sprite->move(0.f, -speed);

    if (facingLeft)
    {
        if (movementAfterCollision.x > 0)
        {
            facingLeft = false;
            sprite->flipX = true;
        }
    }
    else
    {
        if (movementAfterCollision.x < 0)
        {
            facingLeft = true;
            sprite->flipX = false;
        }
    }
}

void Player::die()
{
    scene->endSceneTransition();
    alive = false;
}

glm::vec2 Player::calculateMovement(float dt)
{
    float dx = 0.f;
    float dy = 0.f;

    if (scene->window->isKeyPressed(GLFW_KEY_W))
        dy = -speed;
    if (scene->window->isKeyPressed(GLFW_KEY_S))
        dy = speed;
    if (scene->window->isKeyPressed(GLFW_KEY_A))
        dx = -speed;
    if (scene->window->isKeyPressed(GLFW_KEY_D))
        dx = speed;

    return glm::vec2(dx, dy);
}

glm::vec2 Player::calculateCollision(glm::vec2 movement, std::vector<s2d::Object *> collideables)
{
    for (s2d::Object *e : collideables)
    {
        // x-axis collisions
        bool xCollision = sprite->getShiftedBoundingBox(movement.x, 0.0f).intersectsBounds(e->getBoundingBox());
        if (xCollision)
        {
            float dx = 0;
            while (abs(dx) < abs(movement.x))
            {
                bool xCollision = sprite->getShiftedBoundingBox(dx, 0.0f).intersectsBounds(e->getBoundingBox());
                if (xCollision)
                {
                    if (movement.x > 0)
                        movement.x = dx - 0.1f;
                    if (movement.x < 0)
                        movement.x = dx + 0.1f;
                    break;
                }
                if (movement.x > 0)
                {
                    dx += 0.1f;
                }
                else
                {
                    dx -= 0.1f;
                }
            }
        }

        bool yCollision = sprite->getShiftedBoundingBox(0.0f, movement.y).intersectsBounds(e->getBoundingBox());
        if (yCollision)
        {
            float dy = 0;
            while (abs(dy) < abs(movement.y))
            {
                bool yCollision = sprite->getShiftedBoundingBox(0.0f, dy).intersectsBounds(e->getBoundingBox());
                if (yCollision)
                {
                    if (movement.y > 0)
                        movement.y = dy - 0.1f;
                    if (movement.y < 0)
                        movement.y = dy + 0.1f;
                    break;
                }
                if (movement.y > 0)
                {
                    dy += 0.1f;
                }
                else
                {
                    dy -= 0.1f;
                }
            }
        }
    }
    return movement;
}
