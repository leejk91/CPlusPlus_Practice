int solution(int N) {
    // write your code in C++14 (g++ 6.2.0)
    int binary[20] = { 0, };
    int temp = N;
    int position = 0;
    int start = 0;
    int gap = 0;

    while (1)
    {
        binary[position] = temp % 2;
        temp = temp / 2;
        position++;

        if (temp == 0)
            break;
    }

    for (int i = position - 1; i >= 0; i--)
    {
        if (start == 0)
        {
            if (binary[i] == 1)
            {
                start++;
            }
        }
        else if (start == 1)
        {
            if (binary[i] == 0)
            {
                gap++;
            }
            else if (binary[i] == 1)
            {
                start++;
            }
        }
    }

    if (start == 1)
        gap = 0;

    return gap;

}