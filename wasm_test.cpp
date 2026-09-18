#include <vector>
#include <cstdint>
#include <cstring>
#include <emscripten.h>
#include <SDL.h>
#include <iostream>

using namespace std;

SDL_Renderer* renderer = nullptr;
SDL_Window* window = nullptr;
SDL_Surface* surface = nullptr;

int w = 1000;
int h = 1000;

void prnjs(string h)
{
  string s = "alert('" + h + "')";
  emscripten_run_script(s.c_str());
}

void prnjs(string h, string e)
{
  string s = "alert('" + h + e + "')";
  emscripten_run_script(s.c_str());
}

void mainloop()
{
  SDL_RenderClear(renderer);

  SDL_Rect rect;
  rect.x = 250;
  rect.y = 150;
  rect.w = 200;
  rect.h = 200;

  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderDrawRect(renderer, &rect);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

  SDL_RenderPresent(renderer); 
  /*if (SDL_UpdateWindowSurface(window) < 0)
  {
    prnjs("SDL_UpdateWindowSurface(window): ", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  */
}

int main(int argc, char* argv[])
{ 
  prnjs("this is a test");
  cout << "this is a test" << endl;
  if (SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE) != SDL_TRUE) prnjs("Could not set vsync. It may not be available on your platform.");
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    prnjs("SDL_Init(SDL_INIT_EVERYTHING): ", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  window = SDL_CreateWindow("scribbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_ALLOW_HIGHDPI);
  if (window == NULL)
  {
    prnjs("SDL_CreateWindow(\"scribbles\", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_ALLOW_HIGHDPI): ", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  
  surface = SDL_GetWindowSurface(window);
  if (surface == NULL)
  {
    prnjs("SDL_GetWindowSurface(window): ", SDL_GetError());
    exit(EXIT_FAILURE);
  }
 
  emscripten_set_main_loop(mainloop, 0, 1);
}