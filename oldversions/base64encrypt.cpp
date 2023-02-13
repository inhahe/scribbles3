// base64encrypt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <base64.h>
#include <string>

std::string text1 = 
"Parameters:\n"
"  --seed <integer> - randomization seed. use this to get the same exact pattern you did before\n"
"  --file <filename.gif> - if an output file is specified, --loop will be enabled\n"
"                          and the animation will only loop once, and it won't\n"
"                          let you close it until it's done.\n"
"  --spacepoints <integer> - number of curve anchorpoints in space\n"
"                            increase this to make more complicated shapes.defaults to 30\n"
"  --timepoints <integer> - number of curve anchorpoints in time per space anchorpoint\n"
"                           increase this to make the loops longer when specifying --loop or --file\n"
"                           defaults to 5\n"
"  --w <integer> - screen width. defaults to 1000>\n"
"  --h <integer> - screen height. defaults to 1000\n"
"  --bgcolor <six-digit hex number> - background color - defaults to #ffffff>\n"
"  --fgcolor <six-digit hex number> - foreground color - defaults to #0000ff>\n"
"  --loop - loops back on itself in time seamlessly\n"
"  --incontiguous - spacepoints don't move contiguously through time; they skip pixels\n"
"  --pointsperspacecurve <integer> - number of points calculated on each bezier curve\n"
"                                    lines are drawn between each point. defaults to 100\n"
"                                    make this 1 for a jagged effect\n"
"  --pointspertimecurve <integer> - number of points calculated on each bezier curve of change\n"
"                                   if incontiguous is not specified, points are connected by lines across time\n"
"                                   try 1 along with --discontiguous to get a rapid-fire succession of unique shapes\n";

std::string text2 = "Made by Richard A. Nichols III (Inhahe)\n";

int main()
{
  for (char& c : text1) 
  {
    c = c ^ 0b01101001;
  }
  for (char& c : text2)
  {
    c = c ^ 0b01101001;
  }
  std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(text1.c_str()), text1.length());
  std::cout << encoded << std::endl;
  std::string encoded2 = base64_encode(reinterpret_cast<const unsigned char*>(text2.c_str()), text2.length());
  std::cout << encoded2 << std::endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
