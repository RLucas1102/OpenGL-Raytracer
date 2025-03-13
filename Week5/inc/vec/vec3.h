/********************************************************************************
 * Name: Lucas Robenolt
 * 
 * Last Updated: 3/12/2025
 * 
 * Email: robenoltl@gmail.com
 *
 * Purpose: This vector class provides a construct for making vectors in 3D space.
 *          Useful utility functions that vectors will use, but are not members
 *          of the vector class exists below the class. Overall this class,
 *          provides users a way to create vectors and do simple vector arithmetic
 * 
 ********************************************************************************/

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
        
        // Returns a 3D vector that is the negation of the current vector object
        vec3 negate() const {
            return vec3(-_x, -_y, -_z);
        }

        // Returns the magnitude (length) of the vector object
        float length() const {
            return std::sqrt(this->length_squared());
        }

        // Returns the magnitude squared (length squared) of the vector object
        float length_squared() const {
            return (_x * _x) + (_y * _y) + (_z * _z);
        }

        // Print out the x, y, and z components of the vector
        void print() {
            std::cout << _x << ", " << _y << ", " << _z << std::endl;
        }

};

// Vector Arithmetic
// -----------------

/*
 * This function adds two vectors together and returns the result.
 * 
 * @param u and v are any arbitrary vectors
 * @return Resulting vector when doing u + v
 */
inline vec3 add(const vec3& u, const vec3& v) {
    return vec3(u.getX() + v.getX(), u.getY() + v.getY(), u.getZ() + v.getZ());
}
     
/*
 * This function subtracts two vectors together and returns the result.
 * 
 * @param u and v are any arbitrary vectors 
 * @return Resulting vector when doing u - v
 */
inline vec3 subtract(const vec3& u, const vec3& v) {
    return vec3(u.getX() - v.getX(), u.getY() - v.getY(), u.getZ() - v.getZ());
}
  
/*
 * This function multiplies a vector by a scalar and returns the result
 * 
 * @param u is an arbitrary vector and scalar is the value to multiply by
 * @return Resulting vector when doing scalar * u
 */
inline vec3 multiply(const vec3& u, float scalar) {
    return vec3(u.getX() * scalar, u.getY() * scalar, u.getZ() * scalar);
}

/*
 * This function computes the dot product of two vectors to find
 * the angle between them.
 * 
 * @param u and v are any arbitrary vectors 
 * @return Resulting vector when doing u . v
 */
inline float dot(const vec3& u, const vec3& v) {
    return    (u.getX() * v.getX()) 
            + (u.getY() * v.getY())
            + (u.getZ() * v.getZ());
}
        
/*
 * This function computes the cross product of two vectors to find
 * a new vector that is orthogonal to u and v
 * 
 * @param u and v are any arbitrary vectors
 * @return Resulting vector when doing u x v
 */
inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(((u.getY() * v.getZ()) - (u.getZ() * v.getY())), 
                ((u.getZ() * v.getX()) - (u.getX() * v.getZ())),
                ((u.getX() * v.getY()) - (u.getY() * v.getX())));
}

/*
 * This function normalizes a given vector to "remove" the magnitude
 * from the vector and only consider its direction
 * 
 * @param v is any arbitrary vector
 * @return Normalized vector of given vector
 */
inline vec3 normalize(const vec3& v) {
    float vec_length = v.length();

    float norm_x = v.getX() / vec_length;
    float norm_y = v.getY() / vec_length;
    float norm_z = v.getZ() / vec_length;

    return vec3(norm_x, norm_y, norm_z);
}

#endif