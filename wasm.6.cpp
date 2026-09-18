//todo: add error checks for SDL_PollEvent, SDL_DestroyWindow, SDL_DestroyRenderer, SDL_Quit
//todo: sometimes the program runs with fps being a lot slower for no apparent reason
//todo: update SDL2
//why do gifs sometimes have defects when they're aborted? they shouldn't.
//todo: sometimes when it ends it prints two newlines after fps, sometimes 1
#include <vector>
#include <cstdint>
#include <cstring>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <SDL.h>
#include <SDL_hints.h>
#include <iostream>
#define sleep(x) Sleep(1000 * (x))
//#include <SDL2/SDL.h>
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

struct Context
{
  int spacecurvepoints = 100;
  int timecurvepoints = 100;
  int spacecurves = 30;
  int timecurves = 5;
  int w = 1000;
  int h = 1000;
  rgb bg = { 255, 255, 255 };
  rgb fg = { 0, 0, 255 };
  int seed = -1;
  bool dowrite = false;
  bool noloop = true;
  bool contiguous = true;
  string filename = "";
  bool rotatehue = false;
  float huespeed = 1;
  int huemult = 1;
  float sat = 100;
  float val = 100;
  bool noscreen = false;
  bool running = true;
  int framespan = 50;
  bool enable_vsync = false;

  float hue = 160;
  bool* screen = nullptr;
  uint8_t* image = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Window* window = nullptr;
  SDL_Surface* surface = nullptr;
  SDL_Texture* texture = nullptr;
  SDL_PixelFormat* pixel_format_surface = nullptr;
  SDL_Event event;
  vector<point> dispanchors;
  vector<point>* timepercanchors;
  int bs;
  metapoints* mps = nullptr;
  vector<point> timeanchors;
  vector<point> timepercanchors2;
};


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

void drawscreen(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* surface, int w, int h, vector<point> disppoints,
  bool* screen, uint8_t* image, rgb bg, rgb fg, SDL_PixelFormat* pixel_format_surface, bool enable_vsync)
{
  //for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) screen[++sp] = false;
  memset(screen, 0, w * h * sizeof(bool));
  enum direction { none, up, down };
  direction ldir = none, dir;
  point lp = { -1, -1 };
  int sp = 0;
  int s = disppoints.size();
  uint8_t* pixels = nullptr;
  bool* sp_screen = nullptr;
  uint8_t* sp_pixels = nullptr;
  int pitch;
  Uint32 fgint_screen = 0;
  Uint32 bgint_screen = 0;
  bgint_screen = SDL_MapRGBA(pixel_format_surface, bg.r, bg.g, bg.b, 0xff);
  fgint_screen = SDL_MapRGBA(pixel_format_surface, fg.r, fg.g, fg.b, 0xff);
  Uint32 bgint_image = bg.r + (bg.g << 8) + (bg.b << 16) + 0xff000000;
  Uint32 fgint_image = fg.r + (fg.g << 8) + (fg.b << 16) + 0xff000000;
  uint8_t* sp_image = image;
  SDL_Texture* texture;
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

  pixels = (uint8_t*)(surface->pixels);
  pitch = surface->pitch;
  if (SDL_MUSTLOCK(surface))
  {
    if (SDL_LockSurface(surface) < 0)
    {
      prnjs("SDL_LockSurface(surface): ", SDL_GetError());
      exit(EXIT_FAILURE);
    }
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

  
  if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
  if (enable_vsync) 
  {
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL)
    {
      prnjs("SDL_CreateTextureFromSurface(renderer, surface), NULL, NULL): ", SDL_GetError());
      exit(EXIT_FAILURE);
    }
    if (SDL_RenderCopy(renderer, texture, NULL, NULL) < 0)
    {
      prnjs("SDL_RenderCopy(renderer, texture, NULL, NULL) < 0): ", SDL_GetError());
      exit(EXIT_FAILURE);
    }
    SDL_RenderPresent(renderer);
    if (SDL_RenderClear(renderer) < 0)  //"You are strongly encouraged to call SDL_RenderClear() to initialize the backbuffer 
    {                                   //before starting each new frame's drawing, even if you plan to overwrite every pixel." 
      prnjs("(SDL_RenderClear(renderer): ", SDL_GetError());   //- https://wiki.libsdl.org/SDL2/SDL_RenderPresent
      exit(EXIT_FAILURE);
    }
    SDL_DestroyTexture(texture); //dunno why we can't just make texture once in main and use it over and over? 
  }                              //screen is black when I do that
  else
  {
    if (SDL_UpdateWindowSurface(window) < 0)
    {
      prnjs("SDL_UpdateWindowSurface(window): ", SDL_GetError());
      exit(EXIT_FAILURE);
    }
  }
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

void mainloopnorepeat(void *arg)
{
  Context* context = static_cast<Context*>(arg);
  for (int i = 0; i < context->spacecurves; i++) context->dispanchors.push_back(context->mps[i].getpoint());
  if (context->rotatehue) context->fg = HSVtoRGB(context->hue, context->sat, context->val);
  drawscreen(context->window, context->renderer, context->surface, context->w, context->h, createdisploop(createpercloop(context->dispanchors, context->spacecurvepoints)),
    context->screen, context->image, context->bg, context->fg, context->pixel_format_surface, context->enable_vsync);
  if (context->rotatehue)
  {
    context->hue += context->huespeed;
    context->hue = fmod(context->hue + 360, 360);
  }
  vector<point>().swap(context->dispanchors);

  SDL_PollEvent(&context->event);
  if (context->event.type == SDL_QUIT)
  {
    SDL_DestroyWindow(context->window);
    SDL_DestroyRenderer(context->renderer);
    SDL_Quit();
    emscripten_cancel_main_loop();
  }
}

void mainlooprepeat(void *arg)
{
  Context* context = static_cast<Context*>(arg);
  for (int i2 = 0; i2 < context->timecurves * context->bs; i2++)
  {
    for (int i = 0; i < context->spacecurves; i++) context->dispanchors.push_back(context->timepercanchors[i][i2]);
    if (context->rotatehue)
    {
      context->hue += context->huespeed;
      context->hue = fmod(context->hue + 360, 360);
    }
    if (context->rotatehue) context->fg = HSVtoRGB(context->hue, context->sat, context->val);
    drawscreen(context->window, context->renderer, context->surface, context->w, context->h, createdisploop(createpercloop(context->dispanchors, context->spacecurvepoints)),
      context->screen, context->image, context->bg, context->fg, context->pixel_format_surface, context->enable_vsync);
    vector<point>().swap(context->dispanchors);

    SDL_PollEvent(&context->event);
    if (context->event.type == SDL_QUIT || not context->running)
    {
      SDL_DestroyWindow(context->window);
      SDL_DestroyRenderer(context->renderer);
      SDL_Quit();
      emscripten_cancel_main_loop();
    }
  }
}

int main(int argc, char* argv[])
{
  /*SDL_version *version;
  SDL_GetVersion(version);
  prnjs("SDL_GetVersion() major: ", to_string(version->major));
  prnjs("SDL_GetVersion() minor: ", to_string(version->minor));
  prnjs("SDL_GetVersion() patch: ", to_string(version->patch));
  */
  

  Context context;
  /*SDL_DisplayMode DM;
  SDL_GetCurrentDisplayMode(0, &DM);
  context.h = DM.h;
  context.w = DM.w;
  cout << "height: " << context.h << endl;
  cout << "width: " << context.w << endl;
  */
  context.hue = 160;
  context.screen = new bool[context.w * context.h];
  context.renderer = nullptr;
  context.window = nullptr;
  context.surface = nullptr;
  context.texture = nullptr;
  context.pixel_format_surface = nullptr;
  context.timepercanchors = new vector<point>[context.spacecurves];
  context.bs = testbeziersize(context.timecurvepoints);
  context.image = new uint8_t[context.w * context.h * 4];

  //emscripten_set_canvas_element_size("canvas", 1000, 1000);

  if (context.seed == -1) context.seed = time(NULL);
  srand(context.seed);
  //SDL_SetHintWithPriority(SDL_HINT_WINDOWS_DPI_AWARENESS, "unaware", SDL_HINT_OVERRIDE);
  if (SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE) != SDL_TRUE) prnjs("Could not set vsync. It may not be available on your platform.");
  if (SDL_Init(SDL_INIT_VIDEO))
  {
    prnjs("SDL_Init(SDL_INIT_VIDEO): ", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  //context.window = SDL_CreateWindow("scribbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, context.w, context.h, SDL_WINDOW_ALLOW_HIGHDPI + SDL_WINDOW_FULLSCREEN_DESKTOP);
  context.window = SDL_CreateWindow("scribbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, context.w, context.h, 0);
  context.window = SDL_CreateWindow("scribbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, context.w, context.h, SDL_WINDOW_ALLOW_HIGHDPI);
  if (context.window == NULL)
  {
    prnjs("SDL_CreateWindow(\"scribbles\", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_ALLOW_HIGHDPI): ", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  if (context.enable_vsync)
  {
    context.surface = SDL_CreateRGBSurface(0, context.w, context.h, 32, 0, 0, 0, 0);
    if (context.surface == NULL)
    {
      prnjs("SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0): ", SDL_GetError());
      exit(EXIT_FAILURE);
    }
    context.renderer = SDL_CreateRenderer(context.window, -1, 0);
    if (context.renderer == NULL)
    {
      prnjs("SDL_CreateRenderer(context.window, -1, 0): ", SDL_GetError());
      exit(EXIT_FAILURE);
    }
    if (SDL_RenderClear(context.renderer) < 0)  //"You are strongly encouraged to call SDL_RenderClear() to initialize the backbuffer 
    {                                   //before starting each new frame's drawing, even if you plan to overwrite every pixel." 
      prnjs("(SDL_RenderClear(renderer): ", SDL_GetError());   //- https://wiki.libsdl.org/SDL2/SDL_RenderPresent
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    context.surface = SDL_GetWindowSurface(context.window);
    if (context.surface == NULL)
    {
      prnjs("SDL_GetWindowSurface(window): ", SDL_GetError());
      exit(EXIT_FAILURE);
    }
    context.w = context.surface->w;
    context.h = context.surface->h;

  }
  context.pixel_format_surface = context.surface->format;
  
  if (context.noloop)
  {
    context.mps = new metapoints[context.spacecurves];
    for (int i = 0; i < context.spacecurves; i++) context.mps[i] = metapoints(context.w, context.h, context.timecurvepoints, context.contiguous);
    emscripten_set_main_loop_arg(mainloopnorepeat, &context, 0, 1);
  }
  else
  {
    for (int i = 0; i < context.spacecurves; i++)
    {
      context.timeanchors = randanchors(context.w, context.h, context.timecurves);
      context.timepercanchors2 = createpercloop(context.timeanchors, context.timecurvepoints);
      context.timepercanchors[i] = context.timepercanchors2;
    }
    emscripten_set_main_loop_arg(mainlooprepeat, &context, 0, 1);
  }
}