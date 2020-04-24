#include <iostream>
using namespace std;
//백준 2750 : 수 정렬하기

int N = 0;

void setting() // 입출력 속도 가속
{
    ios_base::sync_with_stdio(0);
    cin.tie(NULL);
    cout.tie(NULL);

}

void Sort(int* arr)
{
    int i, j;
    int temp;

    for (i = 1; i < N; i++) // 앞의 원소와 비교하기 위해서 두번째 배열부터 시작
    {
        temp = arr[i]; // i의 값을 비교하기 위해 임시 저장
        for (j = i - 1; j >= 0; j--) 
        {
            if (arr[j] > temp) // 앞의 값과 비교 한다.
            {
                arr[j + 1] = arr[j]; // 앞의 값이 크다면 이동
            }
            else
                break; // 작다면 루프에서 빠져나온다

        }
        arr[j + 1] = temp; // 앞의 값에 임시 값을 넣는다.
    }
}


int main() {

    setting();

    cin >> N; // 숫자 갯수 입력

    int* K = new int[N];

    for (int i = 0; i < N; i++)
    {
        cin >> K[i]; // 배열 입력
    }

    Sort(K); // 정렬 함수 시작

    for (int i = 0; i < N; i++)
    {
        cout << K[i] << "\n"; // 배열 출력
    }
}