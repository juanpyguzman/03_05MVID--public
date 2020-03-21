//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __MIND_H__
#define __MIND_H__ 1

#include <cstdint>

class Body;
class World;

class Mind {
  public:
    Mind() {};
    ~Mind() {};

    void init(World* world, Body* body);
    void update(uint32_t dt) const;
  private:
    World* world_ = nullptr;
    Body* body_ = nullptr;
};

#endif