#include "time_loc.h"

Time initTime(int year, int month, int day, int hour, int minute, double second)
{
    Time time;
    time.year = year;
    time.month = month;
    time.day = day;
    time.hour = hour;
    time.minute = minute;
    time.second = second;

    return time;
}

void dispTime(Time time)
{
    printf("%4d-%02d-%02dT%02d:%02d:%012.9lfZ\n", time.year, time.month, time.day, time.hour, time.minute, time.second);
}

Time str2Time(char *str)
{
    Time time;
    sscanf(str, "%d-%d-%dT%d:%d:%lf", &time.year, &time.month, &time.day, &time.hour, &time.minute, &time.second);
    return time;
}

Time cal2Date(double jd)
{
    Time time;
    double tmp;
    int a, b, c, d, e;
    a = (int)(jd + 0.5);
    b = a + 1537;
    c = (int)((b - 122.1) / 365.25);
    d = (int)(365.25 * c);
    e = (int)((b - d) / 30.6001);

    time.day   = b - d - (int)(30.6001 * e);
    time.month = e - 1 - 12 * (int)(e / 14);
    time.year  = c - 4715 - (int)((7 + time.month) / 10);

    tmp = (jd - (int)jd) * (3600 * 24);
    time.hour = ((int)tmp / 3600 + 12) % 24;

    time.minute = ((int)tmp % 3600) / 60;
    time.second = (int)tmp % 60 + tmp - (int)tmp;

    return time;
}

double cal2Jday(Time time)
{
    int y, m, B;

    y = time.year;
    m = time.month;
    if (time.month <= 2)
    {
        y = time.year - 1;
        m = time.month + 12;
    }

    B = -2;
    if (time.year > 1582 || (time.year == 1582 && (time.month > 10 || (time.month == 10 && time.day >= 15))))
    {
        B = y / 400 - y / 100;
    }

    return (floor(365.25 * y) + floor(30.6001 * (m + 1)) + B + 1720996.5 + time.day + time.hour / 24.0 + time.minute / 1440.0 + time.second / 86400.0);
}
