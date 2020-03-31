
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
        steeringLinear_ = Vec2(0.0f, 0.0f);
        //steering->angular = 0.0f;   //no angular
    }
    else {
        const MathLib::Vec2 direction = centerOfMass - thisAgent->getKinematic()->position;
        const float distance = direction.length();
        float target_speed = max_speed_;      //max speed
        if (distance < slow_radius_) {        //inside the slow zone
          //speed slowing down
            target_speed = (max_speed_ * distance) / slow_radius_;
        }
        //velocity towards the target
        const MathLib::Vec2 target_velocity = direction.normalized() * target_speed;
        //steering->linear += ((target_velocity/neighboursNum - thisAgent->getKinematic()->velocity)/time_to_target_).normalized() * cohesionComponent_* max_acceleration_;
        //steering->angular = 0.0f;   //no angular

        steeringLinear_ = ((target_velocity / neighboursNum - thisAgent->getKinematic()->velocity) / time_to_target_).normalized() * max_acceleration_;
    }

}

Vec2 Cohesion::getSteeringLinear() {
    return steeringLinear_;
}