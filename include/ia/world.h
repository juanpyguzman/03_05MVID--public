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
        for (int i = 0; i < numSoldiers; i++)
        {
            soldiers_[i].shutdown();
        }
      //agent3_.shutdown();
      //ia_.shutdown();
    };

    void update(const float dt) { 
        for (int i = 0; i < numSoldiers; i++)
        {
            soldiers_[i].update(dt);
        }
        //agent3_.update(dt);
        //ia_.update(dt); 
    }
    void render() const { 
        for (int i = 0; i < numSoldiers; i++)
        {
            soldiers_[i].render();
        }

        //agent3_.render();
        //ia_.render(); 
    }

    void setZones(zonas zonasMapa) {
        zonas_ = zonasMapa;
        //zones_ = zones;

        /*for (int i = 0; i < WINDOW_WIDTH/8; i++) {
            for (int j = 0; j < WINDOW_HEIGHT / 8; j++) {
                if (zones_[i * WINDOW_WIDTH/8 + j] == zone::Exterior) {
                    std::cout << "Zona X: " << i << "      Y: " << j << "----> Exterior" << std::endl;
                }
            }
        }*/

    }

    void setDoors(std::vector<doors>* doorsState)
    {
        doors_ = doorsState;
    }
    
    void init() {
        for (int i = 0; i < numSoldiers; i++)
        {
            soldiers_.push_back(soldier_);
        }
        
        for (int i = 0; i < numSoldiers; i++)
        {
            soldiers_[i].init(this, Body::Role::Soldier, Body::Type::Autonomous);
        }
    }

    Agent* agent() { return &soldiers_[0]; }
    zonas zonas_;
    std::vector<doors>* doors_;
    //Agent* ia() { return &ia_; }
  private:
      int numSoldiers = 10;
      std::vector<Agent> soldiers_;
      Agent soldier_;

      std::vector<zone> zones_;
      //Agent ia_;
};

#endif