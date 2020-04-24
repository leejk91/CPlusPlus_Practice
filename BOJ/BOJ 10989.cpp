#include <iostream>
using namespace std;

//백준 10989 : 수 정렬하기 3

// 카운팅 정렬

void setting() // cin과 cout 실행속도 
{
    ios::sync_with_stdio(0); // scanf/ printf랑 cin/cout 싱크를 맞추지 않게
    cin.tie(NULL);
    cout.tie(NULL);
}

int main() 
{

    setting();
  
    int N;
    cin >> N; // 수의 갯수 입력

    int C[10010] = { 0, }; // 10000까지 갯수를 카운팅할 배열

    for (int i = 0; i < N; i++)
    {
        int a;
        cin >> a;
        C[a] += 1;
    }

    // 입력받은 수를 전부 다 입력 받아서 저장하게 되면 제한된 8MB의 메모리를 초과해버린다

    for (int i = 1; i <= 10000; i++)
    {
        for (int j = 0; j < C[i]; j++) // 배열의 갯수만큼 출력한다.
        {
            cout << i << "\n";
        }
    }

}