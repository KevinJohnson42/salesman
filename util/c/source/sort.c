#include <stdio.h>
#include <stdlib.h>
#include <float.h>


int find_char(char*x,char c)
{
    int i=0;
    while(x[i])
    {
        if(x[i] == c){return i;}
        i++;
    }
    return -1;
}


int main(int argc, char *argv[])
{
    if( (argc != 3) && (argc != 2) )
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

    //Count number of lines
    int lines=0;
    for(int i=0;i<n;i++)
    {
        if (buf[i] == '\n'){lines++;}
    }

    //Get scores: filename score\n
    double*scores = malloc(sizeof(double)*lines);
    int p = 0;
    for(int i=0;i<lines;i++)
    {
        //Skip filename
        p += find_char(buf+p,' ');
        p++;

        //Get score
        scores[i] = strtod(buf+p,0);

        //Skip to next line
        p += find_char(buf+p,'\n');
        p++;
    }

    //Sort scores O(N^2)
    int*sorted = malloc(sizeof(int)*lines);
    char*valid = malloc(sizeof(char)*lines);
    for(int i=0;i<lines;i++){valid[i]=1;}
    for(int i=0;i<lines;i++)
    {
        double min = DBL_MAX;
        int id = 0;
        for(int j=0;j<lines;j++)
        {
            if(valid[j])
            {
                if (scores[j] < min) {min=scores[j]; id=j;}
            }
        }
        sorted[i] = id;
        valid[id] = 0;
    }

    //Open file if needed
    if (argc == 3)
    {
        fp = fopen(argv[2],"a");
        if(fp==0){fp = fopen(argv[2],"w");}
    }

    //Show Scores
    int start=0,stop;
    for(int i=0;i<lines;i++)
    {
        //Get filename. Skip down to correct line
        int p=0;
        for(int j=0;j<sorted[i];j++)
        {
            p += find_char(buf+p,'\n');
            p++;
        }

        start = p;
        stop = start + find_char(buf+start,' ');
        buf[stop] = 0;

        //Print it
        if(argc == 2){printf("%s %f\n",buf+start,scores[sorted[i]]);}
        if(argc == 3){fprintf(fp,"%s %f\n",buf+start,scores[sorted[i]]);}

        //Replace null
        buf[stop] = ' ';
    }
    if(argc == 3){fclose(fp);}

    return 0;
}