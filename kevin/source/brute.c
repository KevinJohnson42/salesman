#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>


typedef struct
{
    double x;
    double y;
} point;

unsigned int factorial(unsigned int x)
{
    unsigned int y = 1;
    for(int i=1;i<=x;i++){y *= i;}
    return y;
}


double distance(point a, point b)
{
    double dx = a.x-b.x;
    double dy = a.y-b.y;
    return sqrt(dx*dx+dy*dy);
}

double route_distance(point*p, int n)
{
    double sum = 0;
    for(int i=0;i<n-1;i++)
    {
        sum += distance(p[i],p[i+1]);
    }
    sum += distance(p[n-1],p[0]);
    return sum;
}

double* distance_matrix(point*p, int n)
{
    double*dm = malloc(sizeof(double)*n*n);
    for(int i=0;i<n;i++)
    for(int j=0;j<n;j++)
    {
        dm[i*n+j] = distance(p[i],p[j]);
    }
    return dm;
}

double route_distance_matrix(double*dm, point*p, int n)
{
    double sum = 0;
    for(int i=0;i<n-1;i++)
    {
        sum += dm[n*i + i+1];
    }
    sum += dm[n*(n-1) + 0];
    return sum;
}

void copy_point(point x, point*y)
{
	y->x = x.x;
	y->y = x.y;
}

void copy_path(point*x, point*y, int n)
{
	for(int i=0;i<n;i++)
	{
		copy_point(x[i],&y[i]);
	}
}

void rotate_path(point*p, int n)
{
    //To the right
    point t;
    copy_point(p[n-1],&t);
    for(int i=0;i<n-1;i++)
    {
        p[n-1-i] = p[n-2-i];
    }
    copy_point(t,&p[0]);
}


void swap_points(point*p, int a, int b)
{
    point t;
    copy_point(p[a],&t);
    copy_point(p[b],&p[a]);
    copy_point(t,&p[b]);
}


void brute(double*best_dist, point*best_route, point*route, int k, point*p, int n, unsigned int*count, unsigned int limit)
{
    if(k == n-1)
    {
        copy_point(p[k],&route[k]);
        return;
    }
    for(int i=k;i<n;i++)
    {
        copy_point(p[i],&route[k]);
        swap_points(p,k,i);
        brute(best_dist,best_route,route,k+1,p,n,count,limit);
        swap_points(p,k,i);
        double d = route_distance(route,n);
        if(d < (*best_dist))
        {
            *best_dist = d;
            copy_path(route,best_route,n);
        }
        if(*count == limit)
        {
            return; //Need to exit recursive brute
        }
        else
        {
            *count = *count + 1;
        }
    }
}

int main(int argc, char *argv[])
{
    //./exe input output
    if((argc != 2) && (argc != 3))
    {
        printf("Incorrect number of arguments\n");
        exit(1);
    }

    //Open file
    FILE*fp;
    fp = fopen(argv[1],"r");
    if(fp == 0)
    {
        printf("Cannot open file\n");
        exit(2);
    }

    //Read file into buffer
    fseek(fp,0,SEEK_END);
    int n = ftell(fp);
    fseek(fp,0,SEEK_SET);
    char*buf = malloc(sizeof(char)*n);
    int r = fread(buf,1,n,fp);
    if(r != n)
    {
        printf("Read error\n");
        exit(3);
    }
    fclose(fp);


    //Extract points
    n /= 2;
    point*p=malloc(sizeof(point)*n);
    for(int i=0;i<n;i++)
    {
        p[i].x = buf[2*i];
        p[i].y = buf[2*i+1];
    }

    //Find a new path
    double min_dist = route_distance(p,n);
    point*min_route = malloc(sizeof(point)*n);
    copy_path(p,min_route,n);
    point*route = malloc(sizeof(point)*n);
    copy_path(p,route,n);


    //Brute force all the way
    unsigned int count = 0; 

    //N!
    //brute(&min_dist,min_route,route,0,p,n,&count,factorial(n));

    //(N-1)!
    //brute(&min_dist,min_route,route,1,p,n,&count,factorial(n-1));

    //(N-1)!/2
    brute(&min_dist,min_route,route,1,p,n,&count,factorial(n-1)/2);

    //Brute force kinda
    /*
    double best_dist = min_dist;
    int depth = 8;
    repeat:;
    for(int i=0;i<n;i++)
    {
        //Consecutive brute search
        rotate_path(p,n);
        copy_path(p,route,n);
        brute(&min_dist,min_route,route,n-depth,p,n);
        copy_path(min_route,p,n);
        if(min_dist < best_dist){best_dist=min_dist;goto repeat;}

        //Swaps
        if(0)
        {
            for(int j=0;j<n-depth;j++)
            {
                for(int k=n-depth;k<n;k++)
                {
                    swap_points(p,j,k);
                    copy_path(p,route,n);
                    brute(&min_dist,min_route,route,n-depth,p,n);
                    copy_path(min_route,p,n);
                    if(min_dist < best_dist){best_dist=min_dist;goto repeat;}
                }
            }
        }
    }
    */

    //Show some useful info
    printf("Debug Count = %d\n",count);
    printf("Distance = %f\n",min_dist);

    //Dump results
    if (argc == 3)
    {
    	//Generate results
	    char*result=malloc(2*n);
	    for(int i=0;i<n;i++)
	    {
	        result[2*i] = min_route[i].x;
	        result[2*i+1] = min_route[i].y;
	    }

	    //Write results
	    fp = fopen(argv[2],"w");
	    fwrite(result,1,2*n,fp);
	    fclose(fp);
    }
}