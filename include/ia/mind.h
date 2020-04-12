//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __MIND_H__
#define __MIND_H__ 1

#include <cstdint>
#include <ia\pathfinding\pathfinding.h>

class Body;
class World;

class Mind {
  public:
    Mind() {};
    ~Mind() {};

    void init(World* world, Body* body);
    void update(uint32_t dt) const;
    void changeDoor(doors door);
    void setDoors(std::vector<doors> doorsState);
    void setStartPoints(int x0, int y0);
    void setEndPoints(int xf, int yf);
    void getNextIter();

    bool endPath = true;
    Pathfinding pathfinding_;

  private:
    World* world_ = nullptr;
    Body* body_ = nullptr;
    doors doors_[4];
    std::list<pathPoints>* pathPointList_;
    std::list<pathPoints>::iterator list_iter;

    int x0_, y0_, xf_, yf_;

    void CalculatePathFinding();

};

#endif