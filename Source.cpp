//todo: add error checks for SDL_PollEvent, SDL_DestroyWindow, SDL_DestroyRenderer, SDL_Quit
//todo: sometimes the program runs with fps being a lot slower for no apparent reason
//todo: update SDL2
//why do gifs sometimes have defects when they're aborted? they shouldn't.
//todo: sometimes when it ends it prints two newlines after fps, sometimes 1
#include <vector>
#include <cstdint>
#include <iostream>
#include <gif.h>
#include <cstring>
#include <chrono>
#ifdef _WIN32
#include <SDL.h>
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#elif __linux__
#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
struct COORD { int X, Y; };
#endif
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#define fRAND_MAX static_cast <float> (RAND_MAX) 
using namespace std;
using namespace boost::program_options;
using namespace boost;
using namespace std::chrono;

struct rgb
{
  int r, g, b;
};

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


void set_cursor(int x = 0, int y = 0)
{
#ifdef _WIN32
  HANDLE handle;
  COORD coordinates;
  handle = GetStdHandle(STD_OUTPUT_HANDLE);
  coordinates.X = x;
  coordinates.Y = y;
  SetConsoleCursorPosition(handle, coordinates);
#elif __linux__
  cout << "\033[" << y << ";" << x << "H" << flush;
#endif
}

void set_yellow_text()
{
#ifdef _WIN32
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, 14);
#elif __linux__
  cout << "\x1B[93m";
#endif
}

void set_white_text()
{
#ifdef _WIN32
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, 7);
#elif __linux__
  cout << "\x1B[0m";
#endif
}

COORD get_cursor()
{
#ifdef _WIN32
  CONSOLE_SCREEN_BUFFER_INFO cbsi;
  if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbsi))
  {
    return cbsi.dwCursorPosition;
  }
  else
  {
    // The function failed. Call GetLastError() for details.
    COORD invalid = { 0, 0 };
    return invalid;
  }
#elif __linux__
  COORD result;
  char buf[8];
  char cmd[] = "\033[6n";
  struct termios save, raw;
  tcgetattr(0, &save);
  cfmakeraw(&raw); tcsetattr(0, TCSANOW, &raw);
  if (isatty(fileno(stdin)))
  {
    write(1, cmd, sizeof(cmd));
    read(0, buf, sizeof(buf));

    /* It doesn't work!!?
    sscanf(buf,"%d",curline);
    printf("\n\rCurrent Line: %d\n\r" , curline);
    */

    smatch sm1;
    regex_search(string(buf), sm1, regex("\\[(\\d*);(\\d*)R"));
    result.Y = stoi(sm1[1]);
    result.X = stoi(sm1[2]);
  }
  tcsetattr(0, TCSANOW, &save);
  return result;
#endif
}

void show_console_cursor(const bool show)
{
#if defined(_WIN32)
  static const HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cci;
  GetConsoleCursorInfo(handle, &cci);
  cci.bVisible = show; // show/hide cursor
  SetConsoleCursorInfo(handle, &cci);
#elif defined(__linux__)
  cout << (show ? "\033[?25h" : "\033[?25l") << flush; // show/hide cursor
#endif // Windows/Linux
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
  curve c1 = curve();
  curve c2 = curve();
  vector<point> curvepoints2;
  vector<point> curvepoints;
  int pointindex = 0;
  int cpsize = 0;
  point lastpoint = point();
  int pointspercurve = 0;
  bool contiguous = false;
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
      this->c1.p3.x = (rand()) / (fRAND_MAX / w);
      this->c1.p3.y = (rand()) / (fRAND_MAX / h);
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

void drawscreen(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* surface, int w, int h, vector<point> disppoints,
  bool* screen, uint8_t* image, GifWriter& writer, bool noscreen, bool dowrite, rgb bg, rgb fg, SDL_PixelFormat* pixel_format_surface, bool enable_vsync)
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
  if (not noscreen)
  {
    bgint_screen = SDL_MapRGBA(pixel_format_surface, bg.r, bg.g, bg.b, 0xff);
    fgint_screen = SDL_MapRGBA(pixel_format_surface, fg.r, fg.g, fg.b, 0xff);
  }
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

  if (not noscreen)
  {
    pixels = (uint8_t*)(surface->pixels);
    pitch = surface->pitch;
    if (SDL_MUSTLOCK(surface))
    {
      if (SDL_LockSurface(surface) < 0)
      {
        cout << endl << "SDL_LockSurface(surface): " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
      }
    }
  }
  sp_screen = screen;
  sp_image = image;
  for (int y = 0; y < h; y++)
  {
    if (not noscreen) sp_pixels = pixels + y * pitch;
    bool dot = false;
    for (int x = 0; x < w; x++)
    {
      if (*(++sp_screen)) dot = not dot;
      if (dot)
      {
        if (not noscreen) *(uint32_t*)sp_pixels = fgint_screen;
        if (dowrite)
        {
          *(uint32_t*)sp_image = fgint_image;
        }
      }
      else
      {
        if (not noscreen) *(uint32_t*)sp_pixels = bgint_screen;
        if (dowrite)
        {
          *(uint32_t*)sp_image = bgint_image;
        }
      }
      sp_pixels += 4;
      sp_image += 4;
    }
  }

  if (not noscreen)
  {
    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
    if (enable_vsync) 
    {
      texture = SDL_CreateTextureFromSurface(renderer, surface);
      if (texture == NULL)
      {
        cout << endl << "SDL_CreateTextureFromSurface(renderer, surface), NULL, NULL): " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
      }
      if (SDL_RenderCopy(renderer, texture, NULL, NULL) < 0)
      {
        cout << endl << "SDL_RenderCopy(renderer, texture, NULL, NULL) < 0): " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
      }
      SDL_RenderPresent(renderer);
      if (SDL_RenderClear(renderer) < 0)  //"You are strongly encouraged to call SDL_RenderClear() to initialize the backbuffer 
      {                                   //before starting each new frame's drawing, even if you plan to overwrite every pixel." 
        cout << endl << "(SDL_RenderClear(renderer): " << SDL_GetError() << endl;   //- https://wiki.libsdl.org/SDL2/SDL_RenderPresent
        exit(EXIT_FAILURE);
      }
      SDL_DestroyTexture(texture); //dunno why we can't just make texture once in main and use it over and over? 
    }                              //screen is black when I do that
    else
    {
      if (SDL_UpdateWindowSurface(window) < 0)
      {
        cout << endl << "SDL_UpdateWindowSurface(window): " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
      }
    }
  }
  if (dowrite)
  {
    GifWriteFrame(&writer, image, w, h, 2, 8, true);
  }
}

rgb hex2rgb(string s)
{
  rgb rgb2;
  smatch sm;
  if (not (regex_match(s, sm, regex("[#]?(\\w{2})(\\w{2})(\\w{2})"))))
  {
    cout << "Invalid color specification: " << s << endl;
    exit(EXIT_FAILURE);
  }
  rgb2.r = stoi(sm[1], nullptr, 16);
  rgb2.g = stoi(sm[2], nullptr, 16);
  rgb2.b = stoi(sm[3], nullptr, 16);
  return rgb2;
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

int parsecommandline(int argc, char* argv[])
{
  int r = 0;
  try
  {
    options_description desc{ "\nOptions" };
    desc.add_options()
      ("help", "display this help screen")
      ("seed", value<int>(), "randomization seed. use this to get the same exact pattern you got before "
        "(but some of the other options will eliminate all similarity in the pattern if they're any different)")
      ("file", value<string>(),
        "if an output file is specified, --loop will be enabled, and the animation will stop after one loop. "
        "filename extension should be \"gif\"")
      ("noscreen", "doesn't display anything. only for use with --file")
      ("vsync", "don't update the screen faster than the screen refresh rate. this isn't supported on all platforms")
      ("spacecurves", value<int>(),
        "number of curves in space. "
        "increase this to make more complicated shapes. defaults to 30")
      ("timecurves", value<int>(),
        "number of curves in time per space curve. only applies when --loop or --file is enabled. "
        "increase this to make the loops longer. defaults to 5")
      ("w", value<int>(), "window width. defaults to 1000. I recommend a square aspect ratio; otherwise the graphics are "
        "kinda skewed")
      ("h", value<int>(), "window height. defaults to 1000. smaller width and height make the program run faster  ")
      ("bgcolor", value<string>(), "background color, six-digit hex number. defaults to #ffffff, or #000000 if "
        "--rotatehue is enabled")
      ("fgcolor", value<string>(), "foreground color, six-digit hex number. defaults to #0000ff")
      ("rotatehue", "make fgcolor cycle through the hues. overrides --fgcolor")
      ("huespeed", value<float>(), "amount to increment hue per frame if --rotatehue is enabled. floating point. defaults "
        "to 1. hue cycles from 0 to 360. ignored if --file or --loop is enabled.")
      ("huemult", value<int>(), "if --loop or --file is enabled and --rotatehue is enabled, --huemult "
        "specifies how many times to cycle through hues per time loop. defaults to 1")
      ("saturation", value<float>(), "saturation of colors when using --rotatehue. 1 to 100. "
        "defaults to 100")
      ("value", value<float>(), "brightness of colors when using --rotatehue. 1 to 100. "
        "defaults to 100")
      ("loop", "loops back on itself in time seamlessly. I recommend using --vsync with --loop; otherwise it'll go "
        "way too fast")
      ("incontiguous", "make it so that spacecurves don't move contiguously through time, but skip pixels. "
        "this will have the effect of making the animation change faster. --incontiguous is automatically "
        "enabled when --loop or --file is enabled. I recommend using --vsync with --incontiguous; otherwise it'll "
        "go way too fast")
      ("spacecurvepoints", value<int>(),
        "number of points calculated on each bezier curve in space. "
        "lines are drawn between each point. defaults to 100. "
        "make this 1 for a jagged effect")
      ("timecurvepoints", value<int>(),
        "number of points calculated on each bezier curve of change. "
        "if --incontiguous is not enabled, points are connected linearly across time. "
        "try 1 along with --incontiguous to get a rapid-fire succession of completely different shapes");

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
    if (vm.count("noscreen")) noscreen = true;
    if (vm.count("vsync")) enable_vsync = true;
    if (vm.count("spacecurves")) spacecurves = vm["spacecurves"].as<int>();
    if (vm.count("timecurves")) timecurves = vm["timecurves"].as<int>();
    if (vm.count("w")) w = vm["w"].as<int>();
    if (vm.count("h")) h = vm["h"].as<int>();
    if (vm.count("fgcolor")) fg = hex2rgb(vm["fgcolor"].as<string>());
    if (vm.count("loop")) noloop = false;
    if (vm.count("incontiguous")) contiguous = false;
    if (vm.count("spacecurvepoints")) spacecurvepoints = vm["spacecurvepoints"].as<int>();
    if (vm.count("timecurvepoints")) timecurvepoints = vm["timecurvepoints"].as<int>();
    if (vm.count("rotatehue"))
    {
      rotatehue = true;
      bg = { 0, 0, 0 };
    }
    if (vm.count("saturation")) sat = vm["saturation"].as<float>();
    if (vm.count("value")) val = vm["value"].as<float>();
    if (vm.count("bgcolor")) bg = hex2rgb(vm["bgcolor"].as<string>());
    if (vm.count("huespeed"))
    {
      huespeed = vm["huespeed"].as<float>();
      huespeed = copysign(fmod(fabs(huespeed), 360), huespeed);
    }
    if (vm.count("huemult")) huemult = vm["huemult"].as<int>();
    if (argc == 1) cout << desc;
  }
  catch (const error& ex)
  {
    std::cerr << endl;
    std::cerr << ex.what() << endl;
    std::cerr << endl;
    std::cerr << "'" << argv[0] << " --help' for command-line options" << endl;
    r = 1;
  }
  return r;
}

void display_warning(COORD percent_cursor_pos = COORD{ -1, -1 })
{
  show_console_cursor(true);
  if (percent_cursor_pos.X != -1) set_cursor(percent_cursor_pos.X, percent_cursor_pos.Y);
  set_yellow_text();
  cout << endl << endl << "\a";
  cout << "Warning: Writing to file has been aborted. File is not complete. It will not loop seamlessly." << endl;
  set_white_text();
}

GifWriter writer = {};

#ifdef _WIN32
BOOL WINAPI consoleHandler(DWORD signal) {
  switch (signal)
  {
  case CTRL_C_EVENT:
  case CTRL_CLOSE_EVENT:
  case CTRL_BREAK_EVENT:
  case CTRL_LOGOFF_EVENT:
  case CTRL_SHUTDOWN_EVENT:
    if (dowrite)
    {
      GifEnd(&writer);
      display_warning();
    }
    else if (signal != CTRL_CLOSE_EVENT)
    {
      show_console_cursor(true);
      cout << endl << endl;
    }
    running = false;
    if (signal == CTRL_CLOSE_EVENT) return TRUE;
    return FALSE;
  default:
    return FALSE;
  }
}
#elif __linux__
void my_handler(int s) {
  if (dowrite)
  {
    GifEnd(&writer);
    display_warning();
    exit(EXIT_FAILURE);
  }
  else
  {
    show_console_cursor(true);
    cout << endl << endl; 
    exit(EXIT_SUCCESS);
  }
}
#endif

int main(int argc, char* argv[])
{
#ifdef _WIN32
  SetConsoleCtrlHandler((PHANDLER_ROUTINE)consoleHandler, TRUE);
#elif __linux__
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = my_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);
#endif
  float hue = 160;

  if (parsecommandline(argc, argv)) return 0;
  noscreen = noscreen && dowrite;
  enable_vsync = enable_vsync && not noscreen;
  if (dowrite)
  {
    contiguous = false;
    noloop = false;
  }
  if ((not enable_vsync) && ((not noloop) or not contiguous) && not dowrite)
  {
    cout << endl << "Using --vsync is recommended when using --loop or --incontiguous; otherwise it'll probably run too fast." << endl;
  }

  bool* screen = new bool[w * h];
  uint8_t* image = nullptr;
  //Uint8* pixels = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Window* window = nullptr;
  SDL_Surface* surface = nullptr;
  SDL_Texture* texture = nullptr;
  SDL_PixelFormat* pixel_format_surface = nullptr;
  SDL_Event event;
  vector<point> dispanchors;
  vector<point>* timepercanchors = new vector<point>[spacecurves];
  int bs = testbeziersize(timecurvepoints);
  image = new uint8_t[w * h * 4];
  steady_clock::time_point t1, t2;
  int framenum = 0;
  int framespans = 1;

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

  cout << "Made by Richard A. Nichols III (Inhahe)" << endl << endl << endl << flush;
  COORD fps_cursor_pos = get_cursor();
  COORD percent_cursor_pos = get_cursor();
  fps_cursor_pos.Y = percent_cursor_pos.Y - 1;
  show_console_cursor(false);
  if (not noscreen)
  {
    if (SDL_SetHintWithPriority(SDL_HINT_RENDER_VSYNC, "1", SDL_HINT_OVERRIDE) != SDL_TRUE) cout << "Could not set vsync. It may not be available on your platform." << endl;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
      cout << "SDL_Init(SDL_INIT_EVERYTHING): " << SDL_GetError() << endl;
      exit(EXIT_FAILURE);
    }
    window = SDL_CreateWindow("scribbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL)
    {
      cout << "SDL_CreateWindow(\"scribbles\", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_ALLOW_HIGHDPI): " << SDL_GetError() << endl;
      exit(EXIT_FAILURE);
    }
    if (enable_vsync)
    {
      surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
      if (surface == NULL)
      {
        cout << "SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0): " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
      }
      renderer = SDL_CreateRenderer(window, -1, 0);
      if (renderer == NULL)
      {
        cout << "SDL_CreateRenderer(window, -1, 0): " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
      }
      if (SDL_RenderClear(renderer) < 0)  //"You are strongly encouraged to call SDL_RenderClear() to initialize the backbuffer 
      {                                   //before starting each new frame's drawing, even if you plan to overwrite every pixel." 
        cout << "(SDL_RenderClear(renderer): " << SDL_GetError() << endl;   //- https://wiki.libsdl.org/SDL2/SDL_RenderPresent
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      surface = SDL_GetWindowSurface(window);
      if (surface == NULL)
      {
        cout << "SDL_GetWindowSurface(window): " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
      }
    }
    pixel_format_surface = surface->format;
  }

  if (noloop)
  {
    metapoints* mps = new metapoints[spacecurves];
    for (int i = 0; i < spacecurves; i++) mps[i] = metapoints(w, h, timecurvepoints, contiguous);
    set_cursor(fps_cursor_pos.X, fps_cursor_pos.Y);
    cout << "fps: " << flush;
    t1 = steady_clock::now();
    for (;;)
    {
      framenum++;
      if (framenum == framespan)
      {
        t2 = steady_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        double seconds = time_span.count();
        if (seconds >= 1)
        {
          t1 = t2;
          set_cursor(fps_cursor_pos.X, fps_cursor_pos.Y);
          cout << "fps: " << int(float(framespan * framespans) / seconds) << "       " << flush;
          framespans = 1;
        }
        else
        {
          framespans++;
        }
        framenum = 0;
      }
      for (int i = 0; i < spacecurves; i++) dispanchors.push_back(mps[i].getpoint());
      if (rotatehue) fg = HSVtoRGB(hue, sat, val);
      drawscreen(window, renderer, surface, w, h, createdisploop(createpercloop(dispanchors, spacecurvepoints)),
        screen, image, writer, noscreen, dowrite, bg, fg, pixel_format_surface, enable_vsync);
      if (rotatehue)
      {
        hue += huespeed;
        hue = fmod(hue + 360, 360);
      }
      vector<point>().swap(dispanchors);
      if (not noscreen)
      {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
        {
          show_console_cursor(true);
          SDL_DestroyWindow(window);
          SDL_DestroyRenderer(renderer);
          SDL_Quit();
          cout << endl << endl;
          return 0;
        }
      }
    }
  }
  else
  {
    for (int i = 0; i < spacecurves; i++)
    {
      auto timeanchors = randanchors(w, h, timecurves);
      auto timepercanchors2 = createpercloop(timeanchors, timecurvepoints);
      timepercanchors[i] = timepercanchors2;
    }
    set_cursor(fps_cursor_pos.X, fps_cursor_pos.Y);
    cout << "fps: " << flush;
    if (dowrite)
    {
      set_cursor(percent_cursor_pos.X, percent_cursor_pos.Y);
      cout << "0% done" << flush;
    }
    t1 = steady_clock::now();
    for (;;)
    {
      for (int i2 = 0; i2 < timecurves * bs; i2++)
      {
        if (not running)
        {
          if (not noscreen)
          {
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
          }
          display_warning(percent_cursor_pos);
          return  0;
        }
        framenum++;
        if (framenum == framespan)
        {
          t2 = steady_clock::now();
          duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
          double seconds = time_span.count();
          if (seconds >= 1)
          {
            t1 = t2;
            set_cursor(fps_cursor_pos.X, fps_cursor_pos.Y);
            cout << "fps: " << int(float(framespan * framespans) / seconds) << "       " << flush;
            if (dowrite)
            {
              set_cursor(percent_cursor_pos.X, percent_cursor_pos.Y);
              cout << int(float(i2) / float(timecurves * bs) * 100) << "% done" << flush;
            }

            framespans = 1;
          }
          else
          {
            framespans++;
          }
          framenum = 0;
        }

        for (int i = 0; i < spacecurves; i++) dispanchors.push_back(timepercanchors[i][i2]);
        if (rotatehue)
        {
          hue += huespeed;
          hue = fmod(hue + 360, 360);
        }

        if (rotatehue) fg = HSVtoRGB(hue, sat, val);
        drawscreen(window, renderer, surface, w, h, createdisploop(createpercloop(dispanchors, spacecurvepoints)),
          screen, image, writer, noscreen, dowrite, bg, fg, pixel_format_surface, enable_vsync);
        vector<point>().swap(dispanchors);

        if (not noscreen)
        {
          SDL_PollEvent(&event);
          if (event.type == SDL_QUIT)
          {
            SDL_DestroyWindow(window);
            SDL_DestroyRenderer(renderer);
            SDL_Quit();
            //delete [] screen;
            //delete [] timepercanchors;
            cout << endl << endl;
            show_console_cursor(true);
            if (dowrite)
            {
              GifEnd(&writer);
              display_warning(percent_cursor_pos);
            }
            return  0;
          }
        }
      }
      if (dowrite) 
      {
        set_cursor(percent_cursor_pos.X, percent_cursor_pos.Y);
        cout << "100% done" << endl << flush; 
        show_console_cursor(true); 
        GifEnd(&writer);
        if (not noscreen)
        {
          SDL_DestroyWindow(window);
          SDL_DestroyRenderer(renderer);
          SDL_Quit();
        }
        return 0;
      }
    }
  }
}
