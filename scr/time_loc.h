#ifndef TIME_H
#define TIME_H

#include <stdio.h>
#include <math.h>

typedef struct TIME{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;
} Time;

Time initTime(int year, int month, int day, int hour, int minute, double second);
void dispTime(Time time);
Time str2Time(char *str);
Time cal2Date(double jd);
double cal2Jday(Time time);

#endif // TIME_H
