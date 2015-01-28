#include "math_parser.h"
#include "romberg.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>

int main(int argc, const char **argv) {
    std::stringstream ss;
    for(unsigned i = 1; i < argc; ++i)
        ss << argv[i] << ' ';
    
    double a, b;
    unsigned iterations;
    ss >> a >> b >> iterations;
    assert(ss.good());
    
    std::vector<Element> buffer = parse_expression(ss);
    std::cout << std::fixed << romberg(buffer, a, b, iterations) << std::endl;
    
    return 0;
}
