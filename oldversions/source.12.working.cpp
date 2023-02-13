#include "SDL.h"
#include <math.h>       /* sin */
#include <vector>
#include <cstdint>
#include <iostream>
#include <gif.h>
#include <cstring>
using namespace std;
#define PI 3.14159265

const int pointsperspacecurve = 100;
const int pointspertimecurve = 100;
int spacecurves = 30; 
int timecurves = 5;
int w = 1000;
int h = 1000;


uint8_t *image = new uint8_t[w * h * 4];

void SetPixel(int xx, int yy, uint8_t red, uint8_t grn, uint8_t blu)
{
  uint8_t* pixel = &image[(yy * w + xx) * 4];
  pixel[0] = red;
  pixel[1] = grn;
  pixel[2] = blu;
  pixel[3] = 255;  // no alpha for this demo
}

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

vector<point> beziercurve_quadratic(point p1, point p2, point p3, int pointspercurve)
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

vector<point> randanchors(int w, int h, int numpoints)
{
  vector<point> anchors;
  for (int i = 0; i < numpoints; i++)
  {
    point p;
    p.x = int(rand() / (static_cast <float> (RAND_MAX / w)));
    p.y = int(rand() / (static_cast <float> (RAND_MAX / h)));
    anchors.push_back(p);
  }
  return anchors;
}

vector<point> createpercloop(vector<point> anchors, int pointspercurve)
{
  vector<point> percpoints;
  vector<curve> curves;
  int s = anchors.size();
  for (int i = 0; i < s; i++)
  {
    point p1 = anchors[i];
    point p2 = anchors[(i + 1) % s];
    point p3 = anchors[(i + 2) % s];
    point p1c, p2c, p3c;
    p1c.x = p1.x + (p2.x - p1.x) / 2;
    p1c.y = p1.y + (p2.y - p1.y) / 2;
    p2c = p2;
    p3c.x = p2.x + (p3.x - p2.x) / 2;
    p3c.y = p2.y + (p3.y - p2.y) / 2;
    curves.push_back(curve{ p1c, p2c, p3c });
  }
  for (const auto& p : curves)
  {
    auto curvepoints2 = beziercurve_quadratic(p.p1, p.p2, p.p3, pointspercurve);
    percpoints.insert(percpoints.end(), curvepoints2.begin(), curvepoints2.end());
    vector<point>().swap(curvepoints2); //free memory
  }
  return percpoints;
}

vector<point> createdisploop(vector<point> percpoints)
{
  vector<point> disppoints;
  int s = percpoints.size();
  for (int i = 0; i < s; i++)
  {
    auto disppoints2 = plotline(percpoints[i], percpoints[(i + 1) % s]);
    disppoints.insert(disppoints.end(), disppoints2.begin(), disppoints2.end());
    vector<point>().swap(disppoints2); //free memory
  }
  vector<point>().swap(percpoints); //free memory
  return disppoints;
}

void drawscreen(SDL_Renderer* renderer, int w, int h, vector<point> disppoints, bool **screen, GifWriter& writer, bool dowrite)
{
  for (int x = 0; x < w; x++)
  {
    for (int y = 0; y < h; y++)
    {
      screen[x][y] = false;
    }
  }
  enum direction { none, up, down };
  direction ldir = none, dir;
  point lp = { -1, -1 };
  int s = disppoints.size();
  for (int i = 0; i < s * 2; i++)
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
        if (dowrite)
        {
          SetPixel(x, y, 0, 0, 255);
        }
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        if (dowrite)
        {
        SetPixel(x, y, 255, 255, 255);
        }
      }
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
  SDL_RenderPresent(renderer);
  if (dowrite)
  {
    GifWriteFrame(&writer, image, w, h, 2, 8, true);
  }
}

int main(int argc, char* argv[])
{
  SDL_Init(SDL_INIT_EVERYTHING);
    
  int seed = -1;
  bool dowrite = false;
  
  bool** screen = new bool* [w]; // each element is a pointer to an array.
  for (size_t i = 0; i < h; ++i)
  {
    screen[i] = new bool[h];
  }
  
  const char* filename = "";
  for (int i = 0; i < argc; i++) 
  {
    if (strcmp(argv[i], "--seed")==0)
    {
      if (i < argc - 1)
      {
        seed = atoi(argv[i + 1]);
      }
    }
    else if (strcmp(argv[i], "--file")==0)
    {
      if (i < argc - 1)
      {
        filename = argv[i + 1];
        dowrite = true;
      }
    }
    else if (strcmp(argv[i], "--spacecurves")==0)
    {
      if (i < argc - 1)
      {
        spacecurves = atoi(argv[i + 1]);
      }
    }
    else if (strcmp(argv[i], "--timecurves")==0)
    {
      if (i < argc - 1)
      {
        timecurves = atoi(argv[i + 1]);
      }
    }
    else if (strcmp(argv[i], "--w") == 0)
    {
      if (i < argc - 1)
      {
        w = atoi(argv[i + 1]);
      }
    }
    else if (strcmp(argv[i], "--h") == 0)
    {
      if (i < argc - 1)
      {
        h = atoi(argv[i + 1]);
      }
    }
  }

  SDL_Window* window = SDL_CreateWindow("scribbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  
  if (argc == 1)
  {
    std::cout << endl;
    std::cout << "Parameters:" << endl;
    std::cout << "  --seed <randomization seed>" << endl;
    std::cout << "  --file <output filename of animated gif>" << endl;
    std::cout << "  --spacepoints <number of curve anchorpoints in space" << endl;
    std::cout << "  --timepoints <number of curve anchorpoints in time per space anchorpoint>" << endl;
    std::cout << "  --w <screen width>" << endl;
    std::cout << "  --h <screen height" << endl;
    std::cout << endl;
    std::cout << "If you specify an output file, the animation will only loop once, and it won't let you close it until it's done." << endl;
    std::cout << "Otherwise, the animation will loop until you close it." << endl;
  }
  GifWriter writer = {};
  if (dowrite)
  {
    GifBegin(&writer, filename, w, h, 8, true);
  }

  if (seed == -1)
  {
    int t = time(NULL);
    std::cout << endl;
    std::cout << "seed: " << t << endl;
    srand(t);
  }
  else
  {
    srand(seed);
  }

  std::cout << endl;
  std::cout << "Made by Richard A. Nichols III (Inhahe)" << endl;

  vector<point> *timepercanchors = new vector<point>[spacecurves];
  for (int i = 0; i < spacecurves; i++)
  {
    auto timeanchors = randanchors(w, h, timecurves);
    auto timepercanchors2 = createpercloop(timeanchors, pointspertimecurve);
    timepercanchors[i] = timepercanchors2;
  }
  vector<point> dispanchors;
  
  for(;;)
  {
    for (int i2 = 0; i2 < timecurves * (pointspertimecurve + 1); i2++)
    { 
      for (int i = 0; i < spacecurves; i++)
      {
        dispanchors.push_back(timepercanchors[i][i2]);
      }
      drawscreen(renderer, w, h, createdisploop(createpercloop(dispanchors, pointsperspacecurve)), screen, writer, dowrite);
      
      vector<point>().swap(dispanchors);
      SDL_Event event;
      SDL_PollEvent(&event);
      if (filename == "")
      {
        if (event.type == SDL_QUIT)
        {
          SDL_DestroyWindow(window);
          SDL_DestroyRenderer(renderer);
          SDL_Quit();
          for (size_t i = 0; i < w; ++i)
          {
            delete screen[i];
          }
          //delete timepercanchors;
          delete image;
          return 0;
        }
      }
    }
    if (filename != "")
    {
      GifEnd(&writer);
      for (size_t i = 0; i < w; ++i)
      {
        delete screen[i];
      }
      //delete timepercanchors;
      delete image;
      return 0;
    }
  }
  return 0;
}

