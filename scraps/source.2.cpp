void plotPixel(SDL_Renderer* renderer, int x1, int y1, int x2, int y2)
{  
  int dx, dy, decide;
  dx = abs(x2 - x1);
  dy = abs(y2 - y1);
  decide = (dx > dy) ? 0 : 1;
  // If slope is less than one
 
  // pk is initial decision making parameter
  // Note:x1&y1,x2&y2, dx&dy values are interchanged
  // and passed in plotPixel function so
  // it can handle both cases when m>1 & m<1
  int pk = 2 * dy - dx;
  for (int i = 0; i <= dx; i++) {
    // checking either to decrement or increment the
    // value if we have to plot from (0,100) to (100,0)
    x1 < x2 ? x1++ : x1--;
    if (pk < 0) {
      // decision value will decide to plot
      // either  x1 or y1 in x's position
      if (decide == 0) {
        SDL_RenderDrawPoint(renderer, x1, y1);
        pk = pk + 2 * dy;
      }
      else {
        //(y1,x1) is passed in xt
        SDL_RenderDrawPoint(renderer, y1, x1);
        pk = pk + 2 * dy;
      }
    }
    else {
      y1 < y2 ? y1++ : y1--;
      if (decide == 0) {

        SDL_RenderDrawPoint(renderer, x1, y1);
      }
      else {
        SDL_RenderDrawPoint(renderer, y1, x1);
      }
      pk = pk + 2 * dy - 2 * dx;
    }
  }
}


#include "SDL.h"
#include <math.h>       /* sin */
#include <vector>
#include <iostream>

#define PI 3.14159265

int getPt(int n1, int n2, float perc)
{
  int diff = n2 - n1;

  return n1 + int(diff * perc);
}

struct point
{
  int x;
  int y;
};

std::vector<point> plotline(point p1, point p2)
{
  std::vector<point> points;
  int dx, dy, dx1, dy1, px, py, xd, yd, x, y;
  point p;
  dx = p2.x - p1.x;
  dy = p2.y - p1.y;
  dx1 = abs(dx);
  dy1 = abs(dy);
  px = 2 * dy1 - dx1;
  py = 2 * dx1 - dy1;
  if (dy1 <= dx1)
  {
    xd = (p1.x < p2.x) ? 1 : -1;
    y = p1.y;
    x = p1.x;
    p.x = x;
    p.y = y;
    points.push_back(p);
    while (x != p2.x)
    {
      x += xd;
      if (px < 0)
      {
        px += 2 * dy1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
        {
          y += xd;
        }
        else
        {
          y -= xd;
        }
        px += 2 * (dy1 - dx1);
      }
      p.x = x;
      p.y = y;
      points.push_back(p);
    }
  }
  else
  {
    yd = (p1.y < p2.y) ? 1 : -1;
    y = p1.y;
    x = p1.x;
    p.x = x;
    p.y = y;
    points.push_back(p);
    while (y != p2.y)
    {
      y += yd;
      if (py <= 0)
      {
        py += 2 * dx1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
        {
          x += yd;
        }
        else
        {
          x -= yd;
        }
        py += 2 * (dx1 - dy1);
      }
      p.x = x;
      p.y = y;
      points.push_back(p);
    }
  }
  return points;
} 

void plotline(SDL_Renderer* renderer, point p1, point p2)
{
  int dx, dy, dx1, dy1, px, py, xd, yd, x, y;
  dx = p2.x - p1.x;
  dy = p2.y - p1.y;
  dx1 = abs(dx);
  dy1 = abs(dy);
  px = 2 * dy1 - dx1;
  py = 2 * dx1 - dy1;
  if (dy1 <= dx1)
  {
    xd = (p1.x < p2.x) ? 1 : -1;
    y = p1.y;
    x = p1.x;
    SDL_RenderDrawPoint(renderer, x, y);
    while (x != p2.x)
    {
      x += xd;
      if (px < 0)
      {
        px += 2 * dy1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
        {
          y += xd;
        }
        else
        {
          y -= xd;
        }
        px += 2 * (dy1 - dx1);
      }
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
  else
  {
    yd = (p1.y < p2.y) ? 1 : -1;
    y = p1.y;
    x = p1.x;
    SDL_RenderDrawPoint(renderer, x, y);
    while (y != p2.y)
    {
      y += yd;
      if (py <= 0)
      {
        py += 2 * dx1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
        {
          x += yd;
        }
        else
        {
          x -= yd;
        }
        py += 2 * (dx1 - dy1);
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
  for (float n = 15*PI/16; n < 16; n += 1)
  {
    point p1, p2;
    p1.x = 500 + 200 * int(sin(n * 2 * PI / 16));
    p1.y = 500 + 200 * int(cos(n * 2 * PI / 16));
    p2.x = 500 + 400 * int(sin(n * 2 * PI / 16));
    p2.y = 500 + 400 * int(cos(n * 2 * PI / 16));
    for (point p : plotline(p1, p2))
    {
      SDL_RenderDrawPoint(renderer, p.x, p.y);
    }
    SDL_RenderPresent(renderer);

    //  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //  bhm_line(renderer, 500, 500, 500, 700);
    //  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    //  bhm_line(renderer, 500, 500, 500, 300);
  }
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