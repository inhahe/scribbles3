#include <emscripten.h>
#include <cinttypes>
static uint8_t array[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
extern "C" {uint8_t* EMSCRIPTEN_KEEPALIVE getptr() { return &array[0]; }}
int main() 
{
  return 0;
}
