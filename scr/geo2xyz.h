#ifndef GEO2XYZ_H
#define GEO2XYZ_H

#include <model.h>

void geo2xyz(CONF conf, double lon, double lat, double ele, double *x, double *y, double *z);
void xyz2geo(CONF conf, double x, double y, double z, double *lon, double *lat, double *ele);
double deg2rad(double deg);
double rad2deg(double rad);

#endif // GEO2XYZ_H
