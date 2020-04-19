//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "ia/mind.h"
#include "ia/body.h"
#include "ia/world.h"

void Mind::init(World* world, Body* body) {
  world_ = world;
  body_ = body;
  alert_ = world_->alert_;
  alert_time_ = world_->alert_time_;
}

void Mind::update(const uint32_t) const {
  //body_->setTarget(world_->target());
    if (!endPath && pathfinding_.isPath)
    {
        body_->setNextPoint(MathLib::Vec2(list_iter->x, list_iter->y));
    }
}

void Mind::changeDoor(doors door)
{
    pathfinding_.changeDoor(door);
}

void Mind::setDoors(std::vector<doors> doorsState) {
    pathfinding_.setDoors(doorsState);
}

void Mind::setStartPoints(int x0, int y0) {
    x0_ = x0;
    y0_ = y0;
}

void Mind::setEndPoints(int xf, int yf) {
    xf_ = xf;
    yf_ = yf;
    CalculatePathFinding();
}

void Mind::getNextIter() {
    list_iter++;
    if (list_iter == pathPointList_->end())
    {
        endPath = true;

    }
}

void Mind::stopPath() {
    endPath = true;
    pathfinding_.clear();
}

void Mind::CalculatePathFinding() {
    pathfinding_.setStart(x0_, y0_);
    pathfinding_.setEnd(xf_, yf_);
    pathfinding_.calculateA_Star();
    pathPointList_ = pathfinding_.getPathPointList();

    list_iter = pathPointList_->begin();
    endPath = false;
    pathfinding_.clear();

}

bool Mind::clearView(MathLib::Vec2 p0, MathLib::Vec2 pf)
{
    bool clearView=true;
    int x = p0.x() / 8;
    int y = p0.y() / 8;
    int pfx = pf.x() / 8;
    int pfy = pf.y() / 8;

    while (x != pfx || y != pfy)
    {
        if (x < pfx) x++;
        else if (x>pfx) x--;
  
        if (y < pfy) y++;
        else if (y>pfy) y--;

        //si encontramos que se trata de un muro
        if (pathfinding_.map_.getMap(x,y)==1) {
            clearView = false;
        }
    }


    return clearView;
}
