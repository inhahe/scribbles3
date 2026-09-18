#include <emscripten.h>
#include <SDL.h>

int width = 1000;
int height = 1000;

struct Context
{
  SDL_Renderer* renderer = nullptr;
  SDL_Window* window = nullptr;
  SDL_Surface* surface = nullptr;
  SDL_PixelFormat* pixel_format_surface = nullptr;
};

void mainloop(void* arg)
{ 
  Context* context = static_cast<Context*>(arg);
  SDL_RenderClear(context->renderer);
  uint8_t* pixels = (uint8_t*)(context->surface->pixels);
  int pitch = context->surface->pitch;
  int color = SDL_MapRGBA(context->pixel_format_surface, 255, 0, 0, 255);
  if (SDL_MUSTLOCK(context->surface)) SDL_LockSurface(context->surface);
  uint8_t* sp_pixels = nullptr;
  for (int y = 0; y < height; y++)
  {
    sp_pixels = pixels + y * pitch;
    for (int x = 0; x < width; x++)
    {
      *(uint32_t*)sp_pixels = color;
      sp_pixels += 4;
    }
  }
  if (SDL_MUSTLOCK(context->surface)) SDL_UnlockSurface(context->surface);
  SDL_UpdateWindowSurface(context->window);
}

int main(int argc, char* argv[])
{
  Context context;
  SDL_Init(SDL_INIT_VIDEO);
  context.window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
  context.surface = SDL_GetWindowSurface(context.window);
  context.pixel_format_surface = context.surface->format;
  emscripten_set_main_loop_arg(mainloop, &context, 0, 1);
}
