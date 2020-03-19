//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __KINEMATIC_WANDER_H__
#define __KINEMATIC_WANDER_H__ 1

#include <engine/math.h>

class KinematicWander {
  public:
    KinematicWander() {}
    ~KinematicWander() {}

    void calculate(const KinematicStatus& character, const KinematicStatus*, KinematicSteering* steering) {
      MathLib::Vec2 orientation;
      //orientation of character as vector
      orientation.fromPolar(1.0f, character.orientation);

      steering->velocity = orientation * max_speed_;    //max speed
      //rotate to random (binomial distribution around 0)
      steering->rotation = max_rotation * (randomFloat(0.0f, 1.0f) - randomFloat(0.0f, 1.0f));
    }

  private:
    const float max_speed_ = 50.0f;
    const float max_rotation = 3.14f;
};
#endif