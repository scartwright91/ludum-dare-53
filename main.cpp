
#include "Crocodile.h"
#include "Crocodile/s2d/LevelParser.h"
#include "Crocodile/s2d/Layer.h"

#include "src/Player.h"
#include "src/World.h"
#include "src/Cave.h"

using namespace Crocodile;

class Game : public Crocodile::Application
{

public:
    s2d::Text *currentDelivery = nullptr;
    s2d::Text *startText = nullptr;
    std::vector<std::string> layernames = {};

    s2d::Object *sign = nullptr;
    s2d::Object *start = nullptr;
    s2d::Object *end = nullptr;

    World *world = nullptr;
    Cave *cave = nullptr;
    Player *player = nullptr;

    bool inWorld = true;
    bool startGame = true;
    bool endGame = false;

    Game() : Crocodile::Application("Test", 1200, 800)
    {

        init();

        // create start
        start = new s2d::Object();
        ResourceManager::TextureData startTex = resourceManager.getTexture("start");
        start->size = glm::vec2(scene->windowWidth, scene->windowHeight);
        start->setTexture(startTex.textureID);

        startText = new s2d::Text("SPACE to start", true);
        startText->color = glm::vec3(1.f);
        startText->setPosition(glm::vec2((float)scene->windowWidth / 2 - 100.f, (float)scene->windowHeight - 100));

        end = new s2d::Object();
        ResourceManager::TextureData endTex = resourceManager.getTexture("end");
        end->size = glm::vec2(scene->windowWidth, scene->windowHeight);
        end->setTexture(endTex.textureID);

        scene->addChild(start, "hud");
        scene->addChild(startText, "hud");
    }

    ~Game()
    {
        delete player;
        delete world;
    }

    void createGame()
    {
        // Create world and cave
        world = new World(scene, &resourceManager);
        cave = new Cave(scene, &resourceManager);

        // create player and add to scene
        player = new Player(scene, &resourceManager);

        // set camera to track player
        scene->camera->setTarget(player->sprite, false);

        sign = new s2d::Object();
        ResourceManager::TextureData tex = resourceManager.getTexture("sign");
        sign->size = glm::vec2(tex.width, tex.height);
        sign->setTexture(tex.textureID);
        sign->setPosition(glm::vec2(scene->windowWidth / 2 - tex.width / 2, 0.f));

        currentDelivery = new s2d::Text();
        currentDelivery->textScale = glm::vec2(0.75f);
        currentDelivery->color = glm::vec3(1.f);
        currentDelivery->setPosition(glm::vec2((float)scene->windowWidth / 2 - tex.width / 2 + 40.f, 60.f));

        // add to scene and follow with camera
        scene->addChild(sign, "hud");
        scene->addChild(currentDelivery, "hud");
    }

    void update(float dt)
    {

        if (startGame)
        {
            startText->update(dt);
            if (scene->window->isKeyPressed(GLFW_KEY_SPACE))
            {
                scene->clear();
                startGame = false;
                createGame();
            }
            return;
        }

        if (player->deliveringObject)
            currentDelivery->text = "Delivery: " + world->postOffice->currentDelivery;
        else
            currentDelivery->text = "Report to post office";

        if (inWorld)
        {
            player->update(dt, world->collideableObjects, world->hospital->getPosition(), world->lp->getLevelBounds().y);
            world->update(dt, player);
        }
        else
        {
            player->update(dt, cave->collideableObjects, world->hospital->getPosition(), world->lp->getLevelBounds().y);
            cave->update(dt, player);
        }

        // entering cave logic
        if (inWorld)
        {
            if (world->postOffice->currentDelivery == "cave" && player->deliveringObject)
            {
                if (player->sprite->getBoundingBox().intersectsBounds(world->cave->getBoundingBox()))
                {
                    world->cave->outline = true;
                    if (scene->window->isKeyPressed(GLFW_KEY_SPACE))
                    {
                        inWorld = false;
                        world->clear();
                        cave->addObjectsToScene();
                        scene->addChild(player->sprite, "entities");
                        player->sprite->setPosition(glm::vec2(1400.f, 950.f));
                        scene->addChild(sign, "hud");
                        scene->addChild(currentDelivery, "hud");
                    }
                }
            }
        }

        if (inWorld)
        {
            float depth = world->getDepth(player->depth);
            if (depth >= 0.97f)
                depth = 0.97f;
            world->oceanDepth->alpha = depth;
        }
        else
        {
            float depth = 0.8f + cave->getDepth(player->depth) / 5;
            cave->oceanDepth->alpha = depth;
        }

        if (cave->finished)
        {
            cave->clear();
            scene->addChild(end, "hud");
        }
    }

    void init()
    {

        loadResources();

        // define layers
        layernames = {"sky", "ocean", "background", "depth", "monster", "corals", "entities", "foreground", "hud"};
        for (std::string layername : layernames)
        {
            // create layers
            s2d::Layer *layer = new s2d::Layer(layername, 0.f);
            if (layername == "hud" || layername == "depth" || layername == "monster")
            {
                layer->applyZoom = false;
                layer->cameraScroll = false;
            }
            if (layername == "sky")
                layer->depth = 0.5f;
            scene->layerStack->addLayer(layer);
        }

        // scene options
        scene->setTransitionType(s2d::PostProcessing::FADE);
        scene->window->setBackgroundColor(glm::vec3((float)37 / 255, (float)109 / 255, (float)123 / 255));
        scene->camera->setZoom(3.f);
    }

    void loadResources()
    {
        resourceManager.loadTexture("res/images/sky.png", "sky", true);
        resourceManager.loadTexture("res/images/seafloor.png", "seafloor", true);
        resourceManager.loadTexture("res/images/turtle.png", "turtle", false);
        resourceManager.loadTexture("res/images/monster_eyes.png", "monster_eyes", false);
        resourceManager.loadTexture("res/images/monster_teeth.png", "monster_teeth", false);
        resourceManager.loadTexture("res/images/delivery_sign.png", "sign", false);
        resourceManager.loadTexture("res/images/start_pic.png", "start", false);
        resourceManager.loadTexture("res/images/end_pic.png", "end", false);
    }
};

Crocodile::Application *Crocodile::CreateApplication()
{
    return new Game();
}
