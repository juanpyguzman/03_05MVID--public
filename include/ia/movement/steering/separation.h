#ifndef __SEPARATION_H__
#define __SEPARATION_H__ 1

#include "ia/defines.h"

class Separation {
public:
    Separation() {}
    virtual ~Separation() {}

    virtual void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering) {
        //acceleration opposite to the neighbour
        steering->linear = (character.position - target->position).normalized() * max_acceleration_;
        steering->angular = 0.0f;   //no angular
    }

private:
    const float max_acceleration_ = 5.0f;
};
#endif