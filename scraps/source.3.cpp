#include "SDL.h"
#include <math.h>       /* sin */
#include <vector>
#include <iostream>
using namespace std;
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

int main(int argc, char* argv[])
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow("scribbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (float n = 0 * PI / 12; n < 12; n += 1)
  {
    point p1, p2;
    p1.x = 500 + int(200 * sin(n * 2 * PI / 12));
    p1.y = 500 + int(200 * cos(n * 2 * PI / 12));
    p2.x = 500 + int(400 * sin(n * 2 * PI / 12));
    p2.y = 500 + int(400 * cos(n * 2 * PI / 12));
    //for (point p : plotline(p1, p2))
    //auto p = a.begin(); i != p.end(); ++p)
    for (vector<point>::interator p = vecFunc().begin(); (p != vecFunc().end()); ++p)
    {
      SDL_RenderDrawPoint(renderer, p.x, p.y);
    }
  }
  SDL_RenderPresent(renderer);
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

for (float n = 0 * PI / 12; n < 12; n += 1)
{
  point p1, p2;
  p1.x = 500 + int(200 * sin(n * 2 * PI / 12));
  p1.y = 500 + int(200 * cos(n * 2 * PI / 12));
  p2.x = 500 + int(400 * sin(n * 2 * PI / 12));
  p2.y = 500 + int(400 * cos(n * 2 * PI / 12));
  for (point p : plotline(p1, p2))
  {
    SDL_RenderDrawPoint(renderer, p.x, p.y);
  }
  //for (auto &p: plotline(p1, p2))

}