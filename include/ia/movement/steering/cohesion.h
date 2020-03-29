#ifndef __COHESION_H__
#define __COHESION_H__ 1

#include "ia/defines.h"
#include <cstdint>

class World;
class Body;
class Agent;

class Cohesion {
public:
    Cohesion() {}
    virtual ~Cohesion() {}

    virtual void calculate(Agent* thisAgent, World* world, Steering* steering);

private:
    const float max_acceleration_ = 5.0f;
    const float radius_ = 10.0f;
    Vec2 centerOfMass;
    uint16_t neighboursNum = 0;
    const KinematicStatus* target_;
    World* world_ = nullptr;
    
    
};
#endif