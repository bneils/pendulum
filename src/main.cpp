#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <math.h>

#include "vector.hpp"
#include "mass.hpp"
#include "draw.hpp"

const int SCREEN_WIDTH = 480, SCREEN_HEIGHT = 480;
const double g = 9.81;
const double dt = 0.0001;

const int TARGET_FPS = 60;
const double TIME_SCALE = 1;
const int UPDATES_PER_FRAME = 1 / dt / TARGET_FPS * TIME_SCALE;

const double PENDULUM_DIAMETER = 0.03;
const double PENDULUM_MASS = 0.150;
const Vector PENDULUM_INITIAL_POS(-0.15, 0);

const float VIEW_BOT_Y = -0.17;
const float VIEW_HEIGHT = .34;
const float VIEW_LEFT_X = -0.17;
const float VIEW_WIDTH = .34;

// These are just some dummy values. No reason to tweak, unless you want to disable drag
const double DRAG_FLUID_DENSITY = 1.204;
//https://www.engineeringtoolbox.com/drag-coefficient-d_627.html
const double DRAG_COEFFICIENT = 0.5;
const double PENDULUM_FRONTAL_AREA = M_PI / 4 * PENDULUM_DIAMETER * PENDULUM_DIAMETER;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void world_to_screen(Vector *p, float x, float y, float w, float h, int *x_out, int *y_out) {
  if (p == NULL)
    return;
  if (x_out != NULL)
    *x_out = SCREEN_WIDTH * (p->x() - (double) x) / (double) w;
  if (y_out != NULL)
    *y_out = SCREEN_HEIGHT * (1 - (p->y() - (double) y) / (double) h);
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

  draw_circle(0, 0, 6);
  
  Mass pendulum {PENDULUM_MASS, PENDULUM_INITIAL_POS, Vector(0, 0)};  
  Vector center(0, 0);
  
  double radius = pendulum.position.sub(center).magnitude();

  bool quit = false;
  while (!quit) {
    for (int i = 0; i < UPDATES_PER_FRAME; ++i) {
      // The angle is used for separating the gravity components relative to r^
      Vector relative = pendulum.position.sub(center);
      double theta = (relative.y() != 0) ? abs(atan(relative.x() / relative.y())) : M_PI / 2;

      // r_vec points to the center
      Vector r_vec = center.sub(pendulum.position);
      double velocity = pendulum.velocity.magnitude();
      // Summation(F_r) = m(a_c) = F_c = m * v^2 / r = F_t - F_g*cos(theta)
      // F_t = mgcos(theta) + m/2v^2/r = m(gcos(theta) + v^2/r)
      Vector F_t = r_vec.normalize().scale(pendulum.mass * (g * cos(theta) + velocity * velocity / r_vec.magnitude()));
      
      Vector F_g(0, -pendulum.mass * g);
      Vector F_net = F_g.add(F_t);

      // Drag force, assuming the pendulum is a circle
      if (velocity != 0) {
	// area of 3d pendulum would be pi*r^2
	Vector F_d = pendulum.velocity.normalize().scale(0.5 * DRAG_FLUID_DENSITY * velocity * velocity * DRAG_COEFFICIENT * PENDULUM_FRONTAL_AREA);
	F_net = F_net.sub(F_d);
      }

      // Apply riemman rectangle sum step for velocity and position (approximation)
      pendulum.velocity = pendulum.velocity.add(F_net.scale(dt / pendulum.mass));
      pendulum.position = pendulum.position.add(pendulum.velocity.scale(dt));
    }

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &rect);

    int pend_px_x, pend_px_y, pivot_px_x, pivot_px_y;
    world_to_screen(&pendulum.position, VIEW_LEFT_X, VIEW_BOT_Y, VIEW_WIDTH, VIEW_HEIGHT, &pend_px_x, &pend_px_y);
    world_to_screen(&center, VIEW_LEFT_X, VIEW_BOT_Y, VIEW_WIDTH, VIEW_HEIGHT, &pivot_px_x, &pivot_px_y);

    // Draw pendulum, then pivot, then the line connecting them
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    rect = {pivot_px_x - 2, pivot_px_y - 2, 4, 4};
    SDL_RenderDrawRect(renderer, &rect);

    SDL_RenderDrawLine(renderer, pend_px_x, pend_px_y, pivot_px_x, pivot_px_y);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int pend_px_radius = PENDULUM_DIAMETER / 2 / VIEW_WIDTH * SCREEN_WIDTH;
    draw_circle(pend_px_x, pend_px_y, pend_px_radius);
    
    // Flush changes
    SDL_RenderPresent(renderer);

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) quit = true;
    }

    SDL_Delay(1000 / TARGET_FPS);
  }
  
  return 0;
}
