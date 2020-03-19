//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __ARRIVE_H__
#define __ARRIVE_H__ 1

#include <ia/defines.h>

class Arrive {
  public:
    Arrive() {}
    ~Arrive() {}

    virtual void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) {
      //direction to the target
      const MathLib::Vec2 direction = target->position - character.position;
      const float distance = direction.length();    //distance to target

      float target_speed = max_speed_;      //max speed
      if (distance < slow_radius_) {        //inside the slow zone
        //speed slowing down
        target_speed = (max_speed_ * distance) / slow_radius_;
      }

      //velocity towards the target
      const MathLib::Vec2 target_velocity = direction.normalized() * target_speed;
      //linear acceleration adjusted to time
      steering->linear = (target_velocity - character.velocity) / time_to_target_;
      if (steering->linear.length() > max_acceleration_) {   //max out
        //normalized to max acceleration
        steering->linear = steering->linear.normalized() * max_acceleration_;
      }

      steering->angular = 0.0f;     //no angular
    }

  private:
    const float max_acceleration_ = 5.0f;
    const float max_speed_ = 100.0f;
    const float slow_radius_ = 100.0f;
    const float time_to_target_ = 1.0f;
};
#endif