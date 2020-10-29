# Traveling Salesman Problem

### Welcome to the traveling salesman competition.

The routes are located in "routes". Each route consists of a set of 2d points. Each point is represented with 2 signed bytes (-128 to 127). See example for more details.
The results are located in "results". The result file consists of the route points rearranged in the order of travel. You do not need to include the first point at the end of the file.
The distances are located in "scores".

### Top scores
    KJ_route25   1217.516937
    KJ_route50   1891.245258
    KJ_route75   2058.572518
    KJ_route100  2509.923349
    KJ_route500  5363.327143
    KJ_route1000 7373.864043

### Getting started
Create a directory with your name and start writing some code. Place your results in "results" then contact kevin to update scores.

### Rules
1. A file containing the result should start with 0xc6,0x67 because you are not allowed to change the starting point. All routes have the same starting point.
2. The result file should be the same length as the route.
3. There should be no duplicate points (This includes both the original route dataset and in the results).
4. The results should contain the same points as the route, but rearranged in whatever order. No cheating.

### Example
In route25, there are 50 bytes. The first two bytes are the starting point. 

    c667 7369 ff51 ec4a cd29 abba fbf2 46e3
    c27c f854 e81b 8de7 5a76 632e 9f33 9ac9
    3266 b70d 5831 5aa3 5d25 1705 e958 d45e
    b2ab                                   

    Point 0
    x0 = 0xc6 = -58
    y0 = 0x67 = 103

    Point 1
    x1 = 0x73 = 115
    y1 = 0x69 = 105

...

    Point 24
    x24 = 0xb2 = -78
    y24 = 0xab = -85

A soultion to route25 with distance = 1217.516937

    c667 d45e e958 ec4a f854 ff51 3266 5a76
    7369 632e 5d25 5831 1705 fbf2 e81b cd29
    b70d 9f33 8de7 9ac9 abba b2ab 46e3 5aa3
    c27c                                   

Note: The return to origin (0xc6,0x67) is ommited at the end because it is assumed that the salesman must return home.


