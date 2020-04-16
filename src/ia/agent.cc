//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/agent.h"
#include "ia/world.h"

void Agent::init(World* world, const Body::Role role, const Body::Type type, int ID) {
  world_ = world;
  ID_ = ID;
  body_.init(this, role, type, &mind_, world->zonas_, world->doors_);
  mind_.init(world, &body_);
}

void Agent::shutdown() {
  world_ = nullptr;
}

void Agent::update(const uint32_t dt) {
  mind_.update(dt);
  body_.update(dt);
}

void Agent::render() const {
  body_.render();
}