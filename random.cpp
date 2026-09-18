//todo: add error checks for SDL_PollEvent, SDL_DestroyWindow, SDL_DestroyRenderer, SDL_Quit
//todo: sometimes the program runs with fps being a lot slower for no apparent reason
//todo: update SDL2
//why do gifs sometimes have defects when they're aborted? they shouldn't.
//todo: sometimes when it ends it prints two newlines after fps, sometimes 1
#include <vector>
#include <cstdint>
#include <cstring>
#include <iostream>
#define fRAND_MAX static_cast <float> (RAND_MAX) 
using namespace std;

struct rgb
{
  int r, g, b;
};

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

int testbeziersize(int pointspercurve)
{
  int r = 0;
  for (float i2 = 0; i2 < 1; i2 += 1.0 / static_cast <float>(pointspercurve))
    r++;
  return r;
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
    points.push_back(point{ x, y });
    while (x != p2.x)
    {
      x += xd;
      if (px < 0) px += 2 * dy1;
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y += xd;
        else y -= xd;
        px += 2 * (dy1 - dx1);
      }
      points.push_back(point{ x, y });
    }
  }
  else
  {
    yd = (p1.y < p2.y) ? 1 : -1;
    y = p1.y;
    x = p1.x;
    points.push_back(point{ x, y });
    while (y != p2.y)
    {
      y += yd;
      if (py <= 0)
      {
        py += 2 * dx1;
      }
      else
      {
        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x += yd;
        else x -= yd;
        py += 2 * (dx1 - dy1);
      }
      points.push_back(point{ x, y });
    }
  }
  return points;
}

class metapoints
{
  curve c1 = curve();
  curve c2 = curve();
  vector<point> curvepoints2;
  vector<point> curvepoints;
  int pointindex = 0;
  int cpsize = 0;
  point lastpoint = point();
  int pointspercurve = 0;
  bool contiguous = false;
  int h, w;
public:
  metapoints()
  {
  }
  metapoints(const int w, const int h, int pointspercurve, bool contiguous)
  {
    this->contiguous = contiguous;
    //method 1
    this->c1.p1.x = (rand()) / (fRAND_MAX / w);
    this->c1.p1.y = (rand()) / (fRAND_MAX / h);
    this->c1.p2.x = (rand()) / (fRAND_MAX / w);
    this->c1.p2.y = (rand()) / (fRAND_MAX / h);
    this->c1.p3.x = (rand()) / (fRAND_MAX / w);
    this->c1.p3.y = (rand()) / (fRAND_MAX / h);
    this->c2.p1.x = (this->c1.p2.x + this->c1.p1.x) / 2;
    this->c2.p1.y = (this->c1.p2.y + this->c1.p1.y) / 2;
    this->c2.p2 = this->c1.p2;
    this->c2.p3.x = (this->c1.p3.x + this->c1.p2.x) / 2;
    this->c2.p3.y = (this->c1.p3.y + this->c1.p2.y) / 2;
    this->pointspercurve = pointspercurve;
    this->curvepoints = beziercurve_quadratic(this->c2.p1, this->c2.p2, this->c2.p3, this->pointspercurve);
    this->lastpoint = this->curvepoints.back();
    this->w = w;
    this->h = h;
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
      this->c1.p3.x = (rand()) / (fRAND_MAX / this->w);
      this->c1.p3.y = (rand()) / (fRAND_MAX / this->h);
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
        return this->curvepoints2[this->pointindex++];
      }
      else
      {
        this->cpsize = this->curvepoints.size();
        return this->curvepoints[this->pointindex++];
      }
    }
    else
    {
      if (this->contiguous) return this->curvepoints2[this->pointindex++];
      else return this->curvepoints[this->pointindex++];
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
    p.x = int(rand() / (fRAND_MAX / w));
    p.y = int(rand() / (fRAND_MAX / h));
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

static int spacecurvepoints = 100;
static int timecurvepoints = 100;
static int spacecurves = 30;
static int timecurves = 5;
const int w = 1000;
const int h = 1000;
static rgb bg = { 255, 255, 255 };
static rgb fg = { 0, 0, 255 };
static int seed = -1;
static bool noloop = true;
static bool contiguous = true;
static bool rotatehue = false;
static float huespeed = 1;
static int huemult = 1;
static float sat = 100;
static float val = 100;
static bool noscreen = false;
static bool running = true;
static int framespan = 50;
static bool enable_vsync = false;
static float hue = 160;
static bool* screen = nullptr;
static uint8_t* image = nullptr;
static uint8_t* pixels = nullptr;
static vector<point> dispanchors;
static vector<point>* timepercanchors;
static int bs;
static metapoints* mps = nullptr;
static vector<point> timeanchors;
static vector<point> timepercanchors2;

void prnjs(string h)
{
  string s = "alert('" + h + "')";
}

void prnjs(string h, string e)
{
  string s = "alert('" + h + e + "')";
}

void drawscreen(vector<point> disppoints)
{
  //for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) screen[++sp] = false;
  memset(screen, 0, w * h * sizeof(bool));
  enum direction { none, up, down };
  direction ldir = none, dir;
  point lp = { -1, -1 };
  int sp = 0;
  int s = disppoints.size();
  bool* sp_screen = nullptr;
  uint8_t* sp_pixels = nullptr;
  int pitch;
  uint32_t fgint_screen = 0;
  uint32_t bgint_screen = 0;
  uint32_t bgint_image = bg.r + (bg.g << 8) + (bg.b << 16) + 0xff000000;
  uint32_t fgint_image = fg.r + (fg.g << 8) + (fg.b << 16) + 0xff000000;
  uint8_t* sp_image = image;
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
      }
      if (i >= s) break;
      ldir = dir;
    }
    lp = p;
  }

  sp_screen = screen;
  sp_image = image;
  for (int y = 0; y < h; y++)
  {
    sp_pixels = pixels + y * pitch;
    bool dot = false;
    for (int x = 0; x < w; x++)
    {
      if (*(++sp_screen)) dot = not dot;
      if (dot)
      {
        *(uint32_t*)sp_pixels = fgint_screen;
      }
      else
      {
        *(uint32_t*)sp_pixels = bgint_screen;
      }
      sp_pixels += 4;
      sp_image += 4;
    }
  }

  //update window
}

rgb HSVtoRGB(float H, float S, float V) {
  float s = S / 100;
  float v = V / 100;
  float C = s * v;
  float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
  float m = v - C;
  float r, g, b;
  if (H < 60) r = C, g = X, b = 0;
  else if (H < 120) r = X, g = C, b = 0;
  else if (H < 180) r = 0, g = C, b = X;
  else if (H < 240) r = 0, g = X, b = C;
  else if (H < 300)  r = X, g = 0, b = C;
  else r = C, g = 0, b = X;
  int R = (r + m) * 255;
  int G = (g + m) * 255;
  int B = (b + m) * 255;
  return rgb{ R, G, B };
}

void mainloopnorepeat()
{
  for (int i = 0; i < spacecurves; i++) dispanchors.push_back(mps[i].getpoint());
  if (rotatehue) fg = HSVtoRGB(hue, sat, val);
  drawscreen(createdisploop(createpercloop(dispanchors, spacecurvepoints)));
  if (rotatehue)
  {
    hue += huespeed;
    hue = fmod(hue + 360, 360);
  }
  vector<point>().swap(dispanchors);
}

void mainlooprepeat()
{
  for (int i2 = 0; i2 < timecurves * bs; i2++)
  {
    for (int i = 0; i < spacecurves; i++) dispanchors.push_back(timepercanchors[i][i2]);
    if (rotatehue)
    {
      hue += huespeed;
      hue = fmod(hue + 360, 360);
    }
    if (rotatehue) fg = HSVtoRGB(hue, sat, val);
    drawscreen(createdisploop(createpercloop(dispanchors, spacecurvepoints)));
    vector<point>().swap(dispanchors);
  }
}

extern "C" 
{
  void mainloop()
  {
    if (noloop) mainloopnorepeat();
    else mainlooprepeat();
  }
}

int main(int argc, char* argv[])
{
  hue = 160;
  screen = new bool[w * h];
  uint8_t pixels[w * h * 4];
  timepercanchors = new vector<point>[spacecurves];
  bs = testbeziersize(timecurvepoints);
  image = new uint8_t[w * h * 4];
  if (seed == -1) seed = time(NULL);
  srand(seed);
  if (noloop)
  {
    mps = new metapoints[spacecurves];
    for (int i = 0; i < spacecurves; i++) mps[i] = metapoints(w, h, timecurvepoints, contiguous);
  }
  else
  {
    for (int i = 0; i < spacecurves; i++)
    {
      timeanchors = randanchors(w, h, timecurves);
      timepercanchors2 = createpercloop(timeanchors, timecurvepoints);
      timepercanchors[i] = timepercanchors2;
    }
  }
}