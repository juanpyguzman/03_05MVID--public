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

    virtual void calculate(Agent* thisAgent, World* world, Steering* steering, const float separationComponent);

private:
    const float max_acceleration_ = 5.0f;
    float separationComponent_;
    const float radius_ = 10000.0f;
    Vec2 steeringLinear;
    uint16_t neighboursNum = 0;
    const KinematicStatus* target_;
    World* world_ = nullptr;
    
};
#endif