#pragma once

#include <vector>
#include <string>
#include "Crocodile/s2d/Object.h"

using namespace Crocodile;

class PostOffice
{
public:
    s2d::Object *sprite = nullptr;

    PostOffice(s2d::Object *obj);
    ~PostOffice();

    unsigned int deliveryCounter = 0;
    std::vector<std::string> deliveries = {
        "seagull",
        "clownfish",
        "octopus",
        "anglerfish",
        "cave"};
    std::string currentDelivery = deliveries[deliveryCounter];

    void updateDelivery();
};