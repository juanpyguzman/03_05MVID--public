//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __WANDER_H__
#define __WANDER_H__ 1

#include "ia/defines.h"
#include "ia/movement/steering/delegated/face.h"

class Wander : public Face {
  public:
    Wander() {}
    ~Wander() {}

    void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) override {
      //update wander orientation, rate * binomial distribution
      wander_orientation_ += wander_rate_ * (randomFloat(0.0f, 1.0f) - randomFloat(0.0f, 1.0f));

      KinematicStatus new_target;
      //orientation of new target facing combined orientation
      new_target.orientation = wander_orientation_ + character.orientation;

      MathLib::Vec2 char_orient;   //orientation of character as vector
      char_orient.fromPolar(1.0f, character.orientation);

      MathLib::Vec2 target_orient;  //orientation of new target as vector
      target_orient.fromPolar(1.0f, new_target.orientation);

      //the center of the circle
      new_target.position = character.position + (char_orient * wander_offset_);
      //position of the target in the circle
      new_target.position += target_orient * wander_radius_;

      DebugDraw::drawCross(new_target.position, 0x00, 0x00, 0xFF, 0xFF);

      //delegate to face behavior
      Face::calculate(character, &new_target, steering);
      //linear to full acceleration in direction of orientation
      steering->linear = char_orient * max_acceleration_;
    }

  private:
    float wander_offset_ = 50.0f;       //forward offset of circle
    float wander_radius_ = 20.0f;       //radius of circle
    float wander_rate_ = 2.0f;          //max rate at which wander orientation can change
    float wander_orientation_ = 0.0f;   //current orientation of target
    float max_acceleration_ = 5.0f;
};
#endif