#ifndef __FLOCKING_H__
#define __FLOCKING_H__ 1

#include <cstdint>
#include "engine/debug_draw.h"
#include "ia/defines.h"
#include "ia/movement/steering/seek.h"
#include "ia/movement/steering/separation.h"
#include "ia/movement/steering/cohesion.h"
#include "ia/movement/steering/alignment.h"

class World;
class Body;
class Agent;

class Flocking : public Seek {
public:
    Flocking() {}
    ~Flocking() {}

    virtual void calculate(Agent* thisAgent, World* world, const KinematicStatus* target, Steering* steering);

private:
    Separation separation_;
    Cohesion cohesion_;
    Alignment alignment_;
    Vec2 seek_;

    const float separationComponent_ = 0.15f;
    const float cohesionComponent_ = 0.30f;
    const float alignmentComponent_ = 0.10;
    const float seekComponent_ = 0.45f;

    const float max_acceleration_ = 5.0f;
    World* world_ = nullptr;
};
#endif