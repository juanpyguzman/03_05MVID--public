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
#include <iostream>
#include <stdlib.h>

using MathLib::Vec2;

class World {
  public:
    World() {
      //agent_.init(this, Body::Role::Soldier, Body::Type::Autonomous);
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

    void setZones(std::vector<zone> zones) {
        zones_ = zones;

        for (int i = 0; i < WINDOW_WIDTH/8; i++) {
            for (int j = 0; j < WINDOW_HEIGHT / 8; j++) {
                if (zones_[i * WINDOW_WIDTH/8 + j] == zone::Exterior) {
                    std::cout << "Zona X: " << i << "      Y: " << j << "----> Exterior" << std::endl;
                }
            }
        }

    }
    
    void init() {
        agent_.init(this, Body::Role::Soldier, Body::Type::Autonomous);
    }

    Agent* agent() { return &agent_; }
    //Agent* ia() { return &ia_; }
  private:
      Agent agent_;
      std::vector<zone> zones_;
      //Agent ia_;
};

#endif