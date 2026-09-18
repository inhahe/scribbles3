#include <SDL2/SDL.h>
#include <emscripten.h>
#include <iostream>

  std::string title;
  int width, height;
  SDL_Renderer* renderer;
  SDL_Event event;
  SDL_Rect rect;

void callback(){
    while(SDL_PollEvent(&event)){
      if(event.type == SDL_QUIT ){
        exit(0);
      }
    }

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv) {

  SDL_Init(SDL_INIT_EVERYTHING);

  title = "SDL2 It's Works!";
  width = 1280;
  height = 720;

  SDL_Window* window = SDL_CreateWindow(
      title.c_str(),
      50, 30,
      width, height,
      SDL_WINDOW_SHOWN
      );

  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
  renderer = renderer;


  SDL_Surface* surface = SDL_GetWindowSurface(window);

  texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  rect.x = 10;
  rect.y = 10;
  rect.w = surface->w-20;
  rect.h = surface->h-20;

  emscripten_set_main_loop(callback, 0, 1);

  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}