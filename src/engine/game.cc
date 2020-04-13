//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#include "engine/game.h"
#include "engine/debug_draw.h"
#include "engine/window.h"
#include "ia/defines.h"
#include "ia/scene_steering.h"
#include <chrono>
#include <iostream>
#include <stdlib.h>


void Game::init() {
  font_ = TTF_OpenFont(FONT_FILE, FPS_FONT_SIZE);
  if (!font_) {
    printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
  }

  fps_sprite_.setVisible(false);

  loadZonesMap(SDL_LoadBMP("../assets/images/zonas.bmp"));

  world_.setZones(zonas_);
  world_.setDoors(&doors_);
  world_.init();

  createScenes();

  background_sprite_.loadFromFile(BACKGROUND_MAP);
}

void Game::start() {
  uint32_t fps_time{ 0 };
  uint32_t fps_time_acc{ 0 };

  uint32_t next_game_tick = SDL_GetTicks();
  uint32_t update_loops = 0;
  uint32_t render_loops = 0;

  uint16_t counter = 0;

  const uint32_t skip_ticks = 1000 / TICKS_PER_SECOND;

  while (!quit_) {
    uint32_t loops = 0; 
    while ((SDL_GetTicks() > next_game_tick) && (loops < MAX_FRAME_SKIP)) {
      
      handleInput(counter);

      update(skip_ticks);

      next_game_tick += skip_ticks;
      ++loops;
      ++update_loops;
    }

    render();

    const uint32_t c_time = SDL_GetTicks();
    fps_time_acc += (c_time - fps_time);
    fps_time = c_time;
    ++render_loops;
    if (render_loops > 100) {        //show stats each 100 frames
      const float fps = 1000.0f / (fps_time_acc / 100.0f);
      const float ratio = static_cast<float>(render_loops) / static_cast<float>(update_loops);
      char text[255];
      sprintf_s(text, "%d RFPS      %d UFPS", static_cast<uint32_t>(fps), static_cast<uint32_t>(fps / ratio));
      fps_sprite_.loadFromRenderedText(text, SDL_Color FOREGROUND_COLOR, font_, true);
      fps_sprite_.setVisible(true);

      render_loops = 0;
      update_loops = 0;
      fps_time_acc = 0;
    }
  }
}

void Game::shutdown() {
  scenes_[curr_scene_]->shutdown();
  for (uint8_t i = 0; i < SCENE_NUMBER; ++i) {
    delete scenes_[i];
  }
}

void Game::handleInput(uint16_t& counter) {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT) {
      quit_ = true;
    }

    if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        if (e.type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (counter % 2 == 0)
        {
            std::cout << "Inicio de pathfinding -->  ";
            std::cout << "X: " << x << "          Y: " << y << std::endl;
            world_.agent()->mind_.setDoors(doors_);
            world_.agent()->mind_.setStartPoints(x, y);
            world_.agent()->getKinematic()->position = Vec2(x, y);
        }
        else
        {
            std::cout << "Final de pathfinding -->  ";
            std::cout << "X: " << x << "          Y: " << y << std::endl;
            world_.agent()->setBehaviour(Body::Behaviour::Search);
            world_.agent()->getKinematic()->speed = 20.0f;
            world_.agent()->mind_.setEndPoints(x, y);
        }

        counter++;
      }
      scenes_[curr_scene_]->handleMouseEvent(e);
    }

    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_ESCAPE: quit_ = true; break;
        case SDLK_F1:   nextScene(-1); break;
        case SDLK_F2:  nextScene(+1); break;
        case SDLK_1:
            doors_[0].open = !doors_[0].open;
            world_.agent()->mind_.changeDoor(doors_[0]);
            break;
        case SDLK_2:
            doors_[1].open = !doors_[1].open;
            world_.agent()->mind_.changeDoor(doors_[1]);
            break;
        case SDLK_3:
            doors_[2].open = !doors_[2].open;
            world_.agent()->mind_.changeDoor(doors_[2]);
            break;
        case SDLK_4:
            doors_[3].open = !doors_[3].open;
            world_.agent()->mind_.changeDoor(doors_[3]);
            break;
        default:{}
      }
      scenes_[curr_scene_]->handleKeyEvent(e.key.keysym.sym);
    }
  }
}

void Game::update(const uint32_t dt) {
  scenes_[curr_scene_]->update(dt / slo_mo_);
  world_.update(dt / slo_mo_);
}

void Game::render() {
  SDL_Renderer* renderer = Window::instance().getRenderer();
  SDL_SetRenderDrawColor(renderer, 0xD0, 0xD0, 0xD0, 0xFF);
  SDL_RenderClear(renderer);

  background_sprite_.setVisible(true);
  background_sprite_.render();

  fps_sprite_.render();
  scenes_[curr_scene_]->render();
  world_.render();
  DebugDraw::render();

  //Renderizamos el estado de las puertas
  doors_[0].renderDoor(doors_[0], renderer);
  doors_[1].renderDoor(doors_[1], renderer);
  doors_[2].renderDoor(doors_[2], renderer);
  doors_[3].renderDoor(doors_[3], renderer);

  SDL_RenderPresent(renderer);
}

void Game::createScenes() {
  scenes_[0] = new SceneSteering();
  nextScene(0);
}

void Game::nextScene(const int8_t sign) {
  scenes_[curr_scene_]->shutdown();
  curr_scene_ += sign;
  curr_scene_ = clamp<int8_t>(curr_scene_, 0, SCENE_NUMBER - 1);
  scenes_[curr_scene_]->init(&world_);
}

void Game::loadZonesMap(SDL_Surface* map_image) {
    if ((map_image = SDL_LoadBMP("../assets/images/zonas.bmp")) == false)
        return;

    map_image_ = map_image;

    SDL_LockSurface(map_image);
    Uint32* pixels = (Uint32*)map_image->pixels;
    Uint32 pixel;
    Uint8 r, g, b;

    for (int i = 0; i < MAP_L1_WIDTH; ++i) {
        for (int j = 0; j < MAP_L1_HEIGHT; ++j) {
            pixel = pixels[(j * map_image->w) + i];
            SDL_GetRGB(pixel, map_image->format, &r, &g, &b);
            //Exteriores del castillo
            if ((r == 239) && (g == 228) && (b == 176))
            {
                zones.push_back(zone::Exterior);
                zonas_.exterior.push_back(MathLib::Vec2(i, j));
            }
            //Interior del castillo
            else if ((r == 163) && (g == 73) && (b == 164)) {
                zones.push_back(zone::Interior);
                zonas_.interior.push_back(MathLib::Vec2(i, j));
            }
            //햞ea de descanso
            else if ((r == 181) && (g == 230) && (b == 29)) {
                zones.push_back(zone::Rest);
                zonas_.rest.push_back(MathLib::Vec2(i, j));
            }
            //햞ea de trabajo
            else if ((r == 255) && (g == 216) && (b == 0)) {
                zones.push_back(zone::Work);
                zonas_.work.push_back(MathLib::Vec2(i, j));
            }
            //햞ea de carga (izquierda)
            else if ((r == 36) && (g == 255) && (b == 0)) {
                zones.push_back(zone::Loading);
                zonas_.loading.push_back(MathLib::Vec2(i, j));
            }
            //햞ea de descarga (derecha)
            else if ((r == 18) && (g == 0) && (b == 255)) {
                zones.push_back(zone::Unloading);
                zonas_.unloading.push_back(MathLib::Vec2(i, j));
            }
            //Cuarteles (base de los soldados)
            else if ((r == 0) && (g == 198) && (b == 255)) {
                zones.push_back(zone::Base);
                zonas_.base.push_back(MathLib::Vec2(i, j));
            }
            else {
                zones.push_back(zone::Impossible);
                zonas_.impossible.push_back(MathLib::Vec2(i, j));
            }
        }
    }

    SDL_UnlockSurface(map_image);
}