//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __AGENT_H__
#define __AGENT_H__ 1

#include "ia/body.h"
#include "ia/mind.h""

#include <cstdint>

class World;

class Agent {
  public:
    Agent() {};
    ~Agent() {};

    void init(World* world, Body::Role role, Body::Type type);
    void update(uint32_t dt);
    void render() const;
    void shutdown();

    void setSteering(Body::SteeringMode steering) { body_.setSteering(steering); } 
    void setBehaviour(Body::Behaviour behaviour) { body_.setBehaviour(behaviour); }
    const KinematicStatus* getKinematic() const { return body_.getKinematic(); }
    KinematicStatus* getKinematic() { return body_.getKinematic(); }
    
    Body body_;
    Mind mind_;

private:
    World * world_ = nullptr;
};

#endif