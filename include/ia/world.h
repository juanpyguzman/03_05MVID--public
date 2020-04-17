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
    };

    ~World() {
        for (int i = 0; i < numSoldiers; i++)
        {
            soldiers_[i].shutdown();
        }

        for (int i = 0; i < numGuards; i++)
        {
            guards_[i].shutdown();
        }

        for (int i = 0; i < numSlaves; i++)
        {
            slaves_[i].shutdown();
        }
    };

    void update(const float dt) { 
        for (int i = 0; i < numSoldiers; i++)
        {
            soldiers_[i].update(dt);
        }

        for (int i = 0; i < numGuards; i++)
        {
            guards_[i].update(dt);
        }

        for (int i = 0; i < numSlaves; i++)
        {
            slaves_[i].update(dt);
        }
    }

    void render() const { 
        for (int i = 0; i < numSoldiers; i++)
        {
            soldiers_[i].render();
        }

        for (int i = 0; i < numGuards; i++)
        {
            guards_[i].render();
        }

        for (int i = 0; i < numSlaves; i++)
        {
            slaves_[i].render();
        }
    }

    void setZones(zonas zonasMapa) {
        zonas_ = zonasMapa;
    }

    void setDoors(std::vector<doors>* doorsState)
    {
        doors_ = doorsState;
    }

    void setAlert(bool* alert, float* alert_time)
    {
        alert_ = alert;
        alert_time_ = alert_time;
    }
    
    void init() {
        
        //Inicialización de soldados
        for (int i = 0; i < numSoldiers; i++)
        {
            soldiers_.push_back(agent_);
        }
        
        for (int i = 0; i < numSoldiers; i++)
        {
            soldiers_[i].init(this, Body::Role::Soldier, Body::Type::Autonomous, i);
        }

        //Inicialización de guardias
        for (int i = 0; i < numGuards; i++)
        {
            guards_.push_back(agent_);
        }

        for (int i = 0; i < numGuards; i++)
        {
            guards_[i].init(this, Body::Role::Guard, Body::Type::Autonomous, i);
        }

        //Inicialización de esclavos
        for (int i = 0; i < numSlaves; i++)
        {
            slaves_.push_back(agent_);
        }

        for (int i = 0; i < numSlaves; i++)
        {
            slaves_[i].init(this, Body::Role::Slave, Body::Type::Autonomous, i);
        }
    }

    Agent* agent() { return &soldiers_[0]; }
    zonas zonas_;
    std::vector<doors>* doors_;
    bool* alert_;
    float* alert_time_;

  private:
      Agent agent_;
      
      int numSoldiers = 10;
      std::vector<Agent> soldiers_;

      int numGuards = 10;
      std::vector<Agent> guards_;

      int numSlaves = 10;
      std::vector<Agent> slaves_;
};

#endif