#ifndef FORWARD_H
#define FORWARD_H

#include <math.h>
#include <stdio.h>

#include <model.h>
#include <time_loc.h>

double forward(double rx, double ry, double rz, double sx, double sy, double sz);
double distance(double rx, double ry, double rz, double sx, double sy, double sz);
void synthetics(EVENT *evts, STATION *stns, CONF conf);
double normal(double mu, double sigma);
double gaussian_noise(CONF conf);

#endif // FORWARD_H
