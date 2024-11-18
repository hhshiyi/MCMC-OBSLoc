#include "interpolation.h"

double interpolation1D(double x0, double y0, double x1, double y1, double x)
{
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

double interpolation2D(double x, double y, POINT **grid, CONF conf)
{
    int i = (int)((x - conf.min_x) / conf.grid_interval);
    int j = (int)((y - conf.min_y) / conf.grid_interval);

    //fprintf(stdout, "%d %d\n", i, j);
    //fprintf(stdout, "%lf %lf %lf\n", grid[i][j].x, grid[i][j].y, grid[i][j].z);
    //fprintf(stdout, "%lf %lf %lf\n", grid[i+1][j].x, grid[i+1][j].y, grid[i+1][j].z);
    //fprintf(stdout, "%lf %lf %lf\n", grid[i][j+1].x, grid[i][j+1].y, grid[i][j+1].z);
    //fprintf(stdout, "%lf %lf %lf\n", grid[i+1][j+1].x, grid[i+1][j+1].y, grid[i+1][j+1].z);

    if(i >= 0 && i < conf.bx &&
       j >= 0 && j < conf.by)
    {
        double z1 = interpolation1D(grid[i][j].x, grid[i][j].z, grid[i+1][j].x, grid[i+1][j].z, x);
        //fprintf(stdout, "z1: %lf\n", z1);
        double z2 = interpolation1D(grid[i][j+1].x, grid[i][j+1].z, grid[i+1][j+1].x, grid[i+1][j+1].z, x);
        //fprintf(stdout, "z2: %lf\n", z2);
        double z3 = interpolation1D(grid[i][j].y, z1, grid[i][j+1].y, z2, y);
        //fprintf(stdout, "z3: %lf\n", z3);
        return z3;
    }
    else
    {
        fprintf(stdout, "Bathymetric data range is smaller than search range.\n");
        fprintf(stdout, "%lf %lf\n", x, y);
        exit(1);
    }
    /*
    for(int i = 0; i < conf.bx - 1; i++)
        for(int j = 0; j < conf.by - 1; j++)
        {
            if(x >= grid[i][j].x && x <= grid[i+1][j].x &&
               y >= grid[i][j].y && y <= grid[i][j+1].y)
            {
                double z1 = interpolation1D(grid[i][j].x, grid[i][j].z, grid[i+1][j].x, grid[i+1][j].z, x);
                double z2 = interpolation1D(grid[i][j+1].x, grid[i][j+1].z, grid[i+1][j+1].x, grid[i+1][j+1].z, x);
                double z3 = interpolation1D(grid[i][j].y, z1, grid[i][j+1].y, z2, y);

                return z3;
            }
            else
            {
                fprintf(stdout, "Bathymetric data range is smaller than search range: %lf %lf %lf %lf\n", grid[i][j].x, grid[i+1][j].x);
                fprintf(stdout, "%lf %lf\n", x, y);
                exit(1);
            }
        }
        */
}
