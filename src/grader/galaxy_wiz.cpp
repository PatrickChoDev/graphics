//
// Created by Thanapat Chotipun on 15/8/2025 AD.
//


#include <iostream>

#include <cmath>
#include <iomanip>

void move(std::pair<double, double>* p, const double x, const double y)
{
    p->first += x;
    p->second += y;
}

void rotate(std::pair<double, double>* p, const double a)
{
    const double x = p->first;
    const double y = p->second;
    p->first = x * cos(a) - y * sin(a);
    p->second = x * sin(a) + y * cos(a);
}

void scale(std::pair<double, double>* p, const double sx, const double sy)
{
    p->first *= sx;
    p->second *= sy;
}

int main()
{
    int n, q;
    std::cin >> n >> q;

    std::pair<double, double> p[n];

    for (int i = 0; i < n; i++)
    {
        std::cin >> p[i].first >> p[i].second;
    }

    for (int i = 0; i < q; i++)
    {
        int cmd;
        std::cin >> cmd;

        switch (cmd)
        {
        case 1:
            double x, y;
            std::cin >> x >> y;
            for (int j = 0; j < n; j++) move(&p[j], x, y);
            break;
        case 2:
            double a;
            std::cin >> a;
            for (int j = 0; j < n; j++) rotate(&p[j], a);
            break;
        case 3:
            double sx, sy;
            std::cin >> sx >> sy;
            for (int j = 0; j < n; j++) scale(&p[j], sx, sy);
        default: ;
        }
    }
    std::cout.precision(1);
    std::cout << std::fixed;
    for (int i = 0; i < n; i++)
        std::cout << std::round(p[i].first * 10.f) / 10.f << " " << std::round(p[i].second * 10.f) / 10.f << std::endl;
}
