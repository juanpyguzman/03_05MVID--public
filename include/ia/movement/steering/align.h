//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __ALIGN_H__
#define __ALIGN_H__ 1

#include "ia/defines.h"

class Align {
  public:
    Align() {}
    virtual ~Align() {}

    virtual void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) {
      //rotation between character and target wrapped to (-PI, PI)
      const float rotation = wrapAnglePI(target->orientation - character.orientation);
      const float rotation_size = abs(rotation);     //absolute value of rotation

      float target_rotation = max_rotation_;         //max
      if (rotation_size < slow_radius_) {            //inside the slow zone
        //speed of rotation slowing down
        target_rotation = (max_rotation_ * rotation_size) / slow_radius_;
      }

      target_rotation *= sign(rotation);      //positive or negative
      //angular acceleration adjusted to time
      steering->angular = (target_rotation - character.rotation) / time_to_target_;
      if (abs(steering->angular) > max_ang_acc_) {   //too fast
        //normalized to max
        steering->angular = sign(steering->angular) * max_ang_acc_;
      }

      steering->linear = MathLib::Vec2(0.0f, 0.0f);     //no linear
    }

  private:
    const float max_ang_acc_ = 2.0f;
    const float max_rotation_ = 2.0f;
    const float slow_radius_ = 0.2f;
    const float time_to_target_ = 0.1f;
};
#endif