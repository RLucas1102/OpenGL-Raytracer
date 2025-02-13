#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

class vec3 
{
    private:
        // Member variables
        // ----------------
        float _x;
        float _y;
        float _z;

    public:

        // Constructors
        // ------------
        vec3() : _x(0), _y(0), _z(0) {}

        vec3(float x, float y, float z) : _x(x), _y(y), _z(z) {}
        
        // Accessors
        // ---------
        float getX() const { return _x; }
        
        float getY() const { return _y; }
        
        float getZ() const { return _z; }

        // Mutators
        // --------
        void setX(float x) { _x = x; }
        
        void setY(float y) { _y = y; }
        
        void setZ(float z) { _z = z; }
        
        // Other useful member functions
        // -----------------------------
        vec3 normalize() const {
            float vec_length = this->length();

            float norm_x = _x / vec_length;
            float norm_y = _y / vec_length;
            float norm_z = _z / vec_length;

            return vec3(norm_x, norm_y, norm_z);
        }

        float length() const {
            return std::sqrt(this->length_squared());
        }

        float length_squared() const {
            return (_x * _x) + (_y * _y) + (_z * _z);
        }

        vec3 negate() {
            return vec3(-_x, -_y, -_z);
        }

        void print() {
            std::cout << _x << ", " << _y << ", " << _z << std::endl;
        }

};

#endif