#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

typedef struct
{
    double x;
    double y;
    char   v;
} point;

void copy_string(char*x, char*y)
{
    int i=0;
    while(x[i]){y[i]=x[i]; i++;}
    y[i] = 0;
}

double distance(point a, point b)
{
    double dx = a.x-b.x;
    double dy = a.y-b.y;
    return sqrt(dx*dx+dy*dy);
}

int min_distance(point*p, int a, int n)
{
    double d, min = DBL_MAX;
    int id = -1;
    for(int i=0;i<n;i++)
    {
        if (p[i].v)
        {
            d = distance(p[a],p[i]);
            if(d < min){min=d; id=i;}
        }
    }
    return id;
}

int main(int argc, char *argv[])
{
    if(argc != 3)
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


    //Generate random points
    n /= 2;
    point*p=malloc(sizeof(point)*n);
    for(int i=0;i<n;i++)
    {
        p[i].x = buf[2*i];
        p[i].y = buf[2*i+1];
        p[i].v = 1; //All points are valid
    }

    //Create a buffer for the path traveled
    int*path = malloc(sizeof(int)*(n+1));

    //Select a starting point
    int origin = 0;
    path[0] = origin;

    //Travel minimum distances from starting point
    double dist=0;
    for(int j=0;j<n-1;j++)
    {
        //Find next minimum distance
        int current = path[j];
        p[current].v=0;
        int next = min_distance(p,current,n);
        dist += distance(p[current],p[next]);
        path[j+1] = next;
    }

    //Return to origin
    int current = path[n-1];
    dist += distance(p[current],p[origin]);
    path[n] = origin;

    //Show some useful info
    printf("Distance = %f\n",dist);

    //Generate results
    char*result=malloc(2*n);
    for(int i=0;i<n;i++)
    {
        result[2*i] = buf[2*path[i]];
        result[2*i+1] = buf[2*path[i]+1];
    }

    //Write results
    fp = fopen(argv[2],"w");
    fwrite(result,1,2*n,fp);
    fclose(fp);

    /*
    //Dump start
    fp = fopen("start","w");
    fprintf(fp,"%f %f\n",p[path[0]].x,p[path[0]].y);
    fclose(fp);


    //Dump path
    fp = fopen("path","w");
    for(int i=0;i<n+1;i++)
    {
        fprintf(fp,"%f %f\n",p[path[i]].x,p[path[i]].y);
    }
    fclose(fp);

    //Dump nodes
    fp = fopen("nodes","w");
    for(int i=0;i<n+1;i++)
    {
        fprintf(fp,"%d\n",path[i]);
    }
    fclose(fp);
    */



}