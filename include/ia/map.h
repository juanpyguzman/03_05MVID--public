//----------------------------------------------------------------------------
//                                                        _   ________  __
//  Copyright VIU 2020                                   | | / /  _/ / / /
//  Author: Ivan Fuertes <ivan.fuertes@campusviu.es>     | |/ // // /_/ /
//                                                       |___/___/\____/
//----------------------------------------------------------------------------

#ifndef __MAP_H__
#define __MAP_H__ 1

#include <cstdint>
#include <SDL/SDL.h>

#define MAP_L1_WIDTH 128                //Size of map in tiles
#define MAP_L1_HEIGHT 128

class Map {
  public:

    void loadMap() {
      SDL_Surface* map_image;
      if ((map_image = SDL_LoadBMP("../assets/images/costes.bmp")) == false)
        return;

      SDL_LockSurface(map_image);

      const uint8_t stride = map_image->pitch >> 2;

      {
        uint32_t* pixels = static_cast<uint32_t*>(map_image->pixels);
        for (int i = 0; i < MAP_L1_HEIGHT; ++i) {
          uint32_t* step_pixels = pixels;
          for (int j = 0; j < MAP_L1_WIDTH; ++j) {
            if ((*step_pixels & 0x00ffffff) == 0) {
              map_l1[j][i] = 1;     //no walkable
            } else {
              map_l1[j][i] = 0;      //walkable
            }

            step_pixels++;
          }
          pixels += stride;
        }
      }

      SDL_UnlockSurface(map_image);
    }

  private:

    //  Tile Map
    uint8_t map_l1[MAP_L1_WIDTH][MAP_L1_HEIGHT];
};

#endif