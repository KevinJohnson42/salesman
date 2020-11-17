#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NAN 0.0/0.0

typedef struct
{
    double x;
    double y;
} point;

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
		printf("Parallel vertical lines\n");
		ret.x = NAN;
		ret.y = NAN;
		return ret;
	}
	else if(adx==0)
	{
		printf("A vertical line\n");
		ret.x = a1.x;
		double mb = bdy / bdx;
		ret.y = mb*(ret.x-b1.x)+b1.y;
		return ret;
	}
	else if(bdx==0)
	{
		printf("B vertical line\n");
		ret.x = b1.x;
		double ma = ady / adx;
		ret.y = ma*(ret.x-a1.x)+a1.y;
		return ret;
	}

	double ma = ady / adx;
	double mb = bdy / bdx;
	if(ma==mb)
	{
		printf("Parallel lines\n");
		ret.x = NAN;
		ret.y = NAN;
		return ret;
	}
	
	
	ret.x = (ma*a1.x - a1.y - mb*b1.x + b1.y)/(ma - mb);
	ret.y = ma*(ret.x-a1.x)+a1.y;
	printf("Intersection = %f,%f\n",ret.x,ret.y);
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

int main()
{
	srand(time(0));
	point a1,a2,b1,b2;
	a1.x = rand()/(double)RAND_MAX;
	a1.y = rand()/(double)RAND_MAX;
	a2.x = rand()/(double)RAND_MAX;
	a2.y = rand()/(double)RAND_MAX;
	b1.x = rand()/(double)RAND_MAX;
	b1.y = rand()/(double)RAND_MAX;
	b2.x = rand()/(double)RAND_MAX;
	b2.y = rand()/(double)RAND_MAX;

	printf("%d\n",intersect(a1,a2,b1,b2));

	FILE*fp;
	
	//A segment
	fp = fopen("seg1","w");
	fprintf(fp,"%f %f\n",a1.x,a1.y);
	fprintf(fp,"%f %f\n",a2.x,a2.y);
	fclose(fp);

	//B segment
	fp = fopen("seg2","w");
	fprintf(fp,"%f %f\n",b1.x,b1.y);
	fprintf(fp,"%f %f\n",b2.x,b2.y);
	fclose(fp);

	//Point
	point p = intersection(a1,a2,b1,b2);
	fp = fopen("point","w");
	fprintf(fp,"%f %f\n",p.x,p.y);
	fclose(fp);
}