//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __KINEMATIC_ARRIVE_H__
#define __KINEMATIC_ARRIVE_H__ 1

#include <ia/defines.h>

class KinematicArrive {
  public:
    KinematicArrive() {}
    ~KinematicArrive() {}

    void calculate(const KinematicStatus& character, const KinematicStatus* target, KinematicSteering* steering) {
      //direction to the target
      steering->velocity = (target->position - character.position);
      if (steering->velocity.length2() < sq_radius_) {  //inside the radius
        steering->velocity.x() = 0.0f;        //no velocity
        steering->velocity.y() = 0.0f;
      } else {
        steering->velocity /= time_to_target_;     //velocity adjusted to time
        if (steering->velocity.length() > max_speed_) {     //max out
          //normalized direction to max speed
          steering->velocity = steering->velocity.normalized() * max_speed_;
        }
      }
      steering->rotation = 0.0f;   //no rotation
    }

  private:
    const float max_speed_ = 100.0f;
    const float sq_radius_ = 25.0f;      //squared radius
    const float time_to_target_ = 0.5f;
};
#endif