#ifndef __SEPARATION_H__
#define __SEPARATION_H__ 1

#include "ia/defines.h"
#include <cstdint>

class World;
class Body;
class Agent;

class Separation {
public:
    Separation() {}
    virtual ~Separation() {}

    virtual void calculate(Agent* thisAgent, World* world, Steering* steering);

    Vec2 getSteeringLinear();

private:
    const float max_acceleration_ = 5.0f;
    const float radius_ = 10000.0f;
    Vec2 steeringLinear_;
    uint16_t neighboursNum = 0;
    const KinematicStatus* target_;
    World* world_ = nullptr;
    
};
#endif