#include <SDL2/SDL.h>
#include <emscripten.h>
#include <iostream>

struct Context {
  std::string title;
  int width, height;
  SDL_Renderer * renderer;
  SDL_Event event;
  SDL_Rect rect, rect2;
  SDL_Texture * texture;
};

void callback(void * arg){
  Context * context = static_cast<Context*>(arg);
    while(SDL_PollEvent(&context->event)){
      if( context->event.type == SDL_QUIT ){
        exit(0);
      }
    }

    SDL_RenderClear(context->renderer);
    SDL_SetRenderDrawColor(context->renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(context->renderer, &context->rect);
    SDL_RenderPresent(context->renderer);
}

int main(int argc, char** argv) {
  Context context;
  SDL_Init(SDL_INIT_EVERYTHING);

  context.title = "SDL2 It's Works!";
  context.width = 1280;
  context.height = 720;

  SDL_Window* window = SDL_CreateWindow(
      context.title.c_str(),
      50, 30,
      context.width, context.height,
      SDL_WINDOW_SHOWN
      );

  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
  context.renderer = renderer;


  SDL_Surface* surface = SDL_GetWindowSurface(window);

  context.texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  context.rect.x = 10;
  context.rect.y = 10;
  context.rect.w = surface->w-20;
  context.rect.h = surface->h-20;

  emscripten_set_main_loop_arg(callback, &context, 0, 1);

  SDL_DestroyTexture(context.texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}