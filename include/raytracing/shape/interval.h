/********************************************************************************
 * Name: Lucas Robenolt
 * 
 * Last Updated: 4/23/2025
 * 
 * Email: robenoltl@gmail.com
 *
 * Purpose: This class manages the real-valued intervals where 't' is valid and 
 *          counts as a hit. 't' is the variable used within the parametric
 *          calculation of a ray.
 *          
 ********************************************************************************/

#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits> // Used to define infinity below

#define infinity std::numeric_limits<double>::infinity()

class interval {
    private:
        float _min, _max;
    
    public:

        // Static variables
        static const interval empty, universe;

        // Constructors
        interval() : _min(infinity), _max(-infinity) {} // Default

        interval(float min, float max) : _min(min), _max(max) {} // Used for interval with specific min and max

        // Accessors
        float getMin() const { return _min; }
        float getMax() const { return _max; }
    
        // Mutators
        void setMin(float value) { _min = value; }
        void setMax(float value) { _max = value; }

        // Other functions
        
        // Returns the difference between max and min to find the size of the interval
        float size() const { return _max - _min; }

        // Returns true or false depending if t is within the acceptable range
        bool surrounds(float t) const { return _min < t && t < _max; }

        // Ensures that a value is within the min and max
        float clamp(float x) const {
            float result;

            if (x < _min) {
                result = _min;
            }
            else if(x > _max) {
                result = _max;
            }
            else {
                result = x;
            }

            return result;
        }

};
    
// Static variable definitions
const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif