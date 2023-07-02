from ortools.constraint_solver import routing_enums_pb2
from ortools.constraint_solver import pywrapcp


def create_data_model():

    data = {}
    with open('input.txt', 'r') as file:
        # Read the number of cities
        data['num_cities'] = int(file.readline())

        # Read the time windows and duration for each city
        data['time_windows'] = []
        for _ in range(data['num_cities']):
            e, l, d = map(int, file.readline().split())
            data['time_windows'].append((e, l, d))

        # Read the distance matrix
        data['distance_matrix'] = []
        for _ in range(data['num_cities']):
            row = list(map(int, file.readline().split()))
            data['distance_matrix'].append(row)

    return data


def print_solution(manager, routing, solution):

    print('Number of cities:', manager.GetNumberOfNodes())
    index = routing.Start(0)
    plan_output = 'Route:\n'
    while not routing.IsEnd(index):
        node_index = manager.IndexToNode(index)
        plan_output += '{} -> '.format(node_index)
        index = solution.Value(routing.NextVar(index))
    node_index = manager.IndexToNode(index)
    plan_output += '{}\n'.format(node_index)
    print(plan_output)


def main():

    # Instantiate the data problem.
    data = create_data_model()

    # Create the routing index manager.
    manager = pywrapcp.RoutingIndexManager(data['num_cities'], 1, 0)

    # Create Routing Model.
    routing = pywrapcp.RoutingModel(manager)

    def time_callback(from_index, to_index):
        """Returns the travel time between the two nodes."""
        from_node = manager.IndexToNode(from_index)
        to_node = manager.IndexToNode(to_index)
        return data['distance_matrix'][from_node][to_node]

    transit_callback_index = routing.RegisterTransitCallback(time_callback)

    # Define cost of each arc.
    routing.SetArcCostEvaluatorOfAllVehicles(transit_callback_index)

    # Add time window constraints.
    time = 'Time'
    routing.AddDimension(
        transit_callback_index,
        10000,  # Maximum waiting time at each node (in minutes)
        10000,  # Maximum time between visits (in minutes)
        False,  # Don't force start cumul to zero
        time)
    time_dimension = routing.GetDimensionOrDie(time)

    for city in range(1, data['num_cities']):
        e, l, d = data['time_windows'][city]
        time_dimension.CumulVar(manager.NodeToIndex(city)).SetRange(e, l)

    # Set first solution strategy.
    search_parameters = pywrapcp.DefaultRoutingSearchParameters()
    search_parameters.first_solution_strategy = (
        routing_enums_pb2.FirstSolutionStrategy.PATH_CHEAPEST_ARC)

    # Solve the problem.
    solution = routing.SolveWithParameters(search_parameters)

    # Print solution on console.
    if solution:
        print_solution(manager, routing, solution)


if __name__ == '__main__':
    main()
