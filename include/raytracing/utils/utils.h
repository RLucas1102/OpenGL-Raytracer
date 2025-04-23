/********************************************************************************
 * Name: Lucas Robenolt
 * 
 * Last Updated: 4/23/2025
 * 
 * Email: robenoltl@gmail.com
 *
 * Purpose: This utils class constructs holds useful utility functions for
 *          the ray tracer to work. Simple functions like converting degrees
 *          to radians, or getting random numbers will go here
 *
 ********************************************************************************/


#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

#define infinity std::numeric_limits<double>::infinity()
#define pi 3.1415926535897932385

// Converts angle in degrees to radians
inline float degreesToRadians(float degrees) {
    return (degrees * pi) / 180.0;
}

// Returns a random float between [0,1)
inline float random_float() {
    return std::rand() / (RAND_MAX + 1.0);
}

// Returns a random float between [min, max)
inline float random_float(float min, float max) {
    return min + (max-min)*random_float();
}


#endif