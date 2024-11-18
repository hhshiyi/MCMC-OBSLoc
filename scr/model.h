#ifndef MODEL_H
#define MODEL_H

#include <time_loc.h>

// bathmetry grid
typedef struct Point
{
    double x, y, z;
} POINT;

// structure statement for station
typedef struct STN
{
    int id;         // station ID
    char stnm[15];  // station name
    double slat;    // station latitude in km or degree
    double slon;    // station longitude in km or degree
    double sele;    // station elevation in km
    double x, y, z;
    double ori;
} STATION;

// structure statement for arrival time or travel time
typedef struct ARR
{
    int nstn;       // station ID
    int ikps;       // phase identification
    double tt;      // travel time or arrival time in second
    //double syn_tt;
} ARRIV;

// structure statement for each event
typedef struct EVT
{
    int id;         // event ID

    // reference origin time of the event
    Time origin;    // date of reference origin time

    double dsec;    // second uncertainty of reference origin time

    // reference location of the event
    double elat, dlat;     // event latitude and its uncertainty
    double elon, dlon;     // event longitude and its uncertainty
    double depth, ddepth;  // event depth and its uncertainty
    double x, y, z;
    double mag;            // magnitude
    int nump, nums;        // number of P and S related travel times or arrival times
    char phase[15];        // seismic phase identification
    ARRIV *arrivals;       // arrivals
} EVENT;

// structure statement for configure file
typedef struct Config
{
    int verb;            // switch for output information on screen
    Time refTime;        // reference time
    //int icoordinate;     // switch for coordinate option
    int isyn;            // switch for synthetic calculation
    int inoise;
    int iout_loc;        // switch for write locations to file
    int ishotcorr;       //
    double vlen;
    int nstn;            // number of stations
    int neqs;            // number of events
    int ifix;
    int bx, by;
    double grid_interval;
    char file_stn[200];  // file name of input stations
    char file_pick[200]; // file name of input travel times
    char file_loc[200];  // file name of output locations
    char file_bath[200]; // file name of input bathmetry
    double stddev_noise;
    double min_x, max_x; // min and max searching area along X-axis or W-E
    double min_y, max_y; // min and max searching area along y-axis or S-N
    double min_z, max_z; // min and max searching area along Z-axis or depth
    double origLon;      // searching interval along X-axis or W-E
    double origLat;      // searching interval along Y-axis or S-N
    double rotAng;       // searching interval along Z-axis or depth

    // parameters of Monte Carlo
    int algorithm;       // Metropolis or Gibbs
    int seed;            // random seed
    int nsamples;        // total number of model samples of Metroplis-Hastings algorithm
    int savesamples;     // the last samples used to calculate the expectations
    double delta_x;      // sigma for X location or longitude
    double delta_y;      // sigma for Y location or latitude
    double delta_z;      // sigma for Z location or depth
    double delta_t;      // sigma for travel time
    double sigma_t;
} CONF;

#endif // MODEL_H
