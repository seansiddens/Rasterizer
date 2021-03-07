#include "util.h"

// Returns sample of lerp between a two points defining a linear function d = f(i)
// where d is a float and i is an integer
// [f(i0), f(i1)]
float lerp(int i0, float d0, int i1, float d1, int n) {
    return d0 + (d1 - d0) * (n - i0) / (i1 - i0);
}