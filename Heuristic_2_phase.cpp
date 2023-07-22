// Đường dẫn vấn đề: Nhom12_TULKH.txt
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <queue>
#include <ctime>
#include <chrono>
#include <string>

using namespace std;

int N;
vector<int> e, l, d, initPath;
vector<vector<int>> t;
bool visited[1001];
vector<vector<int>> C; // C[i][j] = d[i] + t[i][j]
int BEST = 1000000000; // tổng thời gian đi từ 0 đến N và quay lại 0
vector<int> path;      // đường đi từ 0 đến N và quay lại 0

void input()
{
    cin >> N;
    e.resize(N + 1);
    l.resize(N + 1);
    d.resize(N + 1);
    t.resize(N + 1);
    C.resize(N + 1);
    initPath.resize(N + 1);
    for (int i = 0; i <= N; i++)
    {
        t[i].resize(N + 1);
        C[i].resize(N + 1);
    }
    for (int i = 1; i <= N; i++)
    {
        cin >> e[i] >> l[i] >> d[i];
    }
    for (int i = 0; i <= N; i++)
    {
        for (int j = 0; j <= N; j++)
        {
            cin >> t[i][j];
            C[i][j] = d[i] + t[i][j];
        }
    }
    for (int i = 0; i <= N; i++)
    {
        visited[i] = false;
    }
}

// input from file
void inputFromFile(string fileName)
{
    ifstream file;
    file.open(fileName);
    file >> N;
    e.resize(N + 1);
    l.resize(N + 1);
    d.resize(N + 1);
    t.resize(N + 1);
    C.resize(N + 1);
    initPath.resize(N + 1);
    for (int i = 0; i <= N; i++)
    {
        t[i].resize(N + 1);
        C[i].resize(N + 1);
    }
    for (int i = 1; i <= N; i++)
    {
        file >> e[i] >> l[i] >> d[i];
    }
    for (int i = 0; i <= N; i++)
    {
        for (int j = 0; j <= N; j++)
        {
            file >> t[i][j];
            C[i][j] = d[i] + t[i][j];
        }
    }
    for (int i = 0; i <= N; i++)
    {
        visited[i] = false;
    }
    file.close();
}

// random path
std::vector<int> randomPermutation(int N)
{
    std::vector<int> permutation(N + 1);
    permutation[0] = 0;
    for (int i = 1; i <= N; i++)
    {
        permutation[i] = i;
    }
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(permutation.begin() + 1, permutation.end(), generator);
    return permutation;
}

std::vector<int> initPathGreedyTimeClose()
{
    // copy d to closeTime
    vector<int> closeTime(l);
    // sort
    sort(closeTime.begin(), closeTime.end());
    vector<int> iterSort(N + 1);
    iterSort[0] = 0;
    vector<bool> visited(N + 1);
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            if (closeTime[i] == l[j] && visited[j] == false)
            {
                iterSort[i] = j;
                visited[j] = true;
                break;
            }
        }
    }
    return iterSort;
}

// calculate M: time to go from 0 to i
std::vector<int> M_calculate(vector<int> route)
{
    vector<int> M(route);
    M[0] = 0;
    for (int i = 1; i <= N; i++)
    {
        M[route[i]] = max(e[route[i]], M[route[i - 1]] + C[route[i - 1]][route[i]]);
    }
    return M;
}

// heuristic_phase1: Sum: Max(0, M[i] - l[i]) for i = 1..N
int heuristic_phase1(vector<int> route, vector<int> timeVisit)
{
    // calculate heuristic_phase1
    int sum = 0;
    for (int i = 1; i <= N; i++)
    {
        sum += max(0, timeVisit[i] - l[i]);
    }
    return sum;
}

// heuristic_phase2: M[lastCity] + C[lastCity][0]
int heuristic_phase2(vector<int> route, vector<int> timeVisit)
{
    // calculate heuristic_phase2
    return timeVisit[route[N]] + C[route[N]][0];
}

// fesible: check if route is feasible
bool feasible(vector<int> route, vector<int> timeVisit)
{
    // check if feasible
    for (int i = 1; i <= N; i++)
    {
        if (timeVisit[i] > l[i])
        {
            return false;
        }
    }
    return true;
}

// oneShiftChange
vector<int> oneShiftChange(int i, int j, vector<int> route)
{
    vector<int> newRoute(route);
    int temp = newRoute[i];
    newRoute[i] = newRoute[j];
    newRoute[j] = temp;
    return newRoute;
}
// one opt change
std::vector<int> oneOptChange(int i, int j, vector<int> route)
{
    vector<int> newRoute(route);
    int temp = newRoute[i];
    if (i < j)
    {
        for (int k = i; k < j; k++)
        {
            newRoute[k] = newRoute[k + 1];
        }
        newRoute[j] = temp;
    }
    else
    {
        for (int k = i; k > j; k--)
        {
            newRoute[k] = newRoute[k - 1];
        }
        newRoute[j] = temp;
    }
    return newRoute;
}

// two opt change
std::vector<int> twoOptChange(int i, int j, vector<int> route)
{
    vector<int> newRoute(route);
    int temp;
    if (i < j)
    {
        for (int k = i; k <= (i + j) / 2; k++)
        {
            temp = newRoute[k];
            newRoute[k] = newRoute[j - k + i];
            newRoute[j - k + i] = temp;
        }
    }
    else
    {
        for (int k = j; k <= (i + j) / 2; k++)
        {
            temp = newRoute[k];
            newRoute[k] = newRoute[i - k + j];
            newRoute[i - k + j] = temp;
        }
    }
    return newRoute;
}

// perturbation
std::vector<int> perturbation(vector<int> route, int level)
{
    // random is different with each call
    vector<int> newRoute(route);
    int i, j;
    std::random_device rd;
    std::mt19937 gen(rd());
    int lowerBound = 1;
    int upperBound = N;
    int lowerBound2 = -int(sqrt(N));
    int upperBound2 = int(sqrt(N));

    std::uniform_int_distribution<int> dist(lowerBound, upperBound);
    std::uniform_int_distribution<int> dist2(lowerBound2, upperBound2);
    for (int k = 0; k < int(sqrt(level)); k++)
    {
        i = dist(gen);
        j = dist2(gen);
        while (j == 0)
        {
            j = dist2(gen);
        }
        if (i + j > N)
        {
            j = N - i;
        }
        if (i + j < 1)
        {
            j = 1 - i;
        }
        j = i + j;
        newRoute = oneShiftChange(i, j, newRoute);
    }
    return newRoute;
}

// Constructive phase
std::vector<int> constructivePhase(int Maxlevel)
{
    int level = 1;
    initPath = initPathGreedyTimeClose();
    // initPath = randomPermutation(N);
    vector<int> route(initPath);
    vector<int> timeVisit = M_calculate(route);
    vector<int> bestRoute(route);
    int h = heuristic_phase1(route, timeVisit);
    while (!feasible(route, timeVisit) && level < Maxlevel)
    {
        // perturbation
        route = perturbation(route, level);
        timeVisit = M_calculate(route);
        h = heuristic_phase1(route, timeVisit);
        // local search
        for (int i = 1; i <= N; i++)
        {
            for (int j = max(1, int(i -sqrt(N))); j <= min(N,int(i + sqrt(N))); j++)
            {
                vector<int> newRoute = oneShiftChange(i, j, route);
                // vector<int> newRoute = oneOptChange(i, j, route);
                vector<int> newTimeVisit = M_calculate(newRoute);
                int newH = heuristic_phase1(newRoute, newTimeVisit);
                if (newH < h)
                {
                    route = newRoute;
                    timeVisit = newTimeVisit;
                    h = newH;
                    break;
                }
            }
        }
        if (h < heuristic_phase1(bestRoute, M_calculate(bestRoute)))
        {
            bestRoute = route;
            level = 1;
        }
        else
        {
            level++;
        }
    }
    return route;
}

// VND
std::vector<int> VND(vector<int> route, int level)
{
    // using oneOptChange and twoOptChange and heuristic_phase2
    vector<int> BestRoute(route);
    int bestH = heuristic_phase2(BestRoute, M_calculate(BestRoute));
    int h = bestH;

    route = perturbation(route, level);
    for (int i = 1; i <= N; i++)
    {
        for (int j = max(1, int(i -sqrt(N))); j <= min(N,int(i + sqrt(N))); j++)
        {
            if (i == j)
            {
                continue;
            }
            vector<int> newRoute = oneOptChange(i, j, route);
            vector<int> newTimeVisit = M_calculate(newRoute);
            int newH = heuristic_phase2(newRoute, newTimeVisit);
            if (newH < bestH)
            {
                if (feasible(newRoute, newTimeVisit))
                {
                    route = newRoute;
                    bestH = newH;
                }
            }
        }
    }

    if (bestH < h)
    {
        BestRoute = route;
    }
    // else
    // {
    //     route = perturbation(route, level);
    //     for (int i = 1; i <= N - 1; i++)
    //     {
    //         for (int j = max(1, int(i -sqrt(N))); j <= min(N,int(i + sqrt(N))); j++)
    //         {
    //             vector<int> newRoute = twoOptChange(i, j, route);
    //             vector<int> newTimeVisit = M_calculate(newRoute);
    //             int newH = heuristic_phase2(newRoute, newTimeVisit);
    //             if (newH < bestH)
    //             {
    //                 if (feasible(newRoute, newTimeVisit))
    //                 {
    //                     route = newRoute;
    //                     bestH = newH;
    //                 }
    //             }
    //         }
    //     }
    // }
    // if (bestH < h)
    // {
    //     BestRoute = route;
    // }

    return BestRoute;
}

// optimization phase with GVNS i.e. VNS with VND as local search
std::vector<int> GVNS_OptimizationPhase(vector<int> route, int Maxlevel, int maxTime)
{
    int level = 1;
    vector<int> bestRoute(route);
    int bestH = heuristic_phase2(bestRoute, M_calculate(bestRoute));
    while (level < Maxlevel && clock() < maxTime * CLOCKS_PER_SEC)
    {
        // local search
        route = VND(bestRoute, level);
        if (heuristic_phase2(route, M_calculate(route)) < bestH)
        {
            bestRoute = route;
            bestH = heuristic_phase2(bestRoute, M_calculate(bestRoute));
            level = 1;
        }
        else
        {
            level++;
        }
    }
    return bestRoute;
}

// version 2
std::vector<int> v2OptimizationPhase(vector<int> route, int Maxlevel)
{
    int level = 1;
    vector<int> bestRoute(route);
    int bestH = heuristic_phase2(bestRoute, M_calculate(bestRoute));
    while (level < Maxlevel)
    {
        // local search
        route = constructivePhase(Maxlevel);
        if (heuristic_phase2(route, M_calculate(route)) < bestH)
        {
            bestRoute = route;
            bestH = heuristic_phase2(bestRoute, M_calculate(bestRoute));
            level = 1;
        }
        else
        {
            level++;
        }
    }
    return bestRoute;
}

int main()
{
    // input();
    inputFromFile("input.txt");

    int Maxlevel = N * N * N;
    int maxTime = 10;
    auto start = chrono::high_resolution_clock::now();
    vector<int> route = constructivePhase(Maxlevel);
    cout << "constructive phase" << endl;
    for (int i = 1; i <= N; i++)
    {
        cout << route[i] << " ";
    }
    cout << endl;
    // cout << heuristic_phase1(route, M_calculate(route)) << endl;
    cout << heuristic_phase2(route, M_calculate(route)) << endl;

    route = GVNS_OptimizationPhase(route, Maxlevel, maxTime);
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    // route = v2OptimizationPhase(route, Maxlevel);
    cout << "after optimization" << endl;
    for (int i = 1; i <= N; i++)
    {
        cout << route[i] << " ";
    }
    cout << endl;
    // cout << heuristic_phase1(route, M_calculate(route)) << endl;
    cout << heuristic_phase2(route, M_calculate(route)) << endl;
    cout << "Time run: " << duration.count() << " microseconds" << endl;

    //compare with output

    string fileName[] = {"N5.txt", "N10.txt", "N100.txt", "N200.txt", "N300.txt", "N500.txt", "N600.txt", "N700.txt", "N900.txt", "N1000.txt"};
    string fileNameOut[] = {"N5.txt", "N10.txt", "N100.txt", "N200.txt", "N300.txt", "N500.txt", "N600.txt", "N700.txt", "N900.txt", "N1000.txt"};
    ofstream file;
    file.open("testcase/timeRun/time.txt");
    for (int i = 1; i <= 10; i++)
    {
        string fullFileName = "testcase/input/" + fileName[i - 1];
        inputFromFile(fullFileName);
        int Maxlevel = N * N;
        auto start = chrono::high_resolution_clock::now();
        vector<int> route = constructivePhase(Maxlevel);
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

        // output has answer of testcase
        string fullFileNameOut = "testcase/output/" + fileNameOut[i - 1];
        ifstream fileOut;
        fileOut.open(fullFileNameOut);
        int NOut;
        fileOut >> NOut;    
        vector<int> routeOut(NOut + 1);
        for (int i = 1; i <= NOut; i++)
        {
            fileOut >> routeOut[i];
        }
        fileOut.close();
        int costOut = heuristic_phase2(routeOut, M_calculate(routeOut));
        int costMy = heuristic_phase2(route, M_calculate(route));
        if (costOut == costMy)
        {
            cout << "Testcase " << i << " is correct" << endl;
        }
        else
        {
            cout << "Testcase " << i << " is incorrect" << ": " << costOut << " " << costMy << endl;
        }
        file << "Time run testcase " << i << ": " << duration.count() << " microseconds" << endl;
    }
    file.close();
    return 0;

}
