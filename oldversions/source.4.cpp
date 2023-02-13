#include "SDL.h"
#include <math.h>       /* sin */
#include <vector>
#include <iostream>
using namespace std;
#define PI 3.14159265

struct point
{
  int x;
  int y;
};

struct curve
{
  point p1, p2, p3;
};

const int pointspercurve = 100;
const int numcurves = 30;
const int w = 1000;
const int h = 1000;
bool screen[w][h];

float getPt(float n1, float n2, float perc)
{
  float diff = n2 - n1;

  return n1 + diff * perc;
}

vector<point> beziercurve_quadratic(point p1, point p2, point p3)
{
  point p;
  vector<point> points;
  for (float i2 = 0; i2 < 1; i2 += 1.0 / static_cast <float>(pointspercurve))
  {
    float xa = getPt(p1.x, p2.x, i2);
    float ya = getPt(p1.y, p2.y, i2);
    float xb = getPt(p2.x, p3.x, i2);
    float yb = getPt(p2.y, p3.y, i2);
    p.x = int(getPt(xa, xb, i2));
    p.y = int(getPt(ya, yb, i2));
    points.push_back(p);
  }
  return points;
}

vector<point> plotline(point p1, point p2)
{
  vector<point> points;
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
      //cout << p.x << ", " << p.y << endl;
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
      //cout << p.x << ", " << p.y << endl;
      points.push_back(p);
    }
  }
  return points;
}

vector<point> curvepoints;
vector<point> percpoints;
vector<point> disppoints;
vector<curve> curves;
enum direction {none, up, down};

int main(int argc, char* argv[])
{
  srand(time(NULL)); 
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow("scribbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  point p;
  for (int i = 0; i < numcurves; i++)
  {
    p.x = (rand()) / (static_cast <float> (RAND_MAX / w));
    p.y = (rand()) / (static_cast <float> (RAND_MAX / w));
    curvepoints.push_back(p);
  }
  for (int i = 0; i < numcurves; i++)
  {
    point p1 = curvepoints[i];
    point p2 = curvepoints[(i + 1) % numcurves];
    point p3 = curvepoints[(i + 2) % numcurves];
    point p1c, p2c, p3c;
    p1c.x = p1.x + (p2.x - p1.x) / 2;
    p1c.y = p1.y + (p2.y - p1.y) / 2;
    p2c = p2;   
    p3c.x = p2.x + (p3.x - p2.x) / 2;
    p3c.y = p2.y + (p3.y - p2.y) / 2;
    curve c;
    c.p1 = p1c;
    c.p2 = p2c;
    c.p3 = p3c;
    curves.push_back(c);
  }
  direction ldir = none, dir;
  point lp;
  lp.x = -1;
  lp.y = -1;
  for (const auto& c : curves)
  {
    auto curvepoints2 = beziercurve_quadratic(c.p1, c.p2, c.p3);
    percpoints.insert(percpoints.end(), curvepoints2.begin(), curvepoints2.end());
    int s = percpoints.size();
  }
  int s = percpoints.size();
  for (int i = 0; i < s; i++)
  {
    auto disppoints2 = plotline(percpoints[i], percpoints[(i + 1) % s]);
    disppoints.insert(disppoints.end(), disppoints2.begin(), disppoints2.end());
  }
  s = disppoints.size();
  int ly = -1;
  for (int i = 0; s < s * 2; i++)
  {
    point p = disppoints[i%s];
    if (lp.y != -1 && p.y != lp.y)
    {
      dir = p.y < lp.y ? up : down;
      if (dir == ldir)
      {
        screen[lp.x][lp.y] = not screen[lp.x][lp.y];
        SDL_RenderDrawPoint(renderer, lp.x, lp.y);
      }
      if (i >= s)
      {
        break;
      }
      ldir = dir;
    }
    lp = p;
  }
  for (int y = 0; y < h; y++)
  {
    bool dot = false;
    for (int x = 0; x < w; x++)
    {
      if (screen[x][y])
      {
        dot = not dot;
      }
      if (dot)
      {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      }
      SDL_RenderDrawPoint(renderer, x, y);
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

