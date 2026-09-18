#include <stdlib.h>

extern "C" {

int sum(int a[], int len)
{
    int sum = 0;
    for (int i = 0; i < len; i++)
    {
        sum += a[i];
        a[i] = 255;
    }
    return sum;
}
}

