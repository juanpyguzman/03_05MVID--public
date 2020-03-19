//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __FACE_H__
#define __FACE_H__ 1

#include <ia/defines.h>
#include "ia/movement/steering/align.h"

class Face : public Align {
  public:
    Face() {}
    ~Face() {}

    virtual void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) {
      //direction to target
      const MathLib::Vec2 direction = target->position - character.position;

      KinematicStatus new_target = *target;         //new target
      //orientation of new target facing direction
      new_target.orientation = atan2(direction.y(), direction.x());

      //delegate to align behavior with new target
      Align::calculate(character, &new_target, steering);
    }
};
#endif