# Traveling Salesman Problem

### Welcome to the traveling salesman competition.

Routes - The routes are located in "routes". Each route consists of a set of 2d points. Each point is represented with 2 signed bytes (-128 to 127). See example for more details.

Results - The results are located in "results". The result file consists of the route points rearranged in the order of travel. You do not need to include the first point at the end of the file.

Scores - The distances are located in "scores".

Images - The plots of the results are in "images".

### Top scores

#### Route25
![Alt text](images/KJ_rando_route25.png?raw=true "KJ_rando_route25")

    1. KJ_rando_route25         1036.948036
    2. KJ_double_edge_route25   1100.733427
    3. KJ_shorty_route25        1100.733427
    4. example_route25          1217.516937
    5.
#### Route50
![Alt text](images/KJ_rando2_route50.png?raw=true "KJ_rando2_route50")

    1. KJ_rando2_route50        1418.678570
    2. KJ_rando_route50         1421.332014
    3. KJ_double_edge_route50   1630.612894
    4. KJ_shorty_route50        1828.804881
    5. 
#### Route75
![Alt text](images/KJ_rando2_route75.png?raw=true "KJ_rando2_route75")

    1. KJ_rando2_route75        1750.370921
    2. KJ_rando_route75         1803.545940
    3. KJ_shorty_route75        1983.101872
    4. KJ_double_edge_route75   2073.471324
    5. 
#### Route100
![Alt text](images/KJ_rando2_route100.png?raw=true "KJ_rando2_route100")

    1. KJ_rando2_route100       2020.455552
    2. KJ_double_edge_route100  2197.143112
    3. KJ_shorty_route100       2355.275030
    4. 
    5. 
#### Route500
![Alt text](images/KJ_double_edge_route500.png?raw=true "KJ_double_edge_route500")

    1. KJ_double_edge_route500  4782.509379
    2. KJ_shorty_route500       5203.577905
    3. 
    4. 
    5. 
#### Route1000
![Alt text](images/KJ_double_edge_route1000.png?raw=true "KJ_double_edge_route1000")

    1. KJ_double_edge_route1000 6675.472068
    2. KJ_shorty_route1000      7130.381167
    3. 
    4.
    5.

### Getting started
Clone the repo

    git clone https://github.com/KevinJohnson42/salesman

Read the rules. Create a directory with your name and start writing some code. Place your results in "results" then contact kevin to update scores.

### Rules
1. No duplicating or creating new points.
2. Write your own code.

### Example
In route25, there are 50 bytes. Each pair of bytes makes a 2d point (x,y).

    67 c6 69 73 51 ff 4a ec  29 cd ba ab f2 fb e3 46
    7c c2 54 f8 1b e8 e7 8d  76 5a 2e 63 33 9f c9 9a
    66 32 0d b7 31 58 a3 5a  25 5d 05 17 58 e9 5e d4
    ab b2                                           
                          

    Point 0
    x0 = 0x67 = 103
    y0 = 0xc6 = -58
    

    Point 1
    x1 = 0x69 = 105
    y1 = 0x73 = 115
    
    ...

    Point 24
    x24 = 0xab = -85
    y24 = 0xb2 = -78
    

A soultion to route25 with distance = 1100.733427

    67 c6 7c c2 5e d4 58 e9  4a ec 54 f8 51 ff 66 32
    76 5a 69 73 31 58 2e 63  25 5d a3 5a e3 46 05 17
    f2 fb 1b e8 29 cd 33 9f  0d b7 e7 8d c9 9a ba ab
    ab b2

![Alt text](images/KJ_double_edge_route25.png?raw=true "KJ_double_edge_route25")


Note: The return to origin (0x67,0xc6) is ommited in the plot, but the distance is still accounted for.