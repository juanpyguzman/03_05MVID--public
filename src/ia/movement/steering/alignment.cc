
#include "ia/world.h"

void Alignment::calculate(Agent* thisAgent, World* world, Steering* steering) {
    /*//acceleration opposite to the neighbour
    steering->linear = (character.position - target->position).normalized() * max_acceleration_;
    steering->angular = 0.0f;   //no angular*/
    world_ = world;
    neighboursNum = 0;
    for (uint16_t i = 0; i < world_->numIA(); ++i)
    {
        target_ = world_->ia(i)->getKinematic();
        if (world->ia(i) != thisAgent) {
            if ((thisAgent->getKinematic()->position - target_->position).length2() < radius_)
            {
                steering->linear += (thisAgent->getKinematic()->position - target_->position).normalized() * max_acceleration_;
                steering->angular = 0.0f;   //no angular
                neighboursNum++;
            }
        }

    }

    if (neighboursNum == 0)
    {
        steering->linear = Vec2(0.0f, 0.0f);
    }

}