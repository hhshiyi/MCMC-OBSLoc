#ifndef DATA_IO_H
#define DATA_IO_H

#include <model.h>
#include <stdio.h>

CONF initializing();
void dispConf(CONF conf);
void getStations(STATION *stns, CONF conf);
void getEvents(EVENT *evts, CONF conf);
void getBathymetry(CONF conf, POINT **bathy);
void info();
char* parseLine(FILE *fp, int len);
void shotcorrect(double x, double y, double ori, CONF conf, double *xc, double *yc);
#endif // DATA_IO_H
