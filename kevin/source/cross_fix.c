#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

//#define NAN 0.0/0.0

typedef struct
{
    double x;
    double y;
} point;

void copy_point(point x, point*y)
{
    y->x = x.x;
    y->y = x.y;
}

void swap_points(point*p, int a, int b)
{
    point t;
    copy_point(p[a],&t);
    copy_point(p[b],&p[a]);
    copy_point(t,&p[b]);
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
    for(int i=0;i<n;i++)
    {
        sum += distance(p[i],p[i+1]);
    }
    sum += distance(p[n-1],p[0]);
    return sum;
}

int swap_points_opt(point*p, int a, int b, int n)
{
    //1 on success
    double old,new;
    old = route_distance(p,n);
    swap_points(p,a,b);
    new = route_distance(p,n);
    if(old < new){swap_points(p,a,b); return 0;}
    return 1;
}

point intersection(point a1, point a2, point b1, point b2)
{
    //y1 = m1(x-xa0)+ya0
    //y2 = m2(x-xb0)+yb0
    //m1(x-xa0)+ya0 = m2(x-xb0)+yb0
    //m1*x - m1*xa0 + ya0 = m2*x - m2*xb0 + yb0
    //m1*x - m2*x = m1*xa0 - ya0 - m2*xb0 + yb0
    //x = (m1*xa0 - ya0 - m2*xb0 + yb0) / (m1 - m2)
    point ret;
    double adx = a2.x - a1.x;
    double ady = a2.y - a1.y;
    double bdx = b2.x - b1.x;
    double bdy = b2.y - b1.y;
    if((adx==0)&&(bdx==0))
    {
        //printf("Parallel vertical lines\n");
        ret.x = NAN;
        ret.y = NAN;
        return ret;
    }
    else if(adx==0)
    {
        //printf("A vertical line\n");
        ret.x = a1.x;
        double mb = bdy / bdx;
        ret.y = mb*(ret.x-b1.x)+b1.y;
        return ret;
    }
    else if(bdx==0)
    {
        //printf("B vertical line\n");
        ret.x = b1.x;
        double ma = ady / adx;
        ret.y = ma*(ret.x-a1.x)+a1.y;
        return ret;
    }

    double ma = ady / adx;
    double mb = bdy / bdx;
    if(ma==mb)
    {
        //printf("Parallel lines\n");
        ret.x = NAN;
        ret.y = NAN;
        return ret;
    }
    
    
    ret.x = (ma*a1.x - a1.y - mb*b1.x + b1.y)/(ma - mb);
    ret.y = ma*(ret.x-a1.x)+a1.y;
    //printf("Intersection = %f,%f\n",ret.x,ret.y);
    return ret;
}

int intersect(point a1, point a2, point b1, point b2)
{
    point p = intersection(a1,a2,b1,b2);
    if((p.x != p.x)||(p.y != p.y)){return 0;}
    if(((p.x > a1.x)&&(p.x > a2.x))||(p.x < a2.x)&&(p.x < a1.x)){return 0;}
    if(((p.x > b1.x)&&(p.x > b2.x))||(p.x < b2.x)&&(p.x < b1.x)){return 0;}
    if(((p.y > a1.y)&&(p.y > a2.y))||(p.y < a2.y)&&(p.y < a1.y)){return 0;}
    if(((p.y > b1.y)&&(p.y > b2.y))||(p.y < b2.y)&&(p.y < b1.y)){return 0;}
    return 1;
}


int main(int argc, char *argv[])
{
    //./exe input output
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


    //Extract points
    n /= 2;
    point*p=malloc(sizeof(point)*n);
    for(int i=0;i<n;i++)
    {
        p[i].x = buf[2*i];
        p[i].y = buf[2*i+1];
    }

    //Fix crosses - TODO: check last to first
    again:;
    for(int i=0;i<n-1;i++)
    {
        //Fix intersections
        double dist = route_distance(p,n);
        for(int j=i;j<n-1;j++)
        {
            if((i != j)&&((i+1) != j)) //Not the same line
            {
                if(intersect(p[i],p[i+1],p[j],p[j+1]))
                {
                    /*
                    printf("Intersection found!\n");
                    if(swap_points_opt(p,i+1,j,n))
                    {
                        printf("Swap %d %d\n",i+1,j);
                        goto again;
                    }
                    */
                    swap_points(p,i+1,j);
                    printf("Swap %d %d\n",i+1,j);
                    goto done;
                }
            }
        }
        if((i != n-1)&&((i+1) != n-1)&&(i != 0)) //Not the same line
        {
            if(intersect(p[i],p[i+1],p[n-1],p[0]))
            {
                /*
                printf("Intersection found!\n");
                if(swap_points_opt(p,i+1,n-1,n))
                {
                    printf("Swap %d %d\n",i+1,n-1);
                    goto again;
                }
                */
                swap_points(p,i+1,n-1);
                printf("Swap %d %d\n",i+1,n-1);
                goto done;
            }
        }
    }


    done:;
    //Generate results
    char*result=malloc(2*n);
    for(int i=0;i<n;i++)
    {
        result[2*i] = (char)p[i].x;
        result[2*i+1] = (char)p[i].y;
    }

    //Write results
    fp = fopen(argv[2],"w");
    fwrite(result,1,2*n,fp);
    fclose(fp);

}