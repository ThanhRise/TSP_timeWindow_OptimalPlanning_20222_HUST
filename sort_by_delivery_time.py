def delivery_route(N, customers, t):
    # Sort the customers based on their earliest delivery time
    customers.sort(key=lambda x: x[0])

    # Initialize the current time and the delivery route
    current_time = 0
    route = []

    # Deliver to each customer in the sorted list
    for i in range(N):
        # Calculate the travel time to the next customer
        travel_time = t[0][customers[i][1]-1] if i == 0 else t[route[-1]][customers[i][1]-1]

        # Update the current time and add the customer to the route
        current_time += travel_time + customers[i][2]
        route.append(customers[i][1])

    return route

def main():
    # Open the input file
    with open('input.txt', 'r') as file:
        # Read the input
        N = int(file.readline())
        customers = []
        for i in range(1, N+1):
            e, l, d = map(int, file.readline().split())
            customers.append((e, i, d))
        t = []
        for _ in range(N+2):
            row = list(map(int, file.readline().split()))
            t.append(row)

        # Call the function with the input
        route = delivery_route(N, customers, t)

        # Print the output
        print(N)
        print(' '.join(map(str, route)))

if __name__ == "__main__":
    main()