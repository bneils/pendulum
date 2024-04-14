#include <SDL.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

void set_pixel(int x, int y, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderDrawPoint(renderer, x, y);
}
