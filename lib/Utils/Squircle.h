#ifndef Squircle_h
#define Squircle_h
#include "Arduino.h"
#include <math.h>

class Squircle {
  public:
    static void cirleToSquare(double u, double v, double& x, double& y);
    static void squareToCircle(double x, double y, double& u, double& v);
};

#endif
