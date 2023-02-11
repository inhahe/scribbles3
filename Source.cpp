#include "SDL.h"
#include <math.h>       /* sin */
#include <vector>
#include <cstdint>
#include <iostream>
#include <gif.h>
#include <cstring>
#include <base64.h>
using namespace std;
#define PI 3.14159265

int pointsperspacecurve = 100;
int pointspertimecurve = 100;

int spacecurves = 30; 
int timecurves = 5;
int w = 1000;
int h = 1000;
int bg_r = 255, bg_g = 255, bg_b = 255, fg_r = 0, fg_g = 0, fg_b = 255;

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

class metapoints
{
  curve c1;
  curve c2;
  vector<point> curvepoints2;
  vector<point> curvepoints;
  int pointindex = 0;
  int cpsize;
  point lastpoint;
  int pointspercurve;
  bool contiguous;
public:
  metapoints()
  {
  }
  metapoints(const int w, const int h, int pointspercurve, bool contiguous)
  {
    this->contiguous = contiguous;
    this->c1.p1.x = (rand()) / (static_cast <float> (RAND_MAX / w));
    this->c1.p1.y = (rand()) / (static_cast <float> (RAND_MAX / h));
    this->c1.p2.x = (rand()) / (static_cast <float> (RAND_MAX / w));
    this->c1.p2.y = (rand()) / (static_cast <float> (RAND_MAX / h));
    this->c1.p3.x = (rand()) / (static_cast <float> (RAND_MAX / w));
    this->c1.p3.y = (rand()) / (static_cast <float> (RAND_MAX / h));
    this->c2.p1.x = (this->c1.p2.x + this->c1.p1.x) / 2;
    this->c2.p1.y = (this->c1.p2.y + this->c1.p1.y) / 2;
    this->c2.p2 = this->c1.p2;
    this->c2.p3.x = (this->c1.p3.x + this->c1.p2.x) / 2;
    this->c2.p3.y = (this->c1.p3.y + this->c1.p2.y) / 2;
    this->pointspercurve = pointspercurve;
    this->curvepoints = beziercurve_quadratic(this->c2.p1, this->c2.p2, this->c2.p3, this->pointspercurve);
    this->lastpoint = curvepoints.back();
    if (this->contiguous)
    {
      point lp = { -1, -1 };
      for (auto& p : this->curvepoints)
      {
        if (lp.x != -1)
        {
          auto linepoints = plotline(lp, p);
          this->curvepoints2.insert(this->curvepoints2.end(), linepoints.begin(), linepoints.end());
          vector<point>().swap(linepoints); //free memory
        }
        lp = p;
      }
      this->cpsize = this->curvepoints2.size();
      vector<point>().swap(curvepoints); //free memory
    }
    else
    {
      this->cpsize = this->curvepoints.size();
    }
  }

  point getpoint()
  {
    if (this->pointindex >= this->cpsize)
    {
      vector<point>().swap(this->curvepoints2);
      this->c1.p1 = this->c1.p2;
      this->c1.p2 = this->c1.p3;
      this->c1.p3.x = (rand()) / (static_cast <float> (RAND_MAX / w));
      this->c1.p3.y = (rand()) / (static_cast <float> (RAND_MAX / h));
      this->c2.p1.x = (this->c1.p2.x + this->c1.p1.x) / 2;
      this->c2.p1.y = (this->c1.p2.y + this->c1.p1.y) / 2;
      this->c2.p2 = this->c1.p2;
      this->c2.p3.x = (this->c1.p3.x + this->c1.p2.x) / 2;
      this->c2.p3.y = (this->c1.p3.y + this->c1.p2.y) / 2;
      this->pointindex = 0;
      curvepoints = beziercurve_quadratic(this->c2.p1, this->c2.p2, this->c2.p3, this->pointspercurve);
      if (this->contiguous)
      {
        point lp = this->lastpoint;
        for (auto& p : curvepoints)
        {
          auto linepoints = plotline(lp, p);
          this->curvepoints2.insert(this->curvepoints2.end(), linepoints.begin(), linepoints.end());
          vector<point>().swap(linepoints); //free memory
          lp = p;
        }
        this->lastpoint = curvepoints.back();
        vector<point>().swap(curvepoints);
        this->cpsize = this->curvepoints2.size();
        return this->curvepoints2[this->pointindex];
      }
      else
      {
        this->cpsize = this->curvepoints.size();
        return this->curvepoints[this->pointindex];
      }
    }
    else
    {
      if (this->contiguous)
      {
        return this->curvepoints2[this->pointindex++];
      }
      else
      {
        return this->curvepoints[this->pointindex++];
      }
    }
  }
  ~metapoints()
  {
    vector<point>().swap(curvepoints2); //free memory
  }
};

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

void drawscreen(SDL_Renderer* renderer, int w, int h, vector<point> disppoints, bool *screen, GifWriter& writer, bool dowrite)
{
  int sp = 0;
  for (int y = 0; y < h; y++)
  {
    for (int x = 0; x < w; x++)
    {
      screen[++sp] = false;
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
        sp = lp.y * w + lp.x;
        screen[sp] = not screen[sp];
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
  sp = 0;
  for (int y = 0; y < h; y++)
  {
    bool dot = false;
    for (int x = 0; x < w; x++)
    {
      if (screen[++sp])
      {
        dot = not dot;
      }
      if (dot)
      {
        SDL_SetRenderDrawColor(renderer, fg_r, fg_g, fg_b, 255);
        if (dowrite)
        {
          SetPixel(x, y, fg_r, fg_g, fg_b);
        }
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, bg_r, bg_g, bg_b, 255);
        if (dowrite)
        {
        SetPixel(x, y, bg_r, bg_g, bg_b);
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

string parameters = "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";
string myname = "JAgNDEkLEEk7AAoBCBsNSShHSScACgEGBRpJICAgSUEgBwEIAQxA";

int main(int argc, char* argv[])
{
  SDL_Init(SDL_INIT_EVERYTHING);
    
  int seed = -1;
  bool dowrite = false;
  bool noloop = true;
  bool contiguous = true;
  
  bool* screen = new bool[w*h]; 
    
  const char* filename = "";
  for (int i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], "--seed") == 0)
    {
      if (i < argc - 1)
      {
        seed = atoi(argv[i + 1]);
      }
    }
    else if (strcmp(argv[i], "--file") == 0)
    {
      if (i < argc - 1)
      {
        filename = argv[i + 1];
        dowrite = true;
      }
    }
    else if (strcmp(argv[i], "--spacepoints") == 0)
    {
      if (i < argc - 1)
      {
        spacecurves = atoi(argv[i + 1]);
      }
    }
    else if (strcmp(argv[i], "--timepoints") == 0)
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
    else if (strcmp(argv[i], "--bgcolor") == 0)
    {
      if (i < argc - 1)
      {
        int b = argv[i + 1][0] == '#' ? 1 : 0;
        char subbuff[3];
        memcpy(subbuff, &argv[i + 1][b], 2);
        subbuff[2] = '\0';
        sscanf_s(subbuff, "%x", &bg_r);
        memcpy(subbuff, &argv[i + 1][b + 2], 2);
        subbuff[2] = '\0';
        sscanf_s(subbuff, "%x", &bg_g);
        memcpy(subbuff, &argv[i + 1][b + 4], 2);
        subbuff[2] = '\0';
        sscanf_s(subbuff, "%x", &bg_b);
      }
    }
    else if (strcmp(argv[i], "--fgcolor") == 0)
    {
      if (i < argc - 1)
      {
        int b = argv[i + 1][0] == '#' ? 1 : 0;
        char subbuff[3];
        memcpy(subbuff, &argv[i + 1][b], 2);
        subbuff[2] = '\0';
        sscanf_s(subbuff, "%x", &fg_r);
        memcpy(subbuff, &argv[i + 1][b + 2], 2);
        subbuff[2] = '\0';
        sscanf_s(subbuff, "%x", &fg_g);
        memcpy(subbuff, &argv[i + 1][b + 4], 2);
        subbuff[2] = '\0';
        sscanf_s(subbuff, "%x", &fg_b);
      }
    }
    else if (strcmp(argv[i], "--loop") == 0)
    {
      noloop = false;
    }
    else if (strcmp(argv[i], "--incontiguous") == 0)
    {
      contiguous = false;
    }
    else if (strcmp(argv[i], "--pointsperspacecurve") == 0)
    {
      if (i < argc - 1)
      {
        pointsperspacecurve = atoi(argv[i + 1]);
      }
    }
    else if (strcmp(argv[i], "--pointspertimecurve") == 0)
    {
      if (i < argc - 1)
      {
        pointspertimecurve = atoi(argv[i + 1]);
      }
    }
    else if (strcmp(argv[i], "--help") == 0)
    {
      string p_d = base64_decode(parameters);
      for (char& c : p_d)
      {
        c = c ^ 0b01101001;
      }
      cout << p_d << endl;
      delete image;
      return 0;
    }
  }
  if (dowrite)
  {
    contiguous = false;
    noloop = false;
  }
  
  SDL_Window* window = SDL_CreateWindow("scribbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  
  if (argc == 1)
  {
    string p_d = base64_decode(parameters);
    for (char& c : p_d)
    {
      c = c ^ 0b01101001;
    }
    cout << p_d << endl;
  }
  GifWriter writer = {};
  if (dowrite)
  {
    GifBegin(&writer, filename, w, h, 8, true);
  }

  if (seed == -1)
  {
    int t = time(NULL);
    std::cout << "seed: " << t << endl;
    std::cout << endl;
    srand(t);
  }
  else
  {
    srand(seed);
  }

  string p_me = base64_decode(myname);
  for (char& c : p_me)
  {
    c = c ^ 0b01101001;
  }
  cout << p_me << endl;
  
  if (noloop)
  {
  
    metapoints* mps = new metapoints[spacecurves];
    for (int i = 0; i < spacecurves; i++)
    {
      mps[i] = metapoints(w, h, pointspertimecurve, contiguous);
    }
    
    vector<point> dispanchors;
    for (;;)
    {
      for (int i = 0; i < spacecurves; i++)
      {
        point p = mps[i].getpoint();
        
        dispanchors.push_back(p);
      }
      SDL_RenderPresent(renderer);
      drawscreen(renderer, w, h, createdisploop(createpercloop(dispanchors, pointsperspacecurve)), screen, writer, false);
      vector<point>().swap(dispanchors);
      SDL_Event event; 
      SDL_PollEvent(&event);
      if (event.type == SDL_QUIT)
      {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 0;
      }
    }
  }
  else
  {
    vector<point>* timepercanchors = new vector<point>[spacecurves];
    for (int i = 0; i < spacecurves; i++)
    {
      auto timeanchors = randanchors(w, h, timecurves);
      auto timepercanchors2 = createpercloop(timeanchors, pointspertimecurve);
      timepercanchors[i] = timepercanchors2;
    }
    vector<point> dispanchors;

    for (;;)
    {
      for (int i2 = 0; i2 < timecurves * pointspertimecurve; i2++)
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
            //delete [] screen;
            //delete [] timepercanchors;
            delete image;
            return 0;
          }
        }
      }
      if (filename != "")
      {
        GifEnd(&writer);
        //delete [] screen;
        //delete [] timepercanchors;
        delete image;
        return 0;
      }
    }
  }
  return 0;
}

