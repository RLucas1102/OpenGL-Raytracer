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
        vec3 negate() const {
            return vec3(-_x, -_y, -_z);
        }

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

        void print() {
            std::cout << _x << ", " << _y << ", " << _z << std::endl;
        }

};

// Vector Arithmetic
// -----------------
inline vec3 add(const vec3& u, const vec3& v) {
    return vec3(u.getX() + v.getX(), u.getY() + v.getY(), u.getZ() + v.getZ());
}
        
inline vec3 subtract(const vec3& u, const vec3& v) {
    return vec3(u.getX() - v.getX(), u.getY() - v.getY(), u.getZ() - v.getZ());
}
        
inline vec3 multiply(const vec3& u, float scalar) {
    return vec3(u.getX() * scalar, u.getY() * scalar, u.getZ() * scalar);
}

inline float dot(const vec3& u, const vec3& v) {
    return    (u.getX() * v.getX()) 
            + (u.getY() * v.getY())
            + (u.getZ() * v.getZ());
}
        
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(((u.getY() * v.getZ()) - (u.getZ() * v.getY())), 
                ((u.getZ() * v.getX()) - (u.getX() * v.getZ())),
                ((u.getX() * v.getY()) - (u.getY() * v.getX())));
}

#endif