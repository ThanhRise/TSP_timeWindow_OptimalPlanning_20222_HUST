// Đường dẫn vấn đề: Nhom12_TULKH.txt
#include <iostream>
using namespace std;

int N, e[1001], l[1001], d[1001], t[1001][1001], s[1001];
bool visited[1001];

// int X[1001][1001]; // X[i][j] = 1 nếu có đường đi từ i đến j và 0 nếu không có
int C[1001][1001];     // C[i][j] = d[i] + t[i][j]
int M[1001];           // tổng thời gian đi tử 0 đến i (đến thôi chứ chưa dỡ hàng)
int BEST = 1000000000; // tổng thời gian đi từ 0 đến N và quay lại 0
int path[1001];        // đường đi từ 0 đến N và quay lại 0
int Cmin = 1000000000;

// ràng buộc M
// M(u) = M(u) + C[i][u] nếu X(u,i) = 1 và e(u) < M(u) < l(u)
// M(u) = e(u) + C[i][u] nếu X(u,i) = 1 và M(u) < e(u)

void input()
{
    // •Input
    // •Line 1: contains a positive integer 𝑁(1≤𝑁≤1000)
    // •Line 𝑖+1(𝑖=1,...,𝑁): contains 𝑒(𝑖),𝑙(𝑖)and 𝑑(𝑖)
    // •Line 𝑖+𝑁+2(𝑖=0,1,...,𝑁): contains the ith row of the matrix 𝑡
    // •Output
    // •Line 1: contains 𝑁
    // •Line 2: contains 𝑠[1],𝑠[2],...,𝑠[𝑁]
    cin >> N;
    for (int i = 1; i <= N; i++)
    {
        cin >> e[i] >> l[i] >> d[i];
    }
    for (int i = 0; i <= N; i++)
    {
        for (int j = 0; j <= N; j++)
        {
            cin >> t[i][j];
        }
    }
    d[0] = 0;
    for (int i = 0; i <= N; i++)
    {
        for (int j = 0; j <= N; j++)
        {
            C[i][j] = d[i] + t[i][j];
        if (i != j){
            Cmin = min(Cmin, C[i][j]);
        }
        }
    }
}

void solution()
{
    // cout << "Solution: ";
    int temp_total = 0;
    temp_total = M[s[N]] + C[s[N]][0];
    if (temp_total < BEST)
    {
        BEST = temp_total;
        for (int i = 0; i <= N; i++)
        {
            path[i] = s[i];
        }
    }
}

int TRY(int k, int prev)
{
    for (int i = 1; i <= N; i++)
    {
        if (!visited[i] && M[prev] + C[prev][i] <= l[i])
        {
            visited[i] = true;
            s[k] = i;
            int M_prev = M[i];
            if (M[prev] + C[prev][i] < e[i])
            {
                M[i] = e[i];
            }
            else
            {
                M[i] = M[prev] + C[prev][i];
            }
            if (M[i] > BEST - Cmin*(N-k+1))
            {
                visited[i] = false;
                M[i] = M_prev;
                continue;
            }
            if (k == N)
            {
                solution();
            }
            else
            {
                TRY(k + 1, i);
            }
            visited[i] = false;
            M[i] = M_prev;
        }
    }
    return 1000000000;
}

// decoding code for function TRY
// for i = 1 to N
//     if not visited[i] and M[prev] + C[prev][i] <= l[i]
//         visited[i] = true
//         s[k] = i
//         M_prev = M[i]
//         if M[prev] + C[prev][i] < e[i]
//             M[i] = e[i]
//         else
//             M[i] = M[prev] + C[prev][i]
//         if M[i] > BEST - Cmin*(N-k+1)
//             visited[i] = false
//             M[i] = M_prev
//             continue
//         if k == N
//             solution()
//         else
//             TRY(k + 1, i)
//         visited[i] = false
//         M[i] = M_prev


int main()
{
    input();
    s[0] = 0;
    M[0] = 0;
    TRY(1, 0);
    cout << N << endl;
    for (int i = 1; i <= N; i++)
    {
        cout << path[i] << " ";
    }
    // cout << endl;
    // cout << BEST;
}
