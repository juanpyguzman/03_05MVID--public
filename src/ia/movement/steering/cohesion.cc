
#include "ia/world.h"

void Cohesion::calculate(Agent* thisAgent, World* world, Steering* steering) {
    /*//acceleration opposite to the neighbour
    steering->linear = (character.position - target->position).normalized() * max_acceleration_;
    steering->angular = 0.0f;   //no angular*/
    world_ = world;
    neighboursNum = 0;
    centerOfMass = thisAgent->getKinematic()->position;
    for (uint16_t i = 0; i < world_->numIA(); ++i)
    {
        target_ = world_->ia(i)->getKinematic();
        if (world->ia(i) != thisAgent) {
            //std::cout << (thisAgent->getKinematic()->position - target_->position).length2() << std::endl;
            if ((thisAgent->getKinematic()->position - target_->position).length2() < radius_)
            {
                centerOfMass += target_->position;
                neighboursNum++;
            }
        }

    }

    if (neighboursNum == 0)
    {
        steering->linear = Vec2(0.0f, 0.0f);
        steering->angular = 0.0f;   //no angular
    }
    else {
        steering->linear += (centerOfMass/neighboursNum - thisAgent->getKinematic()->position).normalized() * max_acceleration_;
        steering->angular = 0.0f;   //no angular
    }

}