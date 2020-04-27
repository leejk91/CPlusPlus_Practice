#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

//백준 1427 소트 인사이드


int main(void) 
{

	char number[100];

	cin >> number;

	sort(number, number + strlen(number)); // 정렬

	for (int i = strlen(number)-1; i >=0 ; i--) // 내림차순으로 출력
	{
		cout << number[i];
	}

}