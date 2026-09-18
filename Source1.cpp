#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <termios.h>
#include <boost/regex.hpp>
using namespace boost;
using namespace std;

struct COORD { int X, Y; };

COORD get_cursor()
{
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
    smatch sm1;
    regex_search(string(buf), sm1, regex("\\[(\\d*);(\\d*)R"));
    result.Y = stoi(sm1[1]);
    result.X = stoi(sm1[2]);
  }
  tcsetattr(0, TCSANOW, &save);
  return result;
}

void set_cursor(int x = 0, int y = 0)
{
  cout << "\033[" << y << ";" << x << "H" << flush;
}

void show_console_cursor(const bool show) 
{
  cout << (show ? "\033[?25h" : "\033[?25l") << flush;// show/hide cursor
}

int main(int argc, char* argv[])
{
  COORD point;
  show_console_cursor(false);
  for (int x = 0; x < 100; x++)
  {
    point = get_cursor();
    cout << x << "%" << flush;
    set_cursor(point.X, point.Y);
    sleep(1);
  }
}
