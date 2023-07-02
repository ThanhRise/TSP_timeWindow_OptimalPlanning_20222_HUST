### TSP with time windows - many different approaches to the problem
A delivery worker picks up goods from the warehouse (point 0) and needs to deliver the goods to 𝑁customers 1,2,...,𝑁.
Customer 𝑖 is located at point 𝑖 and has a delivery request in the period from 𝑒(𝑖) to 𝑙(𝑖) and delivery in  (𝑖) time unit (𝑠).Know that 𝑡(𝑖,𝑗) is the travel time from point 𝑖 to point 𝑗. The delivery man departs from the warehouse at time 0, Please calculate the delivery route for the delivery staff (return to the warehouse) so that the total travel time is the shortest.
-----------------------------------------------------------
Each solution is represented by a permutation 𝑠[1],𝑠[2],...,𝑠[𝑁]of 1,2,...,𝑁.
•Input
•Line 1: contains a positive integer 𝑁(1≤𝑁≤1000)
•Line 𝑖+1(𝑖=1,...,𝑁): contains 𝑒(𝑖),𝑙(𝑖)and 𝑑(𝑖)
•Line 𝑖+𝑁+2(𝑖=0,1,...,𝑁): contains the ith row of the matrix 𝑡
.•Output
•Line 1: contains 𝑁•Line 2: contains 𝑠[1],𝑠[2],...,𝑠[𝑁]
----------------------------------------------------------------
```
5
50 90 20
300 350 15
215 235 5
374 404 20
107 147 20
0 50 10 100 70 10
50 0 40 70 20 40 
10 40 0 80 60 0
100 70 80 0 70 80
70 20 60 70 0 60
10 40 0 80 60 0
```
```
output: 
path: 1 5 3 2 4
cost: 465
```