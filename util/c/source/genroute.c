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
    if(argc != 2)
    {
        printf("Incorrect number of arguments\n");
        exit(1);
    }
    int n = 2*atoi(argv[1]);

    //Buffer random data
    char*buf = malloc(n);
    for(int i=0;i<n;i++){buf[i]=rand_byte();}

    //Find duplicates and replace them
    int duplicate = 1;
    while(duplicate > 0)
    {
        duplicate = find_duplicate(buf,n);
        if(duplicate)
        {
            buf[duplicate] = rand_byte();
            buf[duplicate+1] = rand_byte();
            printf("Duplicate replaced!\n");
        }
    }
    
    //Write file
    char title[] = "route";
    char filename[64];
    copy_string(title,filename);
    copy_string(argv[1],filename+sizeof(title)-1);
    FILE*fp;
    fp = fopen(filename,"w");
    fwrite(buf,1,n,fp);
    fclose(fp);

    //set term png
    //set output 'test.png'
    //plot 'route100' binary format='%char' using 2:1 w linespoints
}