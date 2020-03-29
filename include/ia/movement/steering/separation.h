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

private:
    const float max_acceleration_ = 5.0f;
    const float radius_ = 5000.0f;
    uint16_t neighboursNum = 0;
    const KinematicStatus* target_;
    World* world_ = nullptr;
    
};
#endif