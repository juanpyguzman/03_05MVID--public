//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __SEEK_H__
#define __SEEK_H__ 1

#include <ia/defines.h>

class Seek {
  public:
    Seek() {}
    ~Seek() {}

    virtual void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) {
      //aceeleration towards the target
      steering->linear = (target->position - character.position).normalized() * max_acceleration_;
      steering->angular = 0.0f;   //no angular
    }

  private:
    const float max_acceleration_ = 5.0f;
};
#endif