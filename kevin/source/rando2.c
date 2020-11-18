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

double total_distance(point*p, int n)
{
    double sum = 0;
    for(int i=0;i<n-1;i++)
    {
        sum += distance(p[i],p[i+1]);
    }
    sum += distance(p[n-1],p[0]);
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

int cross_fix(point*p, int n, int limit)
{
    int count = 0;
    again:
    if(count < limit)
    {
        for(int i=0;i<n-1;i++)
        {
            //Fix intersections
            //double dist = route_distance(p,n);
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
                        count++;
                        //printf("Swap %d %d\n",i+1,j);
                        goto again;
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
                    count++;
                    //printf("Swap %d %d\n",i+1,n-1);
                    goto again;
                }
            }
        }
    }
}



void random_path(point*x, point*y, int n)
{
    char*valid=malloc(n);
    for(int i=0;i<n;i++){valid[i]=1;}
    for(int i=0;i<n;i++)
    {
        int num = rand()%(n-i);
        int count = 0;
        int id = -1;
        for(int j=0;j<n;j++)
        {
            if(valid[j])
            {
                if(count == num)
                {
                    id = j;
                    valid[j] = 0;
                    break;
                }
                else
                {
                    count++;
                }
            }
        }
        if(id==-1){printf("Your code is broken\n"); exit(1);}
        copy_point(x[id],&y[i]);
    }
    free(valid);
}

void move_point(point*p, int a, int b, int s, int n)
{
    //Move a into b and shift
    point temp;
    if(a==b){return;}
    else if(a > b)
    {
        copy_point(p[a],&temp);
        for(int i=a;i>b;i--)
        {
            copy_point(p[i-1],&p[i]);
        }
        copy_point(temp,&p[b]);
    }
    else
    {
        copy_point(p[a],&temp);
        for(int i=a;i<b;i++)
        {
            copy_point(p[i+1],&p[i]);
        }
        copy_point(temp,&p[b]);
    }
    if(s)
    {
        if(b==n-1){swap_points(p,b,0);}
        else{swap_points(p,b,b+1);}
    }
}

int main(int argc, char *argv[])
{
    //./exe input output
    if((argc != 2) && (argc != 3) && (argc != 4))
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

    point*y=malloc(sizeof(point)*n);
    point*start=malloc(sizeof(point)*n);
    copy_path(p,start,n);
    //printf("n = %d\n",n);
    double best_dist = total_distance(p,n);
    double old_dist = total_distance(p,n);
    
    if(argc==4)
    {
        srand(atoi(argv[3]));
    }
    else
    {
        srand(0x42);
    }
    int count = 0;
    int try_cross_fix = 1;
    int iter = n*n*n;
    while(1)
    {
        random_path(start,p,n);
        //cross_fix(p,n,n*n);
        //copy_path(p,y,n);
        
        old_dist = total_distance(p,n);
        double new_dist;
        for(int k=0;k<iter;k++) // 1,000,000 passed last update
        {

            copy_path(p,y,n);

            //Try a random number of moves
            double another;
            do
            {
                another = rand()/(double)RAND_MAX;
                int a = rand()%n;
                int b = rand()%n;
                int s = rand()&1;

                move_point(y,a,b,s,n);
            } while(another < 0.5);
            
            //Check
            new_dist = total_distance(y,n);
            if (new_dist < old_dist)
            {
                //printf("New distance = %f\n",new_dist);
                copy_path(y,p,n);
                old_dist = new_dist;
                k=0;
                if(try_cross_fix)
                {
                    cross_fix(y,n,n);
                    new_dist = total_distance(y,n);
                    if(new_dist < old_dist)
                    {
                        copy_path(y,p,n);
                        old_dist = new_dist;
                    }
                }
            }
        }
        count++;
        printf("\r%d %f",count,new_dist);
        fflush(stdout);
        //Dump results
        if ((argc >= 3) && (old_dist < best_dist))
        {
            best_dist = old_dist;
            printf("\nBest distance = %f\n",best_dist);

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
            free(result);
        }
    }

    //Show some useful info
    printf("Distance = %f\n",total_distance(p,n));

    //Got 1036.948036 on route25
    
}