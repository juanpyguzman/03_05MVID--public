
#include "ia/world.h"

void Separation::calculate(Agent* thisAgent, World* world, Steering* steering) {
    /*//acceleration opposite to the neighbour
    steering->linear = (character.position - target->position).normalized() * max_acceleration_;
    steering->angular = 0.0f;   //no angular*/
    world_ = world;
    neighboursNum = 0;
    steeringLinear_ = Vec2(0.0f, 0.0f);
    for (uint16_t i = 0; i < world_->numIA(); ++i)
    {
        target_ = world_->ia(i)->getKinematic();
        if (world->ia(i) != thisAgent) {
            //std::cout << (thisAgent->getKinematic()->position - target_->position).length2() << std::endl;
            if ((thisAgent->getKinematic()->position - target_->position).length2() < radius_)
            {
                steeringLinear_ += (thisAgent->getKinematic()->position - target_->position).normalized() * max_acceleration_;
                neighboursNum++;
            }
        }

    }

    if (neighboursNum == 0)
    {
        //steering->linear = Vec2(0.0f, 0.0f);
        steeringLinear_ = Vec2(0.0f, 0.0f);
    }

    else
    {
        //steering->linear = (steeringLinear / neighboursNum) * separationComponent_;   //no angular;
        //steering->angular = 0.0f;   //no angular

        steeringLinear_ = (steeringLinear_ / neighboursNum);
    }

}

Vec2 Separation::getSteeringLinear()
{
    return steeringLinear_;
}