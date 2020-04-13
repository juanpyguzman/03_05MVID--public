//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __GAME_H__
#define __GAME_H__ 1

#include "engine/scene.h""
#include "engine/sprite.h"
#include "ia/defines.h""
#include "ia/world.h""
#include "ia/pathfinding/pathfinding.h"

#include <SDL/SDL_ttf.h>

class Game {
  public:
    Game() {};
    ~Game() {};

    void init();
    void start();
    void shutdown();
  private:
    void handleInput(uint16_t& counter);
    void update(uint32_t dt);
    void render();

    void createScenes();
    void nextScene(int8_t sign);

    void loadZonesMap(SDL_Surface* map_image);

    bool quit_ = false;
    Sprite background_sprite_;
    Sprite fps_sprite_;
    TTF_Font* font_ = nullptr;

    int8_t curr_scene_ = 0;
    Scene* scenes_[SCENE_NUMBER];

    World world_;
    Map* map_;
    SDL_Surface* map_image_;
    zonas zonas_;

    int8_t slo_mo_ = 1;

    std::vector<doors> doors_{
        { 111,3, 117,3, 114,2, 114,4, false, false },
        { 19,30, 19,37, 18,34, 20,34, false, false },
        { 51,82, 51,87, 50,85, 52,85, false, false },
        { 84,54, 84,57, 83,56, 85,56, false, false },
    };
};

#endif