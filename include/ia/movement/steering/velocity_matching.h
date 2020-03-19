//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __VELOCITY_MATCHING_H__
#define __VELOCITY_MATCHING_H__ 1

#include <ia/defines.h>

class VelocityMatching {
public:
  VelocityMatching() {}
  ~VelocityMatching() {}

  void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) {
    //linear acceleration adjusted to time
    steering->linear = (target->velocity - character.velocity) / time_to_target_;
    if (steering->linear.length() > max_acceleration_) {   //max out
      //normalized to max acceleration
      steering->linear = steering->linear.normalized() * max_acceleration_;
    }

    steering->angular = 0.0f;     //no angular
  }

private:
  const float max_acceleration_ = 5.0f;
  const float time_to_target_ = 1.0f;
};
#endif