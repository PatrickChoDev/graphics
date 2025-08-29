//
// Created by Thanapat Chotipun on 22/8/2025 AD.
//

#include <iostream>

int gcd(const int a, const int b) {
    int res = std::min(a, b);
    while (res > 1) {
        if (a % res == 0 && b % res == 0)
            break;
        res--;
    }
    return res;
}

int main ()
{
    int n;
    std::cin >> n;
    std::pair<int, int> acc = {0, 0};
    for (int i = 0; i < n; i++)
    {
        int x, y;
        std::cin >> x >> y;
        acc.first += x;
        acc.second += y;
    }

    const int g_first = gcd(acc.first, n);
    const int g_second = gcd(acc.second, n);
    std::cout << acc.first / g_first;
    if (g_first != 1) std::cout << "/" << n / g_first;

    std::cout << " ";

    std::cout << acc.second / g_second;
    if (g_second != 1) std::cout << "/" << n / g_second << std::endl;


}