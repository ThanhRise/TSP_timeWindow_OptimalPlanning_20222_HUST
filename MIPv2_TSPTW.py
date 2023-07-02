# MIXED-INTEGER PROGRAMMING MODEL FOR THE TIME WINDOWED TRAVELING SALESMAN PROBLEM

from ortools.linear_solver import pywraplp

def TSP_mixed_integer_programming(n, time_matrix, dist_matrix):
    model = pywraplp.Solver.CreateSolver('SCIP')

    num_nodes = n + 1
    e = {}  # earliest time to visit city i
    l = {}  # latest time to visit city i
    d = {}  # duration time to visit city i
    x = {}  # x[i,j] = 1 if i -> j else 0
    C = {}  # C[i,j] = d[i] + dist_matrix[i][j]
    M = {}  # M[i] = time to visit city i
    d[0] = 0
    e[0] = 0
    l[0] = 100000
    for i in range(1, num_nodes):
        e[i] = time_matrix[i-1][0]
        l[i] = time_matrix[i-1][1]
        d[i] = time_matrix[i-1][2]

    for i in range(num_nodes):
        for j in range(num_nodes):
            x[i, j] = model.IntVar(0, 1, 'x[%i,%i]' % (i, j))
            C[i, j] = d[i] + dist_matrix[i][j]
    M[0] = model.IntVar(0, 0, 'M[%i]' % 0)
    for i in range(1, num_nodes):
        M[i] = model.IntVar(e[i], l[i], 'M[%i]' % i)

    # u[i] = j if city i is visited at position j
    u = {}
    for i in range(num_nodes):
        u[i] = model.IntVar(0, num_nodes - 1, 'u[%i]' % i)
 
    # time waiting at city i
    w = {}
    for i in range(num_nodes):
        w[i] = model.IntVar(0, l[i] - e[i], 'w[%i]' % i)

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
                model.Add(w[j] - M[j] + M[i] >= -C[i, j] + (x[i,j] - 1) * 100000)
                
                

    # define objective function
    # sum(C[i, j] * x[i, j] for i in range(num_nodes) for j in range(num_nodes) if j != i) + sum(w[i] for i in range(num_nodes))
    model.Minimize(sum(C[i, j] * x[i, j] for i in range(num_nodes) for j in range(num_nodes) if j != i) + sum(w[i] for i in range(num_nodes)))


    status = model.Solve()
    if status == pywraplp.Solver.OPTIMAL:
        print('Objective value =', model.Objective().Value())
        for i in range(num_nodes):
            for j in range(num_nodes):
                if x[i, j].solution_value() > 0:
                    print('x[%i,%i] = %i' % (i, j, x[i, j].solution_value()))
        for i in range(num_nodes):
            print('M[%i] = %i' % (i, M[i].solution_value()))
        for i in range(num_nodes):
            print('w[%i] = %i' % (i, w[i].solution_value()))
        for i in range(num_nodes):
            print('u[%i] = %i' % (i, u[i].solution_value()))

    return model.Objective().Value()

if __name__ == '__main__':

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
    print(TSP_mixed_integer_programming(N, customers, t))
    
    


