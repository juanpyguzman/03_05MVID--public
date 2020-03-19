//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __KINEMATIC_SEEK_H__
#define __KINEMATIC_SEEK_H__ 1

#include <ia/defines.h>

class KinematicSeek {
  public:
    KinematicSeek() {}
    ~KinematicSeek() {}

    void calculate(const KinematicStatus& character, const KinematicStatus* target, KinematicSteering* steering) {
      //going full speed towards the target
      steering->velocity = (target->position - character.position).normalized() * max_speed_;
      steering->rotation = 0.0f;     //no rotation
    }

  private:
    const float max_speed_ = 100.0f;
};
#endif