
#ifndef __SEEKPATH_H__
#define __SEEKPATH_H__ 1

#include "ia/defines.h"
#include <list>

class SeekPath {
  public:
    SeekPath() {}
    virtual ~SeekPath() {}

    /*virtual void calculate(const KinematicStatus& character, std::list<pathPoints>* pathPointList, Steering* steering) {
        //acceleration towards the target
        for (std::list<pathPoints>::iterator list_iter = pathPointList->begin();
            list_iter != pathPointList->end(); list_iter++) {
        
            steering->linear = (MathLib::Vec2(list_iter->x, list_iter->y) - character.position).normalized() * max_acceleration_;

            steering->angular = 0.0f;   //no angular    
            
        }

    }   */

    virtual void calculate(const KinematicStatus& character, MathLib::Vec2 nextPoint, Steering* steering) {
        //acceleration towards the target

        steering->linear = (nextPoint - character.position).normalized() * max_acceleration_;
       // std::cout << "Next Point    X: " << nextPoint.x << "        Y:          " << nextPoint.y << std::endl;
        steering->angular = 0.0f;   //no angular    

   

    }

  private:
    const float max_acceleration_ = 5.0f;
};
#endif