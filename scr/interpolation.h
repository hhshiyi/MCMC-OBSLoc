#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <model.h>

double interpolation1D(double x0, double y0, double x1, double y1, double x);
double interpolation2D(double x, double y, POINT **grid, CONF conf);

#endif // INTERPOLATION_H
