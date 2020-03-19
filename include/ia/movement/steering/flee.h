//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __FLEE_H__
#define __FLEE_H__ 1

#include <ia/defines.h>

class Flee {
  public:
    Flee() {}
    ~Flee() {}

    virtual void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) {
      //acceleration opposite to the target
      steering->linear = (character.position - target->position).normalized() * max_acceleration_;
      steering->angular = 0.0f;   //no angular
    }

  private:
    const float max_acceleration_ = 5.0f;
};
#endif