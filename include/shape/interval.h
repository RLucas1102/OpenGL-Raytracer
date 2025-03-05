#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>

#define infinity std::numeric_limits<double>::infinity()

class interval {
    private:
        float _min, _max;
    
    public:

        // Static variables
        static const interval empty, universe;

        // Constructors
        interval() : _min(infinity), _max(-infinity) {} // Default

        interval(float min, float max) : _min(min), _max(max) {}

        // Accessors
        float minValue() const { return _min; }
        float maxValue() const { return _max; }
    
        // Mutators
        void setMinValue(float value) { _min = value; }
        void setMaxValue(float value) { _max = value; }

        // Other functions
        float size() const { return _max - _min; }

        bool contains(float x) const { return _min <= x && x <= _max; }

        bool surrounds(float x) const { return _min < x && x < _max; }

};
    
// Static variable definitions
const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

#endif