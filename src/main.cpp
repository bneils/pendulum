#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <math.h>

#include "vector.hpp"
#include "mass.hpp"

const int SCREEN_WIDTH = 640, SCREEN_HEIGHT = 480;
const double g = 9.81;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Vector world_to_screen(Vector p, SDL_Rect *frame) {
  Vector result(SCREEN_WIDTH * (p.get_x() - frame->x) / frame->w, SCREEN_HEIGHT * (1 - (p.get_y() - frame->y) / frame->h));
  return result;
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }
  window = SDL_CreateWindow("Pendulum", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  Mass pendulum {50, Vector(-10, 0), Vector(0, 0)};
  const double PENDULUM_DIAMETER = 6;
  const double dt = 0.0001;
  const int TARGET_FPS = 60;
  const double TIME_SCALE = 1;
  const int UPDATES_PER_FRAME = 1 / dt / TARGET_FPS;
  
  Vector center(0, 0);
  double radius = pendulum.position.sub(center).magnitude();
  
  bool quit = false;
  while (!quit) {
    for (int i = 0; i < UPDATES_PER_FRAME * TIME_SCALE; ++i) {
      Vector relative = pendulum.position.sub(center);
      double theta = (relative.get_y() != 0) ? abs(atan(relative.get_x() / relative.get_y())) : M_PI / 2;
      Vector F_g(0, -pendulum.mass * g);
      Vector r_vec = center.sub(pendulum.position);
      double velocity = pendulum.velocity.magnitude();
      Vector F_t = r_vec.normalize().scale(pendulum.mass * (g * cos(theta) + velocity * velocity / r_vec.magnitude()));
      Vector F_net = F_g.add(F_t);
      if (velocity != 0) {
	Vector F_d = pendulum.velocity.normalize().scale(0.5 * 1.204 * velocity * velocity * 0.4 * PENDULUM_DIAMETER);
	F_net = F_net.sub(F_d);
      }
      pendulum.velocity = pendulum.velocity.add(F_net.scale(dt / pendulum.mass));
      pendulum.position = pendulum.position.add(pendulum.velocity.scale(dt));
    }
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &rect);

    rect = {-15, -15, 30, 30};
    Vector px_loc = world_to_screen(pendulum.position, &rect);
    Vector pivot_px_loc = world_to_screen(center, &rect);
    rect = {(int) px_loc.get_x() - 16, (int) px_loc.get_y() - 16, 32, 32};
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_Rect rect2 = {(int) pivot_px_loc.get_x(), (int) pivot_px_loc.get_y(), 2, 2};
    SDL_RenderDrawRect(renderer, &rect2);

    SDL_RenderDrawLine(renderer, rect.x + 16, rect.y + 16, rect2.x, rect2.y);
    
    SDL_RenderPresent(renderer);

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) quit = true;
    }

    SDL_Delay(1000 / TARGET_FPS);
  }
  
  return 0;
}
