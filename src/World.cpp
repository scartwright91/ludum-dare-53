#include "World.h"

World::World(s2d::Scene *scene, ResourceManager *rm)
{
    this->scene = scene;
    this->rm = rm;
    lp = new s2d::LevelParser("res/world/sea_world.json", rm);
    scene->camera->setLevelBounds(lp->getLevelBounds(), true, false);
    createOcean();
    addObjectsToScene();
}

World::~World()
{
    delete lp;
    clear();
}

void World::clear()
{
    for (s2d::Object *o : objects)
        delete o;
    scene->clear();
    objects.clear();
}

void World::update(float dt, Player *player)
{
    seagull->update(dt);
    clownfish->update(dt);
    anglerfish->update(dt);

    for (Fish *f : fish)
    {
        f->update(dt, player);
    }

    // cut scene to target
    if (!player->canMove)
    {
        sceneTimer -= dt;
        if (sceneTimer <= 0)
        {
            scene->camera->setTarget(player->sprite, true);
            player->canMove = true;
        }
        else
            return;
    }

    // pickup delivery from post office
    postOffice->sprite->outline = false;
    if (player->sprite->getBoundingBox().intersectsBounds(postOffice->sprite->getBoundingBox()))
    {
        postOffice->sprite->outline = true;
        if (scene->window->isKeyPressed(GLFW_KEY_SPACE) && !player->deliveringObject)
        {
            player->deliveringObject = true;
            cutToDeliveryTarget(player, postOffice->currentDelivery);
        }
    }

    if (postOffice->currentDelivery == "seagull" && player->deliveringObject)
    {
        // check player-seagull collision
        seagull->sprite->outline = false;
        if (player->sprite->getBoundingBox().intersectsBounds(seagull->sprite->getShiftedBoundingBox(0, 100.f)))
        {
            seagull->sprite->outline = true;
            if (scene->window->isKeyPressed(GLFW_KEY_SPACE))
            {
                player->deliveringObject = false;
                postOffice->updateDelivery();
                seagull->sprite->outline = false;
            }
        }
    }

    if (postOffice->currentDelivery == "clownfish" && player->deliveringObject)
    {
        if (player->sprite->getBoundingBox().intersectsBounds(clownfish->sprite->getBoundingBox()))
        {
            clownfish->sprite->outline = true;
            if (scene->window->isKeyPressed(GLFW_KEY_SPACE))
            {
                player->deliveringObject = false;
                postOffice->updateDelivery();
                clownfish->sprite->outline = false;
            }
        }
    }

    if (postOffice->currentDelivery == "octopus" && player->deliveringObject)
    {
        if (player->sprite->getBoundingBox().intersectsBounds(octopus->getBoundingBox()))
        {
            octopus->outline = true;
            if (scene->window->isKeyPressed(GLFW_KEY_SPACE))
            {
                player->deliveringObject = false;
                postOffice->updateDelivery();
                octopus->outline = false;
            }
        }
    }

    if (postOffice->currentDelivery == "anglerfish" && player->deliveringObject)
    {
        if (player->sprite->getBoundingBox().intersectsBounds(anglerfish->sprite->getBoundingBox()))
        {
            anglerfish->sprite->outline = true;
            if (scene->window->isKeyPressed(GLFW_KEY_SPACE))
            {
                player->deliveringObject = false;
                postOffice->updateDelivery();
                anglerfish->sprite->outline = false;
            }
        }
    }
}

float World::getDepth(float playerDepth)
{
    return playerDepth / lp->getLevelBounds().y;
}

void World::cutToDeliveryTarget(Player *player, std::string currentDelivery)
{
    player->canMove = false;
    sceneTimer = 5.f;
    if (currentDelivery == "seagull")
    {
        sceneTimer = 4.f;
        scene->camera->setTarget(seagull->sprite, true);
    }
    else if (currentDelivery == "clownfish")
    {
        sceneTimer = 6.f;
        scene->camera->setTarget(clownfish->sprite, true);
    }
    else if (currentDelivery == "octopus")
        scene->camera->setTarget(octopus, true);
    else if (currentDelivery == "anglerfish")
        scene->camera->setTarget(anglerfish->sprite, true);
    else if (currentDelivery == "cave")
        scene->camera->setTarget(cave, true);
}

void World::createOcean()
{
    // create ocean depth surface
    oceanDepth = new s2d::Object();
    oceanDepth->size = glm::vec2(scene->windowWidth, scene->windowHeight);
    oceanDepth->color = glm::vec3(0.f);

    sky = new s2d::Object();
    ResourceManager::TextureData texture = rm->getTexture("sky");
    sky->size = glm::vec2(lp->getLevelBounds().x, 2 * texture.height);
    sky->setTexture(texture.textureID);
    sky->move(0.f, -2 * texture.height);

    seafloor = new s2d::Object();
    ResourceManager::TextureData texture2 = rm->getTexture("seafloor");
    seafloor->size = glm::vec2(lp->getLevelBounds().x, 2 * texture2.height);
    seafloor->setTexture(texture2.textureID);
    seafloor->move(0.f, lp->getLevelBounds().y);

    scene->addChild(oceanDepth, "depth");
    scene->addChild(sky, "sky");
    scene->addChild(seafloor, "sky");
}

void World::addObjectsToScene()
{
    const Json::Value entity_data = lp->getLayerData("entities");
    for (int i = 0; i < entity_data.size(); i++)
    {
        const Json::Value spr = entity_data[i];
        s2d::Object *o = parseSprite(spr);
        scene->addChild(o, "entities");
        if (o->label == "seagull")
        {
            float x = o->getPosition().x;
            o->setPosition(glm::vec2(x, -o->size.y));
            std::vector<glm::vec2> movementPath = {
                o->getPosition(),
                glm::vec2(x - 1500.f, -o->size.y),
                glm::vec2(x + 1500.f, -o->size.y)};
            seagull = new Seagull(o, movementPath);
        }
        else if (o->label == "clownfish")
            clownfish = new Clownfish(o, parseMovementPath(spr));
        else if (o->label == "anglerfish")
            anglerfish = new Anglerfish(o, parseMovementPath(spr));
        else if (o->label == "sharky")
            fish.push_back(new Shark(o, parseMovementPath(spr)));
        else if (o->label == "jellyfish")
            fish.push_back(new Jellyfish(o, parseMovementPath(spr)));
        else if (o->label == "octopus")
            octopus = o;
        else
            fish.push_back(new Fish(o, parseMovementPath(spr)));
        objects.push_back(o);
    }

    const Json::Value coral_data = lp->getLayerData("corals");
    for (int i = 0; i < coral_data.size(); i++)
    {
        const Json::Value spr = coral_data[i];
        s2d::Object *o = parseSprite(spr);
        scene->addChild(o, "corals");
        objects.push_back(o);
        collideableObjects.push_back(o);
    }

    const Json::Value bg_data = lp->getLayerData("background");
    for (int i = 0; i < bg_data.size(); i++)
    {
        const Json::Value spr = bg_data[i];
        s2d::Object *o = parseSprite(spr);
        scene->addChild(o, "background");
        if (o->label.find("coral_reef") != std::string::npos)
            o->setDistortionProperties(true, true, true, 0.01f);
        if (o->label == "post_office")
            postOffice = new PostOffice(o);
        if (o->label == "hospital")
            hospital = o;
        if (o->label == "cave")
            cave = o;
        if (o->label == "fish_school" || o->label == "fish_blue" || o->label == "fish_yellow" || o->label == "fish_angel")
            fish.push_back(new Fish(o, parseMovementPath(spr)));
        objects.push_back(o);
    }

    const Json::Value water_data = lp->getLayerData("foreground");
    for (int i = 0; i < water_data.size(); i++)
    {
        const Json::Value spr = water_data[i];
        s2d::Object *o = parseSprite(spr);
        scene->addChild(o, "foreground");
        if (o->label.find("coral_reef") != std::string::npos)
            o->setDistortionProperties(true, true, true, 0.01f);
        if (o->label == "fish_school" || o->label == "fish_blue" || o->label == "fish_yellow" || o->label == "fish_angel")
            fish.push_back(new Fish(o, parseMovementPath(spr)));
        objects.push_back(o);
    }
}

s2d::Object *World::parseSprite(Json::Value spr)
{
    float x = spr["pos"][0].asFloat();
    float y = spr["pos"][1].asFloat();
    int w = spr["width"].asInt();
    int h = spr["height"].asInt();
    std::string name = spr["name"].asString();
    std::string path = "res/" + spr["relative_image_path"].asString();
    s2d::Object *o = new s2d::Object();
    o->label = name;
    o->setStartingPosition(glm::vec2(x, y));
    o->size = glm::vec2(w, h);
    if (name == "")
        o->color = glm::vec3(1.0, 0.0, 0.0);
    else if (path == "res/None")
    {
        float r = spr["color"][0].asFloat() / 255;
        float g = spr["color"][1].asFloat() / 255;
        float b = spr["color"][2].asFloat() / 255;
        glm::vec3 col = glm::vec3(r, g, b);
        o->color = col;
    }
    else
    {
        if (!rm->textureExists(name))
            rm->loadTexture(path.c_str(), name, false);
        o->setTexture(rm->getTexture(name).textureID);
    }
    return o;
}

std::vector<glm::vec2> World::parseMovementPath(Json::Value spr)
{
    const Json::Value path = spr["movement_path"];
    std::vector<glm::vec2> movementPath = {};
    for (int i = 0; i < path.size(); i++)
    {
        const Json::Value pos = path[i];
        float x = pos[0].asFloat();
        float y = pos[1].asFloat();
        movementPath.push_back(glm::vec2(x, y));
    }
    return movementPath;
}
