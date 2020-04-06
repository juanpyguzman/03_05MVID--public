//PATHFINDING BY JUAN PABLO GUZMÁN FERNÁNDEZ

#ifndef __PATHFINDING_H__
#define __PATHFINDING_H__ 1

#include "ia/defines.h"
#include "ia/map.h"
#include "engine/math.h"
#include <list>
#include <queue>

class Pathfinding {
public:
    Pathfinding() {      
        map_.loadMap(SDL_LoadBMP("../assets/images/costes.bmp"));
        for (int i = 0; i < MAP_L1_WIDTH; ++i) {
            for (int j = 0; j < MAP_L1_HEIGHT; ++j) {
                costs_[i][j] = map_.getMap(i, j);
            };
        }
    }

    virtual ~Pathfinding() {}

    void calculate(const KinematicStatus& character, const KinematicStatus* target, Steering* steering);
    void setStart(int x, int y);
    void setEnd(int x, int y);
    void calculateLinearPath();
    void calculateA_Star();
    void enableRenderPathfinding();
    void render();
    void clear();

private:
    int x0_, y0_;
    int xf_, yf_;
    
    std::priority_queue<Node, std::vector<Node>, std::greater<Node> > openList_;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node> > closedList_;
    Node Center, Node0, NodeF;
    Node node[MAP_L1_WIDTH][MAP_L1_HEIGHT];
    Node nextNode_;

    Map map_;
    uint8_t costs_[MAP_L1_WIDTH][MAP_L1_HEIGHT];

    std::list<pathPoints> pathPointList_;
    pathPoints aux_;
    
    bool enableRender_ = false;

    void calculateAdjacent(Node center, int i, int j);
    int heuristic(int x, int y);
    void backPath();

};
#endif