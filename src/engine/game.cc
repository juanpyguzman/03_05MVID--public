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

#include <cstdio>

void Game::init() {
  font_ = TTF_OpenFont(FONT_FILE, FPS_FONT_SIZE);
  if (!font_) {
    printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
  }

  fps_sprite_.setVisible(false);

  createScenes();
  world_.target()->getKinematic()->position = MathLib::Vec2(0.0f, 0.0f);
}

void Game::start() {
  uint32_t fps_time{ 0 };
  uint32_t fps_time_acc{ 0 };

  uint32_t next_game_tick = SDL_GetTicks();
  uint32_t update_loops = 0;
  uint32_t render_loops = 0;

  const uint32_t skip_ticks = 1000 / TICKS_PER_SECOND;

  while (!quit_) {
    uint32_t loops = 0;
    while ((SDL_GetTicks() > next_game_tick) && (loops < MAX_FRAME_SKIP)) {
      handleInput();
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

void Game::handleInput() {
  SDL_Event e;
  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT) {
      quit_ = true;
    }

    if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
      if (e.type == SDL_MOUSEBUTTONUP) {
        int x, y;
        SDL_GetMouseState(&x, &y);

        world_.target()->getKinematic()->position = Vec2(x, y);
      }
      scenes_[curr_scene_]->handleMouseEvent(e);
    }

    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_ESCAPE: quit_ = true; break;
        case SDLK_F1:   nextScene(-1); break;
        case SDLK_F2:  nextScene(+1); break;
        case SDLK_F3:
          slo_mo_ = clamp<int8_t>(++slo_mo_, 1, 10);
          printf("Slow Motion Set To %d\n", slo_mo_);
        break;
        case SDLK_F4:
          slo_mo_ = clamp<int8_t>(--slo_mo_, 1, 10);
          printf("Slow Motion Set To %d\n", slo_mo_);
        break;
        case SDLK_F5:
          DebugDraw::toggleEnabled();
          printf("Debug Draw Mode Changed\n");
        break;
        case SDLK_UP: {
          world_.target()->getKinematic()->speed += 20.0f;
          if (world_.target()->getKinematic()->speed > 140.0f) {
            world_.target()->getKinematic()->speed = 140.0f;
          }
          break; }
        case SDLK_DOWN: {
          world_.target()->getKinematic()->speed -= 20.0f;
          if (world_.target()->getKinematic()->speed <= 0.0f) {
            world_.target()->getKinematic()->speed = 0.0f;
          }
          break; }
        case SDLK_LEFT: {
          world_.target()->getKinematic()->orientation -= 0.2f;
          break;
        }
        case SDLK_RIGHT: {
          world_.target()->getKinematic()->orientation += 0.2f;
          break;
        }
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

  fps_sprite_.render();
  scenes_[curr_scene_]->render();
  world_.render();
  DebugDraw::render();

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