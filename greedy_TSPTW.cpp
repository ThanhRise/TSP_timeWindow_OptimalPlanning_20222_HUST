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
#include <sstream>

using namespace std;

int N;
vector<int> e, l, d, initPath;
vector<vector<int>> t;
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
}

void inputFromFile(string fileName){
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
    file.close();
}

std::vector<int> GreedyTimeClose()
{
    vector<int> closeTime(l);
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

std::vector<int> M_calculate(vector<int> route)
{
    vector<int> M(route);
    M[0] = 0;
    for (int i = 1; i <= N; i++)
    {
        M[route[i]] = max(e[route[i]] ,M[route[i - 1]] + C[route[i - 1]][route[i]]);
    }
    return M;
}

int cost(vector<int> route, vector<int> TimeVisit)
{
    int sum = 0;
    for (int i = 1; i <= N; i++)
    {
        sum += C[route[i - 1]][route[i]];
    }
    sum += C[route[N]][0];
    return sum;
}

std::vector<int> oneOptChange(int i, int j, vector<int> route)
{
    vector<int> newRoute(route);
    int temp = newRoute[i];
        for (int k = i; k > j; k--)
        {
            newRoute[k] = newRoute[k - 1];
        }
        newRoute[j] = temp;
    return newRoute;
}

std::vector<int> greedy()
{   // 0 4 15 18
    vector<int> iterSort = GreedyTimeClose();
    vector<int> orderTime(N + 1);
    vector<int> newRoute(N + 1);
    vector<bool> visited(N + 1);
    vector<int> timeVisit(N + 1);

    newRoute[0] = 0;
    visited[0] = true;
    timeVisit[0] = 0;  
    orderTime[0] = 0;
    for(int i = 1; i <= N; i++)
    {
        orderTime[iterSort[i]] = i;
    } 

    for(int i = 1; i <= N; i++)
    {
        int nextCity = iterSort[i];
        int minCostOneStep = l[nextCity];
        int selectCity = nextCity;
        int selectTime = max(e[nextCity], timeVisit[iterSort[i -1]] +  C[iterSort[i - 1]][nextCity]);
        for(int j = 1; j <= N; j++)
        {
            if(visited[j] == false && j != nextCity)
            {
               int timeCome = max(e[j], timeVisit[iterSort[i -1]] +  C[iterSort[i - 1]][j]);
               if (timeCome < minCostOneStep - C[j][nextCity])
               {
                    minCostOneStep = timeCome + C[j][nextCity];
                    selectCity = j;
                    selectTime = timeCome;
               }
            }
        }
        if (selectCity != nextCity)
        {
            int jChange = orderTime[selectCity];
            iterSort = oneOptChange(jChange, i, iterSort);
            for(int i = 1; i <= N; i++)
            {
                orderTime[iterSort[i]] = i;
            }
        }
        newRoute[i] = selectCity;
        visited[newRoute[i]] = true;
        timeVisit[newRoute[i]] = selectTime;
    }

    return newRoute;
}



int main()
{
    // input();
    inputFromFile("input.txt");
    vector<int> route = greedy();
    vector<int> TimeVisit = M_calculate(route);
    cout << N << endl;
    for (int i = 1; i <= N; i++)
    {
        cout << route[i] << " ";
    }

    // compare output with my answer

    // string fileName[] = {"N5.txt", "N10.txt", "N100.txt", "N200.txt", "N300.txt", "N500.txt", "N600.txt", "N700.txt", "N900.txt", "N1000.txt"};
    // string fileNameOut[] = {"N5.txt", "N10.txt", "N100.txt", "N200.txt", "N300.txt", "N500.txt", "N600.txt", "N700.txt", "N900.txt", "N1000.txt"};
    // ofstream file;
    // file.open("testcase/timeRun/time.txt");
    // for (int i = 1; i <= 10; i++)
    // {
    //     string fullFileName = "testcase/input/" + fileName[i - 1];
    //     inputFromFile(fullFileName);
    //     auto start = chrono::high_resolution_clock::now();
    //     vector<int> route = greedy();
    //     auto stop = chrono::high_resolution_clock::now();
    //     auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    //     // output has answer of testcase
    //     // i want to compare output with my answer
    //     string fullFileNameOut = "testcase/output/" + fileNameOut[i - 1];
    //     ifstream fileOut;
    //     fileOut.open(fullFileNameOut);
    //     int NOut;
    //     fileOut >> NOut;
    //     vector<int> routeOut(NOut + 1);
    //     for (int i = 1; i <= NOut; i++)
    //     {
    //         fileOut >> routeOut[i];
    //     }
    //     fileOut.close();
    //     int costOut = cost(routeOut, M_calculate(routeOut));
    //     int costMy = cost(route, M_calculate(route));
    //     if (costOut == costMy)
    //     {
    //         cout << "Testcase " << i << " is correct" << endl;
    //     }
    //     else
    //     {
    //         cout << "Testcase " << i << " is incorrect" << ": " << costOut << " " << costMy << endl;
    //     }
    //     file << "Time run testcase " << i << ": " << duration.count() << " microseconds" << endl;
    // }
    // file.close();
    // return 0;



}

