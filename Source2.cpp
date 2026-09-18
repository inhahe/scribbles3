#include <unistd.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
  for (int x = 0; x < 100; x++)
  {
    try
    {
      cout << x << "%" << flush;
      sleep(1);
    }
    catch (std::invalid_argument const& ex)
    {
      std::cout << "#1: " << ex.what() << '\n';
    }
  }
}
