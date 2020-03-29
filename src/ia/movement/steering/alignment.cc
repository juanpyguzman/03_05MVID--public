
#include "ia/world.h"

void Alignment::calculate(Agent* thisAgent, World* world, Steering* steering, const float alignmentComponent) {
    /*//acceleration opposite to the neighbour
    steering->linear = (character.position - target->position).normalized() * max_acceleration_;
    steering->angular = 0.0f;   //no angular*/
    world_ = world;
    neighboursNum = 0;
    alignmentComponent_ = alignmentComponent;
    steeringAngular = 0.0f;
    for (uint16_t i = 0; i < world_->numIA(); ++i)
    {
        target_ = world_->ia(i)->getKinematic();
        if (world->ia(i) != thisAgent) {
            if ((thisAgent->getKinematic()->position - target_->position).length2() < radius_)
            {
                neighboursNum++;
                const float rotation = wrapAnglePI(target_->orientation - thisAgent->getKinematic()->orientation);
                const float rotation_size = abs(rotation);

                float target_rotation = max_rotation_;

                if (rotation_size < slow_radius_) {           
            
                    target_rotation = (max_rotation_ * rotation_size) / slow_radius_;
                }

                target_rotation *= sign(rotation);      //positive or negative
                //angular acceleration adjusted to time
                steering->angular = (target_rotation - thisAgent->getKinematic()->orientation) / time_to_target_;
                if (abs(steering->angular) > max_ang_acc_) {   //too fast
                  //normalized to max
                    steeringAngular += sign(steering->angular) * max_ang_acc_;
                }
            }
        }

    }

    if (neighboursNum == 0)
    {
        steering->linear = Vec2(0.0f, 0.0f);
        steering->angular = 0.0f;   //no angular
    }

    else {
        steering->angular = (steeringAngular / neighboursNum) * alignmentComponent_;
        steering->linear = MathLib::Vec2(0.0f, 0.0f);     //no linear
    }

}