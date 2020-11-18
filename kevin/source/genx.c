#include <stdio.h>
#include <stdlib.h>

void copy_string(char*x, char*y)
{
    int i=0;
    while(x[i]){y[i]=x[i]; i++;}
    y[i] = 0;
}

char rand_byte()
{
    return rand()&0xFF;
}

int find_duplicate(char*buf, int n)
{
    for(int i=0;i<n;i+=2)
    {
        char xi = buf[i];
        char yi = buf[i+1];
        for(int j=0;j<n;j+=2)
        {
            if(i != j)
            {
                char xj = buf[j];
                char yj = buf[j+1];
                if((xi==xj)&&(yi==yj))
                {
                    return j;
                }
            }
        }
    }
    return 0;
}


int main(int argc, char *argv[])
{
    int points=2;


    //Buffer
    int n = 2*(points + 2*(points-1) + 5*(points-2) + 1);
    char*buf = malloc(n);
    int total=0;

    //Left
    for(int i=0;i<points;i++)
    {
        buf[2*total] = -100;
        buf[2*total+1] = -100 + (200*i)/(points-1);
        total++;
    }

    //Top
    for(int i=0;i<points-1;i++)
    {
        buf[2*total] = -100 + (200*(i+1))/(points-1);
        buf[2*total+1] = 100;
        total++;
    }

    //Right
    for(int i=0;i<points-1;i++)
    {
        buf[2*total] = 100;
        buf[2*total+1] = 100 - (200*(i+1))/(points-1);
        total++;
    }

    //Bottom
    for(int i=0;i<points-2;i++)
    {
        buf[2*total] = 100 - (200*(i+1))/(points-1);
        buf[2*total+1] = -100;
        total++;
    }

    //Top-Right
    for(int i=0;i<points-2;i++)
    {
        buf[2*total] = (100*(i+1))/(points-1);
        buf[2*total+1] = (100*(i+1))/(points-1);
        total++;
    }

    //Top-Left
    for(int i=0;i<points-2;i++)
    {
        buf[2*total] = -(100*(i+1))/(points-1);
        buf[2*total+1] = (100*(i+1))/(points-1);
        total++;
    }

    //Bot-Right
    for(int i=0;i<points-2;i++)
    {
        buf[2*total] = (100*(i+1))/(points-1);
        buf[2*total+1] = -(100*(i+1))/(points-1);
        total++;
    }

    //Bot-Left
    for(int i=0;i<points-2;i++)
    {
        buf[2*total] = -(100*(i+1))/(points-1);
        buf[2*total+1] = -(100*(i+1))/(points-1);
        total++;
    }

    //Middle
    buf[2*total] = 0;
    buf[2*total+1] = 0;
    total++;

    if((n/2) != total){printf("Fail\n"); exit(1);}
    printf("Points = %d\n",total);

    
    //Write file
    char title[] = "route";
    char filename[64];
    copy_string(title,filename);
    FILE*fp;
    fp = fopen(filename,"w");
    fwrite(buf,1,n,fp);
    fclose(fp);

    //set term png
    //set output 'test.png'
    //plot 'route100' binary format='%char' using 1:2 w linespoints
}