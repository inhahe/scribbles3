#include <SDL.h>
#include <math.h>       /* sin */
#include <vector>
#include <cstdint>
#include <iostream>
#include <gif.h>
#include <cstring>
#include <boost/program_options.hpp>
#define PI 3.14159265

using namespace std;
//namespace po = boost::program_options;
using namespace  boost::program_options;

struct rgb
{
  int r, g, b;
};

int pointsperspacecurve = 100;
int pointspertimecurve = 100;
int spacecurves = 30;
int timecurves = 5;
int w = 500;
int h = 500;
rgb bg = { 255, 255, 255 };
rgb fg = { 0, 0, 255 };
int seed = -1;
bool dowrite = false;
bool noloop = true;
bool contiguous = true;
string filename = "";
bool changehue = false;
float huespeed = 1;
int huemult = 1;

uint8_t* image = new uint8_t[w * h * 4];

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
    this->lastpoint = this->curvepoints.back();
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

void drawscreen(SDL_Renderer* renderer, int w, int h, vector<point> disppoints,
  bool* screen, GifWriter& writer, bool dowrite, rgb bg, rgb fg)
{
  int sp = 0;
  //for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) screen[++sp] = false;
  memset(screen, 0, w * h * sizeof(bool));
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
        //SDL_RenderDrawPoint(renderer, lp.x, lp.y);
      }
      if (i >= s) break;
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
      if (screen[++sp]) dot = not dot;
      if (dot)
      {
        SDL_SetRenderDrawColor(renderer, fg.r, fg.g, fg.b, 255);
        if (dowrite) SetPixel(x, y, fg.r, fg.g, fg.b);
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, 255);
        if (dowrite) SetPixel(x, y, bg.r, bg.g, bg.b);
      }
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
  SDL_RenderPresent(renderer);
  if (dowrite) GifWriteFrame(&writer, image, w, h, 2, 8, true);
}

rgb hex2rgb(string s)
{
  rgb rgb2;
  int b = s[0] == '#' ? 1 : 0;
  char subbuff[3];
  memcpy(subbuff, &s[b], 2);
  subbuff[2] = '\0';
  sscanf_s(subbuff, "%x", &rgb2.r);
  memcpy(subbuff, &s[b + 2], 2);
  subbuff[2] = '\0';
  sscanf_s(subbuff, "%x", &rgb2.g);
  memcpy(subbuff, &s[b + 4], 2);
  subbuff[2] = '\0';
  sscanf_s(subbuff, "%x", &rgb2.b);
  return rgb2;
}

rgb HSVtoRGB(float H, float S, float V) {
  float s = S / 100;
  float v = V / 100;
  float C = s * v;
  float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
  float m = v - C;
  float r, g, b;
  if (H >= 0 && H < 60) {
    r = C, g = X, b = 0;
  }
  else if (H >= 60 && H < 120) {
    r = X, g = C, b = 0;
  }
  else if (H >= 120 && H < 180) {
    r = 0, g = C, b = X;
  }
  else if (H >= 180 && H < 240) {
    r = 0, g = X, b = C;
  }
  else if (H >= 240 && H < 300) {
    r = X, g = 0, b = C;
  }
  else {
    r = C, g = 0, b = X;
  }
  int R = (r + m) * 255;
  int G = (g + m) * 255;
  int B = (b + m) * 255;
  return rgb{ R, G, B };
}

int parsecommandline(int argc, char* argv[])
{
  int r = 0;
  try
  {
    options_description desc{ "Options" };
    desc.add_options()
      ("help", "This help screen")
      ("seed", value<int>(), "randomization seed. use this to get the same exact pattern you did before")
      ("file", value<string>(),
        "if an output file is specified, --loop will be enabled "
        "and the animation will only loop once, and it won't "
        "let you close it until it's done.")
      ("spacepoints", value<int>(),
        "number of curve anchorpoints in space. "
        "increase this to make more complicated shapes.defaults to 30")
      ("timepoints", value<int>(),
        "number of curve anchorpoints in time per space anchorpoint. "
        "increase this to make the loops longer when specifying --loop or --file. "
        "defaults to 5")
      ("w", value<int>(), "window width, defaults to 500")
      ("h", value<int>(), "window height, defaults to 500")
      ("bgcolor", value<string>(), "background color, six-digit hex number, defaults to #ffffff, or "
        "#000000 if --changehue is enabled")
      ("fgcolor", value<string>(), "foreground color, six-digit hex number, defaults to #0000ff")
      ("changehue", "make fgcolor cycle through the hues. overrides --fgcolor")
      ("huespeed", value<float>(), "amount to increment hue per frame if --changehue is enabled. "
        "ignored if --file or --loop enabled. defaults to 1. hue cycles from 0 to 360")
      ("huemult", value<int>(), "if --loop or --file is enabled and chaneghue is enabled, huemult "
        "specifies how many times to cycles through hues per time loop. defaults to 1")
      ("loop", "loops back on itself in time seamlessly")
      ("incontiguous", "make it so that spacepoints don't move contiguously through time, but skip pixels")
      ("pointsperspacecurve", value<int>(),
        "number of points calculated on each bezier curve. "
        "lines are drawn between each point. defaults to 100. "
        "make this 1 for a jagged effect")
      ("pointspertimecurve", value<int>(),
        "number of points calculated on each bezier curve of change. "
        "if incontiguous is not specified, points are connected linearly across time. "
        "try 1 along with --incontiguous to get a rapid-fire succession of unique shapes");

    //("pi", value<float>()->default_value(3.14f), "Pi")
    //("age", value<int>()->notifier(on_age), "Age");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help"))
    {
      cout << desc;
      r = 1;
    }
    if (vm.count("seed")) seed = vm["seed"].as<int>();
    if (vm.count("file"))
    {
      filename = vm["file"].as<string>();
      dowrite = true;
    }
    if (vm.count("spacepoints")) spacecurves = vm["spacepoints"].as<int>();
    if (vm.count("timepoints")) timecurves = vm["timepoints"].as<int>();
    if (vm.count("w")) w = vm["w"].as<int>();
    if (vm.count("h")) h = vm["h"].as<int>();
    if (vm.count("fgcolor")) fg = hex2rgb(vm["fgcolor"].as<string>());
    if (vm.count("loop")) noloop = false;
    if (vm.count("incontiguous")) contiguous = false;
    if (vm.count("pointsperspacecurve")) pointsperspacecurve = vm["pointsperspacecurve"].as<int>();
    if (vm.count("pointspertimecurve")) pointspertimecurve = vm["pointspertimecurve"].as<int>();
    if (vm.count("changehue"))
    {
      changehue = true;
      bg = { 0, 0, 0 };
    }
    if (vm.count("bgcolor")) bg = hex2rgb(vm["bgcolor"].as<string>());
    if (vm.count("huespeed")) huespeed = vm["huespeed"].as<float>();
    if (vm.count("huemult")) huemult = vm["huemult"].as<int>();
    if (argc == 1) cout << desc;
  }
  catch (const error& ex)
  {
    std::cerr << ex.what() << '\n';
  }
  return r;
}

int main(int argc, char* argv[])
{
  float hue = 160;

  SDL_Init(SDL_INIT_EVERYTHING);

  if (parsecommandline(argc, argv)) return 0;
  bool* screen = new bool[w * h];

  if (dowrite)
  {
    contiguous = false;
    noloop = false;
  }
  SDL_Window* window = SDL_CreateWindow("scribbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  GifWriter writer = {};
  if (dowrite)
  {
    GifBegin(&writer, filename.c_str(), w, h, 8, true);
  }

  if (seed == -1)
  {
    seed = time(NULL);
    cout << endl;
    cout << "seed: " << seed << endl;
    cout << endl;
  }
  srand(seed);

  cout << "Made by Richard A. Nichols III (Inhahe)" << endl;;

  if (noloop)
  {

    metapoints* mps = new metapoints[spacecurves];
    for (int i = 0; i < spacecurves; i++) mps[i] = metapoints(w, h, pointspertimecurve, contiguous);

    vector<point> dispanchors;
    for (;;)
    {
      for (int i = 0; i < spacecurves; i++) dispanchors.push_back(mps[i].getpoint());
      SDL_RenderPresent(renderer);
      drawscreen(renderer, w, h, createdisploop(createpercloop(dispanchors, pointsperspacecurve)),
        screen, writer, false, bg, changehue ? HSVtoRGB(hue, 100, 100) : fg);
      if (changehue)
      {
        hue += huespeed;
        if (hue > 360) hue -= 360;
        else if (hue < 0) hue += 360;
      }
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
    int bs = testbeziersize(pointspertimecurve);
    if (changehue) huespeed = 360.0 / (timecurves * bs) * huemult;
    for (;;)
    {
      for (int i2 = 0; i2 < timecurves * bs; i2++)
      {
        for (int i = 0; i < spacecurves; i++) dispanchors.push_back(timepercanchors[i][i2]);
        if (changehue)
        {
          hue += huespeed;
          if (hue > 360) hue -= 360;
        }
        drawscreen(renderer, w, h, createdisploop(createpercloop(dispanchors, pointsperspacecurve)),
          screen, writer, dowrite, bg, changehue ? HSVtoRGB(int(hue), 100, 100) : fg);

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
      if (dowrite)
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

