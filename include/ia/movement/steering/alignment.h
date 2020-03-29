#ifndef __ALIGNMENT_H__
#define __ALIGNMENT_H__ 1

#include "ia/defines.h"
#include <cstdint>

class World;
class Body;
class Agent;

class Alignment {
public:
    Alignment() {}
    virtual ~Alignment() {}

    virtual void calculate(Agent* thisAgent, World* world, Steering* steering, const float alignmentComponent);

private:
    const float radius_ = 10000.0f;
    const float max_ang_acc_ = 2.0f;
    const float max_rotation_ = 2.0f;
    const float slow_radius_ = 0.2f;
    const float time_to_target_ = 0.1f;
    float alignmentComponent_;
    float steeringAngular;
    uint16_t neighboursNum = 0;
    const KinematicStatus* target_;
    World* world_ = nullptr;
    
};
#endif