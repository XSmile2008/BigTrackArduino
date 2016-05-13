#include "Squircle.h"

// Elliptical Grid mapping
// mapping a circular disc to a square region
// input: (u,v) coordinates in the circle
// output: (x,y) coordinates in the square
void Squircle::cirleToSquare(double u, double v, double& x, double& y) {
    double u2 = u * u;
    double v2 = v * v;
    double twosqrt2 = 2.0 * sqrt(2.0);
    double subtermx = 2.0 + u2 - v2;
    double subtermy = 2.0 - u2 + v2;
    double termx1 = subtermx + u * twosqrt2;
    double termx2 = subtermx - u * twosqrt2;
    double termy1 = subtermy + v * twosqrt2;
    double termy2 = subtermy - v * twosqrt2;
    x = 0.5 * sqrt(termx1) - 0.5 * sqrt(termx2);
    y = 0.5 * sqrt(termy1) - 0.5 * sqrt(termy2);
}

// Elliptical Grid mapping
// mapping a square region to a circular disc
// input: (x,y) coordinates in the square
// output: (u,v) coordinates in the circle
void Squircle::squareToCircle(double x, double y, double& u, double& v) {
    u = x * sqrt(1.0 - y*y/2.0);
    v = y * sqrt(1.0 - x*x/2.0);
}

// int Squircle::main() {
//     double x,y;
//     double u,v;
//
//     squareToCircle(-0.789,0.654,u,v);
//     cirleToSquare(u,v,x,y);
//
//     printf("%f %f\n",u,v);
//     printf("%f %f\n",x,y);
//
//     cirleToSquare(0.31415, -0.926535,x,y);
//     squareToCircle(x,y,u,v);
//
//     printf("%f %f\n",x,y);
//     printf("%f %f\n",u,v);
//
//     return 0;
// }
