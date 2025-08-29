//
// Created by Thanapat Chotipun on 21/8/2025 AD.
//

#include <iostream>
#include <cmath>
#include <iomanip>

double round(const double x)
{
    const double value = static_cast<int>(x * 100 + .5);
    return value / 100;
}

int main()
{
    int n;
    std::cin >> n;
    std::pair<double, double> first;

    std::cin >> first.first >> first.second;
    std::pair<double, double> prev = first;
    std::pair<double, double> curr = first;
    double acc = 0;
    n--;
    while (n--)
    {
        prev = curr;
        std::cin >> curr.first >> curr.second;
        acc += round(curr.first * prev.second) - round(curr.second * prev.first);
    }
    acc += round(first.first * prev.second) - round(first.second * prev.first);
    std::cout << std::fixed << std::setprecision(2) << std::abs(acc / 2) << std::endl;
}
