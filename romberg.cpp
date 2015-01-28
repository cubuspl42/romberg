#include "romberg.h"

#include <cmath>
#include <vector>

double romberg(const std::vector<Element> &function, double a, double b, unsigned iterations)
{
    std::vector<double> T_prev(1), T(1);
    std::map<char, double> v;
    double &x = v['x'];
    
    x = a;
    double f_a = evaluate(function, v);
    x = b;
    double f_b = evaluate(function, v);
    T[0] = 0.5 * (b - a) * (f_a + f_b);
    
    double h = b - a;
    unsigned n = 1;
    for(unsigned i = 1; i < iterations; ++i) {
        T.push_back(0), T_prev.push_back(0);
        std::swap(T, T_prev);
        h *= 0.5;
        
        double sum = 0;
        for(unsigned k = 0; k < n; ++k) {
            x = a + (2*k + 1)*h;
            sum += evaluate(function, v);
        }
        n *= 2;
        T[0] = 0.5*T_prev[0] + h*sum;
        
        unsigned m = 1;
        for(unsigned j = 1; j < i + 1; ++j) {
            m *= 4;
            T[j] = (m*T[j-1] - T_prev[j-1])/(m - 1);
        }
    }
    return T.back();
}