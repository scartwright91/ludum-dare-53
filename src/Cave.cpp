#include "Cave.h"

Cave::Cave(s2d::Scene *scene, ResourceManager *rm)
{
    this->scene = scene;
    this->rm = rm;
    lp = new s2d::LevelParser("res/world/cave.json", rm);
}

Cave::~Cave()
{
}

void Cave::clear()
{
    for (s2d::Object *o : objects)
        delete o;
    scene->clear();
    objects.clear();
}

void Cave::update(float dt, Player *player)
{
    if (steps == 0)
    {
        if (player->depth >= 5000.f)
        {
            steps++;
            player->canMove = false;
        }
    }
    if (steps == 1)
    {
        player->sprite->move(0.f, 10.f);
        if (player->depth >= 6500.f)
        {
            steps++;
        }
    }
    if (steps == 2)
    {
        eyes->alpha += dt;
        if (eyes->alpha >= 1.0f)
            steps++;
    }
    if (steps == 3)
    {
        timer -= dt;
        if (timer <= 0)
        {
            timer = 2.f;
            steps++;
        }
    }
    if (steps == 4)
    {
        teeth->alpha += dt;
        if (teeth->alpha >= 1.0f)
        {
            steps++;
        }
    }
    if (steps == 5)
    {
        timer -= dt;
        if (timer <= 0)
        {
            finished = true;
        }
    }
}

float Cave::getDepth(float playerDepth)
{
    return playerDepth / lp->getLevelBounds().y;
}

void Cave::addObjectsToScene()
{

    scene->camera->setLevelBounds(lp->getLevelBounds(), false, true);
    scene->window->setBackgroundColor(glm::vec3(0.f));

    // create ocean depth surface
    oceanDepth = new s2d::Object();
    oceanDepth->size = glm::vec2(scene->windowWidth, scene->windowHeight);
    oceanDepth->color = glm::vec3(0.f);
    scene->addChild(oceanDepth, "depth");

    eyes = new s2d::Object();
    ResourceManager::TextureData eyesTex = rm->getTexture("monster_eyes");
    eyes->size = glm::vec2(scene->windowWidth, scene->windowHeight);
    eyes->setTexture(eyesTex.textureID);
    eyes->alpha = 0.f;

    teeth = new s2d::Object();
    ResourceManager::TextureData teethTex = rm->getTexture("monster_teeth");
    teeth->size = glm::vec2(scene->windowWidth, scene->windowHeight);
    teeth->setTexture(teethTex.textureID);
    teeth->alpha = 0.f;

    scene->addChild(eyes, "monster");
    scene->addChild(teeth, "monster");

    const Json::Value entity_data = lp->getLayerData("entities");
    for (int i = 0; i < entity_data.size(); i++)
    {
        const Json::Value spr = entity_data[i];
        s2d::Object *o = parseSprite(spr);
        scene->addChild(o, "entities");
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
        objects.push_back(o);
    }

    const Json::Value water_data = lp->getLayerData("foreground");
    for (int i = 0; i < water_data.size(); i++)
    {
        const Json::Value spr = water_data[i];
        s2d::Object *o = parseSprite(spr);
        scene->addChild(o, "foreground");
        objects.push_back(o);
    }
}

s2d::Object *Cave::parseSprite(Json::Value spr)
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

std::vector<glm::vec2> Cave::parseMovementPath(Json::Value spr)
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
