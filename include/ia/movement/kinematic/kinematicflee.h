//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __KINEMATIC_FLEE_H__
#define __KINEMATIC_FLEE_H__ 1

#include "ia/defines.h"

class KinematicFlee {
  public:
    KinematicFlee() {}
    ~KinematicFlee() {}

    void calculate(const KinematicStatus& character, const KinematicStatus* target, KinematicSteering* steering) const {
      //going full speed opposite to the target
      steering->velocity = (character.position - target->position).normalized() * max_speed_;
      steering->rotation = 0.0f;     //no rotation
    }

  private:
    const float max_speed_ = 100.0f;
};
#endif