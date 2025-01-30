#ifdef  VEC_H
#define VEC_H

#include <vector>
#include <cmath>
#include <iostream>

class vec 
{

    private:
        std::vector<float> vec_;

    public:
        // Constructors
        vec() { vec_ = {0, 0, 0}; }
        vec(float val1, float val2, float val3) { vec_ = {val1, val2, val3}; }

        float x() const { return vec_.at(0); }
        float y() const { return vec_.at(1); }
        float z() const { return vec_.at(2); }

        
            
};

#endif