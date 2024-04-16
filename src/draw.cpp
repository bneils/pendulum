#include <SDL.h>
#include <stdio.h>
#include "draw.hpp"

extern SDL_Window* window;
extern SDL_Renderer* renderer;

void draw_circle(int x_c, int y_c, int r) {  
  int t1, t2, x, y;

  t1 = r / 16;
  x = r;
  y = 0;
  while (x >= y) {
    // pixel(x,y) to pixel(x,-y)
    // pixel(-x,y) to pixel(-x,-y)
    // pixel(y,x) to pixel(y,-x)
    // pixel(-y,x) to pixel(-y,-x)
    for (int i = y; i >= -y; --i) {
      SDL_RenderDrawPoint(renderer, x + x_c, i + y_c);
      SDL_RenderDrawPoint(renderer, -x + x_c, i + y_c);
    }
    for (int i = x; i >= -x; --i) {
      SDL_RenderDrawPoint(renderer, y + x_c, i + y_c);
      SDL_RenderDrawPoint(renderer, -y + x_c, i + y_c);
    }

    ++y;
    t1 += y;
    t2 = t1 - x;
    if (t2 >= 0) {
      t1 = t2;
      --x;
    }
  }
}
