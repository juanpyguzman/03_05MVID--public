//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __PURSUE_H__
#define __PURSUE_H__ 1

#include "engine/debug_draw.h"
#include "ia/defines.h"
#include "ia/movement/steering/seek.h"

class Pursue: public Seek {
  public:
    Pursue() {}
    ~Pursue() {}

    void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) override {
      //distance to the target
      const float distance = (target->position - character.position).length();
      const float speed = character.velocity.length();    //speed of character

      float prediction = max_prediction_;           //max prediction
      if (speed > (distance / max_prediction_)) {   //reasonable prediction
        prediction = distance / speed;              //calc prediction time
      }

      KinematicStatus new_target = *target;         //new target
      //position of new target
      new_target.position += target->velocity * prediction;

      DebugDraw::drawCross(new_target.position, 0x00, 0x00, 0xFF, 0xFF);

      //delegate to seek behavior with new target
      Seek::calculate(character, &new_target, steering);
    }

  private:
    const float max_prediction_ = 2.0f;
};
#endif