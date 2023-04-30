#include "PostOffice.h"

PostOffice::PostOffice(s2d::Object *obj)
{
    sprite = obj;
}

PostOffice::~PostOffice()
{
}

void PostOffice::updateDelivery()
{
    deliveryCounter++;
    currentDelivery = deliveries[deliveryCounter];
}