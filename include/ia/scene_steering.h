//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __SCENE_SEEK_H__
#define __SCENE_SEEK_H__ 1

#include <engine/scene.h>
#include <engine/sprite.h>
#include <ia/agent.h>

class SceneSteering: public Scene {
  public:
    SceneSteering() {};
    ~SceneSteering() {};

    void init(World* world) final;
    void shutdown() final;

    void update(const uint32_t dt) final;
    void handleMouseEvent(const SDL_Event e) final;
    void handleKeyEvent(const SDL_Keycode key) final;
    void render() final;
};

#endif