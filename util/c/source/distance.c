#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

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


    //Get points
    n /= 2;
    point*p=malloc(sizeof(point)*n);
    for(int i=0;i<n;i++)
    {
        p[i].x = buf[2*i];
        p[i].y = buf[2*i+1];
    }

    //Sum all distances
    double total = 0;
    for(int i=0;i<n-1;i++)
    {
        total += distance(p[i],p[i+1]);
    }

    //Add return path
    total += distance(p[n-1],p[0]);

    //Show
    printf("Distance = %f\n",total);

    //Write result
    fp = fopen(argv[2],"a");
    if(fp==0){fp = fopen(argv[2],"w");}
    fprintf(fp,"%s %f\n",argv[1],total);
    fclose(fp);

    return 0;
}
