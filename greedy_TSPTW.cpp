// Đường dẫn vấn đề: Nhom12_TULKH.txt
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <queue>

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
    input();
    vector<int> route = greedy();
    vector<int> TimeVisit = M_calculate(route);
    for (int i = 1; i <= N; i++)
    {
        cout << route[i] << " ";
    }
    cout << endl;
    cout << cost(route, TimeVisit) << endl;
}
