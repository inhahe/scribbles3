#include "SDL.h"
#include <math.h>       /* sin */

#define PI 3.14159265

int getPt(int n1, int n2, float perc)
{
  int diff = n2 - n1;

  return n1 + (diff * perc);
}

//for (float i = 0; i < 1; i += 0.01)
//{
//  // The Green Line
//  xa = getPt(x1, x2, i);
//  ya = getPt(y1, y2, i);
//  xb = getPt(x2, x3, i);
//  yb = getPt(y2, y3, i);
//
//  // The Black Dot
//  x = getPt(xa, xb, i);
//  y = getPt(ya, yb, i);
//
//  drawPixel(x, y, COLOR_RED);
//}
/// <summary>
/// 
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
/// 
/// 
/// 

void bhm_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2)
{
  int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
  dx = x2 - x1;
  dy = y2 - y1;
  dx1 = fabs(dx);
  dy1 = fabs(dy);
  px = 2 * dy1 - dx1;
  py = 2 * dx1 - dy1;
  if (dy1 <= dx1)
  {
    if (dx >= 0)
    {
      x = x1;
      y = y1;
      xe = x2;
    }
    else
    {
      x = x2;
      y = y2;
      xe = x1;
    }
    SDL_RenderDrawPoint(renderer, x, y);
    for (i = 0; x < xe; i++)
    {
      x = x + 1;
      if (px < 0)
      {
        px = px + 2 * dy1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
        {
          y = y + 1;
        }
        else
        {
          y = y - 1;
        }
        px = px + 2 * (dy1 - dx1);
      }
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
  else
  {
    if (dy >= 0)
    {
      x = x1;
      y = y1;
      ye = y2;
    }
    else
    {
      x = x2;
      y = y2;
      ye = y1;
    }
    SDL_RenderDrawPoint(renderer, x, y);
    for (i = 0; y < ye; i++)
    {
      y = y + 1;
      if (py <= 0)
      {
        py = py + 2 * dx1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
        {
          x = x + 1;
        }
        else
        {
          x = x - 1;
        }
        py = py + 2 * (dx1 - dy1);
      }
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
}

int main(int argc, char* argv[])
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow("scribbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  //SDL_SetRendererDrawColor, renderer, r, g, b);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (float n = 0; n < 16; n += 1)
    {
    bhm_line(renderer, 500, 500, int(500 + 400 * sin(n * 2 * PI / 16)), int(500 + 400 * cos(n * 2 * PI / 16)));
    }
//  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//  bhm_line(renderer, 500, 500, 500, 700);
//  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
//  bhm_line(renderer, 500, 500, 500, 300);

  SDL_RenderPresent(renderer); //show previous operations
  SDL_Event event;
  while (SDL_WaitEvent(&event)) 
    {
    if (event.type == SDL_QUIT)
      {
      SDL_DestroyWindow(window);
      SDL_DestroyRenderer(renderer);
      SDL_Quit();
      }
    }
  return 0;
  }