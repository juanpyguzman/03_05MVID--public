//PATHFINDING BY JUAN PABLO GUZMÁN FERNÁNDEZ

#include "ia/pathfinding/pathfinding.h"
#include <engine\window.h>

    
    void Pathfinding::setStart(int x, int y) {
        x0_ = x;
        y0_ = y;
        Node0.x = x / (WINDOW_WIDTH / MAP_L1_WIDTH);
        Node0.y = y / (WINDOW_WIDTH / MAP_L1_WIDTH);
        Node0.g = 0;
        node[Node0.x][Node0.y] = Node0;
    }
    
    void Pathfinding::setEnd(int x, int y) {
        xf_ = x;
        yf_ = y;
        NodeF.x = x / (WINDOW_WIDTH / MAP_L1_WIDTH);
        NodeF.y = y / (WINDOW_WIDTH / MAP_L1_WIDTH);
        Node0.h = heuristic(Node0.x, Node0.y);
        Node0.f = Node0.g + Node0.h;
        node[NodeF.x][NodeF.y].x = NodeF.x;
        node[NodeF.x][NodeF.y].y = NodeF.y;
        pathPointList_.clear();
    }

    void Pathfinding::calculateLinearPath() {
        if (x0_ - xf_ <= 0)
        {
            for (int i = 0; i < abs(x0_ - xf_); i++)
            {
                aux_.x = x0_ + i;
                aux_.y = y0_;
                pathPointList_.push_back(aux_);
            }
        } else {
            for (int i = 0; i < abs(x0_ - xf_); i++)
            {
                aux_.x = x0_ - i;
                aux_.y = y0_;
                pathPointList_.push_back(aux_);
            }
        }

        if (y0_ - yf_ <= 0) {
            for (int i = 0; i < abs(y0_ - yf_); i++)
            {
                aux_.x = xf_;
                aux_.y = y0_ + i;
                pathPointList_.push_back(aux_);
            }
        }
        else {
            for (int i = 0; i < abs(y0_ - yf_); i++)
            {
                aux_.x = xf_;
                aux_.y = y0_ - i;
                pathPointList_.push_back(aux_);
            }
        }

    }

    void Pathfinding::calculateA_Star(){

        openList_.push(Node0);

        while (Center.x != NodeF.x || Center.y != NodeF.y) {
            closedList_.push(openList_.top());
            Center = openList_.top();
            openList_.pop();
            costs_[Center.x][Center.y] = 1;

            calculateAdjacent(Center, 1, 0);
            calculateAdjacent(Center, 1, 1);
            calculateAdjacent(Center, 0, 1);
            calculateAdjacent(Center, -1, 1);
            calculateAdjacent(Center, -1, 0);
            calculateAdjacent(Center, -1, -1);
            calculateAdjacent(Center, 0, -1);
            calculateAdjacent(Center, 1, -1);

            if (!openList_.empty()) {
                if (openList_.top().g > node[openList_.top().x][openList_.top().y].g) {
                    openList_.pop();
                }
            } else if (openList_.empty()) {
                std::cout << "Camino imposible" << std::endl;
                break;
            }
        };

        if (!openList_.empty()) {
            backPath();
        }
    }

    void Pathfinding::calculateAdjacent(Node center, int i, int j)
    {
        if (center.x + i >= 0 && center.x + i <= 127 && center.y + j >= 0 && center.y + j <= 127)
        {
            //Si es transitable o no está en la lista cerrada
            if (costs_[center.x + i][center.y + j] != 1) {
                //Si no está en la lista abierta
                if (costs_[center.x + i][center.y + j] != 2) {
                    //Calculamos propiedades del nodo adyacente
                    node[center.x + i][center.y + j].x = center.x + i;
                    node[center.x + i][center.y + j].y = center.y + j;
                    if (abs(i) + abs(j) > 1) {
                        node[center.x + i][center.y + j].g = center.g + 14;
                    }
                    else {
                        node[center.x + i][center.y + j].g = center.g + 10;
                    }
                    node[center.x + i][center.y + j].h = heuristic(center.x + 1, center.y + j);
                    node[center.x + i][center.y + j].f = node[center.x + i][center.y + j].g + node[center.x + i][center.y + j].h;
                    node[center.x + i][center.y + j].parentX = center.x;
                    node[center.x + i][center.y + j].parentY = center.y;

                    //Añadimos el nodo a lista abierta
                    openList_.push(node[center.x + i][center.y + j]);
                    costs_[center.x + i][center.y + j] = 2;
                }
                //Si ya está en la lista abierta
                else if (costs_[center.x + i][center.y + j] == 2)
                {
                    //Si el "G" nuevo es menor
                    if (node[center.x + i][center.y + j].g > center.g + 14) {
                        //Actualizamos G
                        node[center.x + i][center.y + j].g = center.g + 14;

                        //Añadimos el nodo a lista abierta
                        openList_.push(node[center.x + i][center.y + j]);
                        costs_[center.x + i][center.y + j] = 2;
                    }
                }
            }
        }     
    }


    int Pathfinding::heuristic(int x, int y)
    {
        //Manhattan
        return 10*(abs(NodeF.x - x) + abs(NodeF.y - y));

        //Diagonal
        /*const uint16_t xDist = NodeF.x - x;
        const uint16_t yDist = NodeF.y - y;
        return (10 * (xDist - yDist)) + ((14 - (10 * 2)) * std::min(xDist, yDist));*/
    }

    void Pathfinding::backPath() {
        nextNode_ = node[NodeF.x][NodeF.y];
        while(nextNode_.parentX != Node0.x || nextNode_.parentY != Node0.y) {
            nextNode_ = node[nextNode_.parentX][nextNode_.parentY];
            aux_.x = nextNode_.x*8;
            aux_.y = nextNode_.y*8;
            pathPointList_.push_back(aux_);
        };
    }

    void Pathfinding::clear() {
        
        while (!openList_.empty())
        {
            openList_.pop();
        }

        while (!closedList_.empty())
        {
            closedList_.pop();
        }

        for (int i = 0; i < MAP_L1_WIDTH; ++i) {
            for (int j = 0; j < MAP_L1_HEIGHT; ++j) {
                costs_[i][j] = map_.getMap(i, j);
                node[i][j].x = 0;
                node[i][j].y = 0;
                node[i][j].f = 0;
                node[i][j].g = 0;
                node[i][j].h = 0;
                node[i][j].parentX = 0;
                node[i][j].parentY = 0;
            };
        }
    }

    void Pathfinding::enableRenderPathfinding() {
        enableRender_ = !enableRender_;
    }

    void Pathfinding::render() {
        if (enableRender_ = true) {
            SDL_Renderer* renderer = Window::instance().getRenderer();
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);

            for (std::list<pathPoints>::iterator list_iter = pathPointList_.begin();
                list_iter != pathPointList_.end(); list_iter++) {
                SDL_Rect rect;
                rect.x = list_iter->x;
                rect.y = list_iter->y;
                rect.h = 8;
                rect.w = 8;
                SDL_RenderFillRect(renderer, &rect);
            }

            enableRenderPathfinding();
            
        }

    }

    bool operator>(const Node& node1, const Node& node2)
    {

        // this will return true when second node  
        // has less "F". Then the object which 
        // have mininum "F" will be at the top(or  
        // max priority) 
        return node1.f > node2.f;
    }