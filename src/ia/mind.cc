//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/mind.h"
#include "ia/body.h"
#include "ia/world.h"

void Mind::init(World* world, Body* body) {
  world_ = world;
  body_ = body;
}

void Mind::update(const uint32_t) const {
  //body_->setTarget(world_->target());
}
