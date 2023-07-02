# solve TSP problem by constraint programming with ortools
# import cp_model

from ortools.sat.python import cp_model


def TSP_CP(dist_matrix):
    # dist_matrix: distance matrix of cities
    # return: optimal tour, optimal tour length

    # define model
    model = cp_model.CpModel()

    # define variables
    n = len(dist_matrix)
    x = {}
    for i in range(n):
        for j in range(n):
            x[i, j] = model.NewIntVar(0, 1, 'x[%i,%i]' % (i, j))
    u = {}
    for i in range(n):
        u[i] = model.NewIntVar(0, n - 1, 'u[%i]' % i)

    # define constraints
    # each city is visited exactly once
    for i in range(n):
        model.Add(sum(x[i, j] for j in range(n) if j != i) == 1)
        model.Add(sum(x[j, i] for j in range(n) if j != i) == 1)
    # subtour elimination
    for i in range(1,n):
        for j in range(n):
            if i != j:
                model.Add(u[i] - u[j] + n * x[i, j] <= n - 1)

    # define objective function
    obj = model.NewIntVar(0, 100000, 'obj')
    model.Add(obj == sum(dist_matrix[i][j] * x[i, j]
              for i in range(n) for j in range(n)))
    model.Minimize(obj)

    # solve model
    solver = cp_model.CpSolver()
    status = solver.Solve(model)

    # get optimal tour
    tour = []
    if status == cp_model.OPTIMAL:
        for i in range(n):
            for j in range(n):
                if solver.Value(x[i, j]) == 1:
                    tour.append((i, j))
        for i in range(n):
            print('u[%i] = %i' % (i, solver.Value(u[i])))
    else:
        print('No solution found.')
    return tour, solver.ObjectiveValue()


if __name__ == '__main__':

    #   0   68   35    1   70   25   79   59   63   65    6   46   82   28   62
    #   92    0   43   28   37   92    5    3   54   93   83   22   17   19   96
    #   48   27    0   39   70   13   68  100   36   95    4   12   23   34   74
    #   65   42   12    0   69   48   45   63   58   38   60   24   42   30   79
    #   17   36   91   43    0    7   41   43   65   49   47    6   91   30   71
    #   51    7    2   94   49    0   24   85   55   57   41   67   77   32    9
    #   45   40   27   24   38   39    0   83   30   42   34   16   40   59    5
    #   31   78    7   74   87   22   46    0   73   71   30   78   74   98   13
    #   87   91   62   37   56   68   56   75    0   53   51   51   42   25   67
    #   31    8   92    8   38   58   88   54   84    0   10   10   59   22   89
    #   23   47    7   31   14   69    1   92   63   56    0   60   25   38   49
    #   84   96   42    3   51   92   37   75   21   97   22    0  100   69   85
    #   82   35   54  100   19   39    1   89   28   68   29   94    0   84    8
    #   22   11   18   14   15   10   17   36   52    1   50   20   57    0    4
    #   25    9   45   10   90    3   96   86   94   44   24   88   15    4    0

    dist_matrix = [[0, 50, 10, 100, 70, 10],
                  [50, 0, 40, 70, 20, 40],
                  [10, 40, 0, 80, 60, 0],
                  [100, 70, 80, 0, 70, 80],
                  [70, 20, 60, 70, 0, 60],
                  [10, 40, 0, 80, 60, 0]]

    tour, obj = TSP_CP(dist_matrix)
    print('Optimal tour: %s' % str(tour))
    print('Optimal tour length: %i' % obj)
