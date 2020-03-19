//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __LOOK_GOING_H__
#define __LOOK_GOING_H__ 1

#include <ia/defines.h>
#include "ia/movement/steering/align.h"

class LookGoing : public Align {
  public:
    LookGoing() {}
    ~LookGoing() {}

    virtual void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) {
      if (character.velocity.length() ==0) {      //no movement
        steering->angular = 0.0f;
        return;
      }

      KinematicStatus new_target = *target;
      //orientation of new target facing velocity
      new_target.orientation = atan2(character.velocity.y(), character.velocity.x());

      //delegate to align behavior with new target
      Align::calculate(character, &new_target, steering);
    }
};
#endif