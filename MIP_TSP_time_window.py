# MIXED-INTEGER PROGRAMMING MODEL FOR THE TIME WINDOWED TRAVELING SALESMAN PROBLEM

from ortools.linear_solver import pywraplp
import time

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
    l[0] = 100000000
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


    # time waiting at city i
    w = {}
    for i in range(num_nodes):
        w[i] = model.IntVar(0, model.infinity(), 'w[%i]' % i)

    # define constraints
    # each city is visited exactly once
    for i in range(num_nodes):
        constraint = model.Constraint(1, 1)
        for j in range(num_nodes):
            if i != j:
                constraint.SetCoefficient(x[i, j], 1)

    # each city is left exactly once
    for i in range(num_nodes):
        constraint = model.Constraint(1, 1)
        for j in range(num_nodes):
            if i != j:
                constraint.SetCoefficient(x[j, i], 1)

    # for i in range(num_nodes):
    #     constraint = model.Constraint(0, 0)
    #     for j in range(num_nodes):
    #         if i == j:
    #             constraint.SetCoefficient(x[i, j], 1)
    
    # time window constraints
    for i in range(num_nodes):
        for j in range(1, num_nodes):
            if i != j:
                constraint = model.Constraint(-model.infinity(), 100000000)
                constraint.SetCoefficient(M[i], 1)
                constraint.SetCoefficient(x[i, j], C[i, j] + 100000000)
                constraint.SetCoefficient(M[j], -1)

    for i in range(num_nodes):
        for j in range(1, num_nodes):
            if i != j:
                constraint = model.Constraint(- 100000000 - C[i,j], model.infinity())
                constraint.SetCoefficient(w[j], 1)
                constraint.SetCoefficient(M[j], -1)
                constraint.SetCoefficient(M[i], 1)
                constraint.SetCoefficient(x[i, j], -100000000)   

    # define objective function
    objective = model.Objective()
    for i in range(num_nodes):
        for j in range(num_nodes):
            if i != j:
                objective.SetCoefficient(x[i, j], C[i, j])
    for i in range(num_nodes):
        objective.SetCoefficient(w[i], 1)
    objective.SetMinimization()

    status = model.Solve()
    if status == pywraplp.Solver.OPTIMAL:
        print(num_nodes- 1)
        solution = {}
        solution[0] = 0
        for i in range(1, num_nodes):
            solution[i] = M[i].solution_value()
        solution = sorted(solution.items(), key=lambda x: x[1])
        for i in range(1,num_nodes):
            print(solution[i][0], end=' ')
    else:
        print('The problem does not have an optimal solution.')

    return model.Objective().Value()

def compare_ans_and_compute_time():
    base_path_input = 'testcase/input'
    input_file = ["N5.txt", "N10.txt", "N100.txt", "N200.txt", "N300.txt", "N500.txt", "N600.txt", "N700.txt", "N900.txt", "N1000.txt"]
    base_path_output = 'testcase/output'
    output_file = ["N5.txt", "N10.txt", "N100.txt", "N200.txt", "N300.txt", "N500.txt", "N600.txt", "N700.txt", "N900.txt", "N1000.txt"]
    write_time_to = 'testcase/timeRun/time.txt'
    with open(write_time_to, 'w') as f:
        for i in range(len(input_file)):
            customers = []
            t = []
            with open(base_path_input + '/' + input_file[i], 'r') as file:
                # Read the input
                N = int(file.readline())

                for k in range(1, N+1):
                    e, l, d = map(int, file.readline().split())
                    customers.append([e, l, d])

                for _ in range(N+1):
                    row = list(map(int, file.readline().split()))
                    t.append(row)
            start_time = time.time()
            ans = TSP_mixed_integer_programming(N, customers, t)
            end_time = time.time()
            f.write(str(end_time - start_time) + '\n')

            with open(base_path_output + '/' + output_file[i], 'r') as file:
                num_nodes = int(file.readline())
                list_nodes = list(map(int, file.readline().split()))
                time_visit = {}
                time_visit[0] = 0
                list_nodes = [0] + list_nodes
                d = [0] + [customers[i][2] for i in range(len(customers))]
                e = [0] + [customers[i][0] for i in range(len(customers))]
                for k in range(1, len(list_nodes)):
                    time_visit[list_nodes[k]] = max(time_visit[list_nodes[k-1]] + t[list_nodes[k-1]][list_nodes[k]] + d[list_nodes[k-1]], e[list_nodes[k]])
                myAns = time_visit[list_nodes[num_nodes]] + t[list_nodes[num_nodes]][0] + d[list_nodes[num_nodes]]

            if ans == myAns:
                print('\n Correct')
            else:
                print('\n Wrong', "myAns = ", myAns, "ans = ", ans)

if __name__ == '__main__':

# import from file
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
    TSP_mixed_integer_programming(N, customers, t)
    
# input from keyboard

    # N = int(input())
    # customers = []
    # t = []
    # for i in range(1, N+1):
    #     e, l, d = map(int, input().split())
    #     customers.append([e, l, d])

    # for _ in range(N+1):
    #     row = list(map(int, input().split()))
    #     t.append(row)
    # TSP_mixed_integer_programming(N, customers, t)
    
    # compare ans and compute time
    # compare_ans_and_compute_time()

