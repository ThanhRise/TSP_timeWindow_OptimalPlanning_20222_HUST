# CONSTAINTS PROGRAMMINGV- TSP with time window
# -----------------------------------------------

# constraints:
# each city is visited exactly once
# subtour elimination
# time window

from ortools.sat.python import cp_model


def TSP_CP(n, time_matrix, dist_matrix):
    model = cp_model.CpModel()
    # X(i,j) = 1 if i -> j else 0
    # define variables
    num_nodes = n + 1
    e = {}  # earliest time to visit city i
    l = {}  # latest time to visit city i
    d = {}  # duration time to visit city i
    x = {}  # x[i,j] = 1 if i -> j else 0
    C = {}  # C[i,j] = d[i] + dist_matrix[i][j]
    M = {}  # M[i] = time to visit city i
    M[0] = 0
    d[0] = 0
    e[0] = 0
    l[0] = 100000
    for i in range(1, num_nodes):
        e[i] = time_matrix[i-1][0]
        l[i] = time_matrix[i-1][1]
        d[i] = time_matrix[i-1][2]
    for i in range(num_nodes):
        for j in range(num_nodes):
            x[i, j] = model.NewIntVar(0, 1, 'x[%i,%i]' % (i, j))
            C[i, j] = d[i] + dist_matrix[i][j]
    for i in range(1, num_nodes):
        M[i] = model.NewIntVar(e[i], l[i], 'M[%i]' % i)

    # u[i] = j if city i is visited at position j
    u = {}
    for i in range(num_nodes):
        u[i] = model.NewIntVar(0, num_nodes - 1, 'u[%i]' % i)
 
    # time waiting at city i
    w = {}
    for i in range(num_nodes):
        w[i] = model.NewIntVar(0, l[i] - e[i], 'w[%i]' % i)

    # define constraints
    # each city is visited exactly once
    for i in range(num_nodes):
        model.Add(sum(x[i, j] for j in range(num_nodes) if j != i) == 1)
        model.Add(sum(x[j, i] for j in range(num_nodes) if j != i) == 1)

    # subtour elimination
    for i in range(1,num_nodes):
        for j in range(num_nodes):
            if i != j:
                model.Add(u[i] - u[j] + num_nodes * x[i, j] <= num_nodes - 1)

    # time window
    for i in range(num_nodes): 
        for j in range(1, num_nodes):
            if i != j:
                model.Add(M[i] + C[i, j]*x[i, j] - M[j] <= (1 - x[i, j]) * 100000)

    for i in range(num_nodes):
        for j in range(1, num_nodes):
            if i != j:
                model.Add(w[j] == M[j] - M[i] - C[i, j]).OnlyEnforceIf(x[i, j])

    # objective
    model.Minimize(sum(C[i, j] * x[i, j] for i in range(num_nodes) for j in range(num_nodes) if j != i) + sum(w[i] for i in range(num_nodes)))

    # solve
    solver = cp_model.CpSolver()
    status = solver.Solve(model)

    if status == cp_model.OPTIMAL:
        print(num_nodes - 1)
        print('Minimum cost = %i' % solver.ObjectiveValue())
        print()
        # for i in range(num_nodes):
        #     for j in range(num_nodes):
        #         if solver.Value(x[i, j]) == 1:
        #             print('%i -> %i' % (i, j))
        # for i in range(num_nodes):
        #     print('M[%i] = %i' % (i, solver.Value(M[i])))
        #     print('w[%i] = %i' % (i, solver.Value(w[i])))
        # print()

        solution = {}
        for i in range(1, num_nodes):
            print('w[%i] = %i' % (i, solver.Value(w[i])))
            solution[solver.Value(u[i])] = i
        for i in range(num_nodes- 1):
            print(solution[i], end=' ')
        print()
        print('Time = ', solver.WallTime(), ' milliseconds')


if __name__ == '__main__':

    # input from file
    input_data = []
    customers = []
    t = []
    with open('input.txt', 'r') as file:
    # Read the input
        N = int(file.readline())
        
        for i in range(1, N+1):
            e, l, d = map(int, file.readline().split())
            customers.append([e, l, d])

        for _ in range(N+1):
            row = list(map(int, file.readline().split()))
            t.append(row)
    TSP_CP(N, customers, t)
# 15 18 4 3 2 16 14 11 8 17 7 1 6 5 13 9 10 12 19
# 15 18 4 2 3 16 14 11 8 17 7 1 6 5 13 9 10 12 19
# 15 18 4 3 2 16 14 11 8 17 7 1 6 5 13 9 10 12 19