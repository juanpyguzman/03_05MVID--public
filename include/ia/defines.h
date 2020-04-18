//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __DEFINES_H__
#define __DEFINES_H__ 1

#include <vector>
#include "mathlib/vec2.h"
#include <SDL/SDL_ttf.h>

#define GAME_NAME "05MVID"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

#define TICKS_PER_SECOND 30
#define MAX_FRAME_SKIP 10

#define FOREGROUND_COLOR { 0, 0, 0, 255 }
#define ALARM_COLOR { 0, 0, 0, 100 }
#define SHADOW_COLOR {160, 160, 160, 255}

#define FONT_FILE "../assets/fonts/8bit.ttf"
#define AGENT_BLUE_PATH "../assets/images/agent_blue.png"
#define AGENT_RED_PATH "../assets/images/agent_red.png"
#define AGENT_GREEN_PATH "../assets/images/agent_green.png"
#define AGENT_PURPLE_PATH "../assets/images/agent_purple.png"
#define AGENT_SOLDIER "../assets/images/soldier.bmp"
#define AGENT_GUARD "../assets/images/guard.bmp"
#define AGENT_SLAVE "../assets/images/slave.bmp"
#define BACKGROUND_MAP "../assets/images/mapa.bmp"
#define COSTES_MAP "../assets/images/costes.bmp"

#define FPS_FONT_SIZE 15
#define ALARM_FONT_SIZE 60

#define SCENE_NUMBER 1

struct KinematicSteering {
  MathLib::Vec2 velocity{ 0.0f, 0.0f };
  float rotation{ 0.0f };
};

struct Steering {
  MathLib::Vec2 linear{ 0.0f, 0.0f };    //linear acceleration
  float angular{ 0.0f };                //angular acceleration
};

struct KinematicStatus {
  MathLib::Vec2 position{ 0.0f, 0.0f };
  float orientation {0.0f};
  MathLib::Vec2 velocity{ 0.0f, 0.0f };  //linear velocity
  float rotation{0.0f};               //angular velocity

  float speed{ 0.0f };
};


//Estructura para el camino
struct pathPoints {
    int x;
    int y;
};

//Estructura para los nodos
class Node {
public:
    int x;
    int y;

    int g;
    int f;
    int h;

    int parentX;
    int parentY;

    Node(int x, int y) : x(x), y(y) {}
    Node() {}
};

class doors {
public:
    int startX;
    int startY;
    int endX;
    int endY;
    int outsideX;
    int outsideY;
    int insideX;
    int insideY;
    bool open;
    bool discovered;


    void renderDoor(doors door, SDL_Renderer* renderer) {
    if (door.open) {
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    }

    for (int i = door.startX; i <= door.endX; i++)
    {
        for (int j = door.startY; j <= door.endY; j++)
        {
            SDL_Rect rect;
            rect.x = i*8;
            rect.y = j*8;
            rect.h = 8;
            rect.w = 8;

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

};

enum class zone {
    Exterior,
    Interior,
    Rest,
    Work,
    Loading,
    Unloading,
    Base,
    Impossible,
};

class zonas {
public:
    std::vector<MathLib::Vec2> exterior;
    std::vector<MathLib::Vec2> interior;
    std::vector<MathLib::Vec2> rest;
    std::vector<MathLib::Vec2> work;
    std::vector<MathLib::Vec2> loading;
    std::vector<MathLib::Vec2> unloading;
    std::vector<MathLib::Vec2> base;
    std::vector<MathLib::Vec2> impossible;

};



#endif