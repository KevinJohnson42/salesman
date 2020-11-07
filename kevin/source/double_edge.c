#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

typedef struct
{
    int id;
    double x;
    double y;
} point;

typedef struct
{
    point p;
    char ec; //Edge count
    point e[2];
} vertex;

void copy_point(point x, point*y)
{
    y->id = x.id;
    y->x = x.x;
    y->y = x.y;
}

int set_edge(vertex*v1, vertex*v2)
{
    //Set the next edge of v1 to v2
    if((v1->ec) > 1){return -1;}
    copy_point(v2->p, &v1->e[v1->ec]);
    v1->ec += 1;
    return 0;
}

int remove_edge(vertex*v1)
{
    //Set the next edge of v1 to v2
    if((v1->ec) <= 0){return -1;}
    v1->ec--;
    return 0;
}

int edge_check(vertex v1, vertex v2)
{
    //Return 0 on success
    if((v1.ec==2)||(v2.ec==2)){return -1;}
    if((v1.ec==1)&&(v1.e[0].id==v2.p.id)){return -1;}
    if((v2.ec==1)&&(v2.e[0].id==v1.p.id)){return -1;}
    return 0;
}

int loop_check(vertex*v, int v1, int v2, int n)
{
    int fail;
    fail = set_edge(&v[v1],&v[v2]);
    if(fail){printf("FAIL\n"); exit(-1);}
    fail = set_edge(&v[v2],&v[v1]);
    if(fail){printf("FAIL\n"); exit(-1);}
    for(int i=0;i<n;i++)
    {
        int start, current, previous;
        start = v[i].p.id;
        previous = -1;
        current = start;
        do
        {
            if(v[current].ec == 0) {break;}
            if(v[current].e[0].id != previous)
            {
                previous = current;
                current = v[current].e[0].id;
            }
            else
            {
                if(v[current].ec == 1) {break;}
                previous = current;
                current = v[current].e[1].id;
            }
            
        } while(current != start);
        if((previous != -1)&&(current == start))
        {
            remove_edge(&v[v1]);
            if(fail){printf("FAIL\n"); exit(-1);}
            remove_edge(&v[v2]);
            if(fail){printf("FAIL\n"); exit(-1);}
            return -1;
        }
    }
    remove_edge(&v[v1]);
    if(fail){printf("FAIL\n"); exit(-1);}
    remove_edge(&v[v2]);
    if(fail){printf("FAIL\n"); exit(-1);}
    return 0;
}

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

int min_distance(vertex*v, double*dm, int n)
{
    int y = -1;
    double min = DBL_MAX;
    for(int i=0;i<n;i++)
    for(int j=0;j<n;j++)
    {
        int id = i*n+j;
        if((dm[id]!=0)&&(dm[id]<min))
        {
            if(edge_check(v[i],v[j]))
            {
                //Skip
                //printf("Skipped\n");
            }
            else
            {
                if (loop_check(v,i,j,n))
                {
                    //Skip
                    //printf("LOOP\n");
                }
                else
                {
                    min = dm[id];
                    y = id;
                    //printf("i=%d, j=%d, id=%d\n",i,j,id);
                }
            }
        }
    }
    return y;
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
    vertex*v=malloc(sizeof(vertex)*n);
    for(int i=0;i<n;i++)
    {
        v[i].p.id = i;
        v[i].p.x = buf[2*i];
        v[i].p.y = buf[2*i+1];
        v[i].ec  = 0;
    }

    //Compute distance matrix
    int n2 = n*n;
    double*dm = malloc(sizeof(double)*n2);
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            dm[i*n+j] = distance(v[i].p,v[j].p);
        }
    }

    //Find minimum edge
    for(int i=0;i<n-1;i++)
    {
        //Get the next shortest edge
        int id = min_distance(v,dm,n);
        if(id==-1){printf("Your logic is flawed\n");goto debug;}

        //Decode the Vertex id
        int v1,v2;
        v1 = id/n;
        v2 = id%n;
        //printf("Got %d %d\n",v1,v2);

        //Set vertex edges, both ways
        int fail;
        fail = set_edge(&v[v1],&v[v2]);
        if(fail){printf("Your logic is still flawed\n");goto debug;}
        fail = set_edge(&v[v2],&v[v1]);
        if(fail){printf("Your logic is still flawed\n");goto debug;}
    }

    //Last connection
    int v1=-1,v2=-1;
    for(int i=0;i<n;i++)
    {
        if(v[i].ec == 1)
        {
            printf("Vertex ID = %d\n",i);
            if(v1==-1){v1=i;}
            else {v2=i;}
        }
    }
    if((v1==-1)||(v2==-1)){printf("Still broken\n"); goto debug;}
    int fail;
    fail = set_edge(&v[v1],&v[v2]);
    if(fail){printf("Your logic is still flawed\n");goto debug;}
    fail = set_edge(&v[v2],&v[v1]);
    if(fail){printf("Your logic is still flawed\n");goto debug;}
    printf("Done!\n");
    
    //return 0;

    //Create a buffer for the path traveled
    int*path = malloc(sizeof(int)*(n+1));


    //Loop
    int previous, current;
    current = 0;
    previous -1;
    for(int i=0;i<n;i++)
    {
        path[i] = current;
        if (v[current].e[0].id != previous)
        {
            previous = current;
            current = v[current].e[0].id;
        }
        else
        {
            previous = current;
            current = v[current].e[1].id;
        }
    }

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
    return 0;


    debug:
    for(int i=0;i<n;i++)
    {
        printf("Vertex %d\n",i);
        printf("Point = %d,%d\n",(char)v[i].p.x,(char)v[i].p.y);
        printf("Count = %d\n",v[i].ec);
        printf("Edge 1 = %d,%d\n",(char)v[i].e[0].x,(char)v[i].e[0].y);
        printf("Edge 2 = %d,%d\n",(char)v[i].e[1].x,(char)v[i].e[1].y);
        printf("\n");
    }
    return -1;
}