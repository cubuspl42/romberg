//
//  romberg.h
//  romberg
//
//  Created by Jakub Trzebiatowski on 21/01/15.
//
//

#ifndef romberg_romberg_h
#define romberg_romberg_h

#include "math_parser.h"

double romberg(const std::vector<Element> &function, double a, double b, unsigned iterations);

#endif
