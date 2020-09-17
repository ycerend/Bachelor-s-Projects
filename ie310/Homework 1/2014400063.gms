SETS
    i sectors / 1*10 /
      j d_sites / 1*5 /;

TABLE d(i,j) distances (in kilometers) for ten sectors in the city
    1   2   3   4   5
1   3.4 1.4 4.9 7.4 9.3
2   2.4 2.1 8.3 9.1 8.8
3   1.4 2.9 3.7 9.4 8.6
4   2.6 3.6 4.5 8.2 8.9
5   1.5 3.1 2.1 7.9 8.8
6   4.2 4.9 6.5 7.7 6.1
7   4.8 6.2 9.9 6.2 5.7
8   5.4 6   5.2 7.6 4.9
9   3.1 4.1 6.6 7.5 7.2
10  3.2 6.5 7.1 6   8.3;

PARAMETER c(j) capacity of j th d_site / 1 350, 2 250, 3 500, 4 400, 5 200 /;

PARAMETER req(i) estimated the snow removal requirements (in 1000s of cubic meters) for each sector in the coming year / 1 153, 2 152,3 154, 4 138, 5 127, 6 129, 7 111, 8 110, 9 130, 10 135 /;

VARIABLES
Z   total cost for snow removal and disposal
BINARY VARIABLE X(i,j) 0 or 1 indicating assignment of d_site j to sector i   ;

EQUATIONS
COST cost for snow removal and disposal
ASSIGN(i) sector-disposal sites assignment constraints
CAPACITY(j) disposal site capacity constraints;

COST .. Z =e= SUM((i,j),0.1*1000*d(i,j)*X(i,j)*req(i));
ASSIGN(i) .. SUM(j,X(i,j)) =e= 1;
CAPACITY(j) .. SUM(i, X(i,j)*req(i)) =l= c(j);

MODEL  GAP /ALL/ ;
Option MIP = Cplex;
option optca=0;
option optcr=0;
SOLVE GAP USING MIP MINIMIZING Z;

DISPLAY X.L;
