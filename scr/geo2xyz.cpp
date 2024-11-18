#include "geo2xyz.h"

void geo2xyz(CONF conf, double lon, double lat, double ele, double *x, double *y, double *z)
{
    *x = (lon - conf.origLon) * 111.111 * cos(deg2rad(lat));
    *y = (lat - conf.origLat) * 111.111;
    *z = ele;
}

void xyz2geo(CONF conf, double x, double y, double z, double *lon, double *lat, double *ele)
{
    *lat = conf.origLat + y / 111.111;
    *lon = conf.origLon + x / (111.111 * cos(deg2rad(*lat)));
    *ele = z;
}

double deg2rad(double deg)
{
    double pi = 3.141592654;
    return deg / 180.0 * pi;
}

double rad2deg(double rad)
{
    double pi = 3.141592654;
    return rad / pi * 180.0;
}
