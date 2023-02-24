// Markus Buchholz. 2023
//g++ hoecken_link.cpp -o t -I/usr/include/python3.8 -lpython3.8

#include <iostream>
#include <tuple>
#include <vector>
#include <math.h>
#include <cmath>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

//----------- system dynamic parameters --------------------

float r = 1.0;

//---------------------------------------------------------------

void plot2D(std::vector<float> X, std::vector<float> Y)
{

    plt::title("Hoecken linkage");
    plt::named_plot("Hoecken linkage", X, Y);
    plt::xlabel("pos X");
    plt::ylabel("pos Y");
    plt::legend();
    plt::xlabel("pos X");
    plt::ylabel("pos Y");
    plt::show();
}

//---------------------------------------------------------------

std::tuple<std::vector<float>, std::vector<float>> simulation()
{

    std::vector<float> x;
    std::vector<float> y;
    float dt = M_PI / 180.0;
    float Ox = 0.0;
    float Oy = 0.0;

    float Ax = 0.0;
    float Ay = 0.0;
    float Bx = 0.0;
    float By = 0.0;
    float Cx = 0.0;
    float Cy = 0.0;
    float Dx = 0.0;
    float Dy = 0.0;

    for (float t = 0.0; t < 2 * M_PI; t = t + dt)
    {

        Ax = r * std::cos(t);
        Ay = r * std::sin(t);

        Bx = 2 * r;
        By = 0.0;

        Cx = (2 + std::cos(t)) * r * 0.5 + r * std::sin(t) * std::sqrt((5 + std::cos(t)) / (5 - 4 * std::cos(t)));
        Cy = std::sin(t) * r * 0.5 + r * (2 - std::cos(t)) * std::sqrt((5 + std::cos(t)) / (5 - 4 * std::cos(t)));

        Dx = 2 * r  + 2 * r * std::sin(t) * std::sqrt((5 + std::cos(t)) / (5 - 4 * std::cos(t)));
        Dy = (2 - std::cos(t)) * r * 2.0 * std::sqrt((5 + std::cos(t)) / (5 - 4 * std::cos(t)));

        
        x.push_back(Dx);
        y.push_back(Dy);

    }
    return std::make_tuple(x, y);
}

int main()
{
    auto sim = simulation();
    plot2D(std::get<0>(sim), std::get<1>(sim));
}
