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
      target_.init(this, Body::Color::Red, Body::Type::Manual);
      for (uint16_t i = 0; i < (sizeof(ia_) / sizeof(*ia_)); ++i)
      {
          ia_[i].init(this, Body::Color::Green, Body::Type::Autonomous);
          ia_[i].getKinematic()->position = Vec2(WINDOW_WIDTH / (2+i), WINDOW_HEIGHT / (2+i));
      }
    };
    ~World() {
      target_.shutdown();
      for (uint16_t i = 0; i < (sizeof(ia_) / sizeof(*ia_)); ++i)
      {
          ia_[i].shutdown();
      }
    };

    void update(const float dt) { 
        target_.update(dt); 
        for (uint16_t i = 0; i < (sizeof(ia_) / sizeof(*ia_)); ++i)
        {
            ia_[i].update(dt);
        }
    }
    
    void render() const {
        target_.render();
        for (uint16_t i = 0; i < (sizeof(ia_) / sizeof(*ia_)); ++i)
        {
            ia_[i].render();
        }
    }

    uint16_t numIA() { return (sizeof(ia_) / sizeof(*ia_)); }


    Agent* target() { return &target_; }
    Agent* ia(uint16_t i) { return &ia_[i]; }
    Agent* agentsIA() { return ia_; }

  private:
    Agent target_;
    Agent ia_[10]; //Definimos aquí el número de Agentes de la IA
};

#endif