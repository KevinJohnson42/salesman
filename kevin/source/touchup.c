#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

typedef struct
{
    double x;
    double y;
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

void move_point(point*p, int a, int b)
{
	//Assumes a > b
	point temp;
	copy_point(p[a],&temp);
	for(int i=a;i>b;i--)
	{
		copy_point(p[i-1],&p[i]);
	}
	copy_point(temp,&p[b]);
}

point* attempt_moves(point*p, int a, int n)
{
	point*y=malloc(sizeof(point)*n);
	double old_dist = total_distance(p,n);
	for(int i=a+1;i<n;i++)
	{
		copy_path(p,y,n);
		move_point(y,i,a);
		double new_dist = total_distance(y,n);
		if (new_dist < old_dist)
		{
			//printf("New distance = %f\n",new_dist);
			return y;
		}
	}
	free(y);
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

    //Find a new path
    point*new_path=0;
	do
	{
		new_path=0;
		for(int i=1;i<n-1;i++)
		{
			new_path = attempt_moves(p,i,n);
			if(new_path)
			{
				copy_path(new_path,p,n);
				free(new_path);
				break;
			}
		}
	} while(new_path);

    //Show some useful info
    printf("Distance = %f\n",total_distance(p,n));

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