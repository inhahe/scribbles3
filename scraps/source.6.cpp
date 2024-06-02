#include "SDL.h"
#include <math.h>       /* sin */
#include <vector>
#include <iostream>
using namespace std;
#define PI 3.14159265

const int pointspercurve = 100;
const int numcurves = 30;
const int w = 1000;
const int h = 1000;
bool screen[w][h];

struct point
{
  int x;
  int y;
};

struct curve
{
  point p1, p2, p3;
};

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

class metapoints
{
  curve c1;
  curve c2;
  vector<point> curvepoints2;
  int pointindex = 0;
  int cpsize;
  point lastpoint;
public:
  metapoints()
  {
  }
  metapoints(const int w, const int h, int pointspercurve)
  {
    c1.p1.x = (rand()) / (static_cast <float> (RAND_MAX / w));
    c1.p1.y = (rand()) / (static_cast <float> (RAND_MAX / h));
    c1.p2.x = (rand()) / (static_cast <float> (RAND_MAX / w));
    c1.p2.y = (rand()) / (static_cast <float> (RAND_MAX / h));
    c1.p3.x = (rand()) / (static_cast <float> (RAND_MAX / w));
    c1.p3.y = (rand()) / (static_cast <float> (RAND_MAX / h));
    c2.p1.x = c1.p1.x + (c1.p2.x - c1.p1.x) / 2;
    c2.p1.y = c1.p1.y + (c1.p2.y - c1.p1.y) / 2;
    c2.p2 = c1.p2;
    c2.p3.x = c1.p3.x + (c1.p3.x - c1.p2.x) / 2;
    c2.p3.y = c1.p3.y + (c1.p3.y - c1.p2.y) / 2;
    auto curvepoints = beziercurve_quadratic(c2.p1, c2.p2, c2.p3);
    lastpoint = curvepoints.back();
    point lp;
    lp.x = -1;
    lp.y = -1;
    for(auto p: curvepoints)
    {
      if (lp.x != -1)
      {
        auto linepoints = plotline(p, lp);
        curvepoints2.insert(curvepoints2.end(), linepoints.begin(), linepoints.end());
        vector<point>().swap(linepoints); //free memory
      }
      lp = p;
    }
    vector<point>().swap(curvepoints); //free memory
  } 
    
  point getpoint()
  {
    cout << "pointindex: " << pointindex << endl;
    cout << "cpsize: " << cpsize << endl;
    if (pointindex >= cpsize)
    {
      vector<point>().swap(curvepoints2);
      c1.p1 = c1.p2;
      c1.p2 = c1.p3;
      c1.p3.x = (rand()) / (static_cast <float> (RAND_MAX / w));
      c1.p3.y = (rand()) / (static_cast <float> (RAND_MAX / h));
      c2.p1.x = c1.p1.x + (c1.p2.x - c1.p1.x) / 2;
      c2.p1.y = c1.p1.y + (c1.p2.y - c1.p1.y) / 2;
      c2.p2 = c1.p2;
      c2.p3.x = c1.p3.x + (c1.p3.x - c1.p2.x) / 2;
      c2.p3.y = c1.p3.y + (c1.p3.y - c1.p2.y) / 2;
      pointindex = 0;
      auto curvepoints = beziercurve_quadratic(c2.p1, c2.p2, c2.p3);
      point lp = lastpoint;
      for (auto p : curvepoints)
      {
        auto linepoints = plotline(p, lp);
        curvepoints2.insert(curvepoints2.end(), linepoints.begin(), linepoints.end());
        vector<point>().swap(linepoints); //free memory
        lp = p;
      }
      cpsize = curvepoints2.size();
      cout << "curvepoints2.size(): " << curvepoints2.size() << endl;
    }
    else
    {
      pointindex++;
      cout << "got here" << endl;

      cout << curvepoints2[pointindex].x << " " << curvepoints2[pointindex].y << endl;
      return curvepoints2[pointindex];
    }
  }
  ~metapoints()
  {
    vector<point>().swap(curvepoints2); //free memory
  }
};

void drawscreen(SDL_Renderer* renderer, int w, int h, point curvepoints[])
{
  for (int x = 0; x < w; x++)
  {
    for (int y = 0; y < h; y++)
    {
      screen[x][y] = false;
    }
  }
  vector<point> percpoints;
  vector<point> disppoints;
  vector<curve> curves;
  enum direction { none, up, down };
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
    vector<point>().swap(curvepoints2); //free memory
  }
  int s = percpoints.size();
  for (int i = 0; i < s; i++)
  {
    auto disppoints2 = plotline(percpoints[i], percpoints[(i + 1) % s]);
    disppoints.insert(disppoints.end(), disppoints2.begin(), disppoints2.end());
    vector<point>().swap(disppoints2); //free memory
  }
  s = disppoints.size();
  int ly = -1;
  for (int i = 0; s < s * 2; i++)
  {
    point p = disppoints[i % s];
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
  //free memory
  vector<point>().swap(percpoints);
  vector<point>().swap(disppoints);
  vector<curve>().swap(curves);
}

int main(int argc, char* argv[])
{
  srand(time(NULL)); 
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window = SDL_CreateWindow("scribbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  point p;
  point curvepoints[numcurves]; 
  metapoints mps[numcurves];
  for (int i = 0; i < numcurves; i++)
  {
    mps[i] = metapoints(w, h, pointspercurve);
  }
  for (int i = 0; i < numcurves; i++)
  {
    curvepoints[i] = mps->getpoint();
  }
 
  drawscreen(renderer, w, h, curvepoints);
  
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

