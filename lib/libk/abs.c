#include <phoenix/kernel.h>

int abs(int x);

int abs(int x)
{
    /* t is -1 if x in negative otherwise t is 0 */
    int t = x >> 31;

    return t ^ (x + t);
}
