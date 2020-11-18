#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

unsigned long long debug_count = 0;

typedef struct
{
    double x;
    double y;
} point;


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

void swap_points(point*p, int a, int b)
{
    point t;
    copy_point(p[a],&t);
    copy_point(p[b],&p[a]);
    copy_point(t,&p[b]);
}

int swap_points_opt(double*d, point*p, int a, int b, int n)
{
    //1 on success
    debug_count++;
    double new;
    swap_points(p,a,b);
    new = route_distance(p,n);
    if((*d) <= new){swap_points(p,a,b); return 0;}
    *d = new;
    return 1;
}


int swap_all_pairs(double*d, point*p, int n)
{
    //1 on success
    for(int i=0;i<n;i++)
    for(int j=i+1;j<n;j++)
    {
        if(swap_points_opt(d,p,i,j,n)){return 1;}
    }
    return 0;
}

int swap_all_pairs_depth(int depth, double*d, point*p, int n)
{
    if(depth == 1)
    {
        return swap_all_pairs(d,p,n);
    }
    for(int i=0;i<n;i++)
    {
        for(int j=i;j<n;j++)
        {
            swap_points(p,i,j);
            if(swap_all_pairs_depth(depth-1,d,p,n)){return 1;}
            swap_points(p,i,j);
        }
    }
    return 0;
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

    //Get the distance matrix
    double d = route_distance(p,n);

    //Find a new path
    int depth = 3;
    int changed = 1;
    while(changed)
    {
        changed = swap_all_pairs_depth(depth,&d,p,n);
        printf("Update = %f, Count = %llu\n",d,debug_count);
    }

    //Show some useful info
    printf("Distance = %f\n",route_distance(p,n));

    //Dump results
    if (argc == 3)
    {
    	//Generate results
	    char*result=malloc(2*n);
	    for(int i=0;i<n;i++)
	    {
	        result[2*i] = p[i].x;
	        result[2*i+1] = p[i].y;
	    }

	    //Write results
	    fp = fopen(argv[2],"w");
	    fwrite(result,1,2*n,fp);
	    fclose(fp);
    }
}