//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __WORLD_H__
#define __WORLD_H__ 1

#include "ia/agent.h"
#include "ia/defines.h"
#include "mathlib/vec2.h"

using MathLib::Vec2;

class World {
  public:
    World() {
      agent_.init(this, Body::Color::Red, Body::Type::Autonomous);
      //ia_.init(this, Body::Color::Green, Body::Type::Autonomous);
     // ia_.getKinematic()->position = Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    };
    ~World() {
      agent_.shutdown();
      //ia_.shutdown();
    };

    void update(const float dt) { 
        agent_.update(dt); 
        //ia_.update(dt); 
    }
    void render() const { 
        agent_.render(); 
        //ia_.render(); 
    }

    Agent* agent() { return &agent_; }
    //Agent* ia() { return &ia_; }
  private:
      Agent agent_;
      //Agent ia_;
};

#endif