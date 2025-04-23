/********************************************************************************
 * Name: Lucas Robenolt
 * 
 * Last Updated: 4/23/2025
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
#include <raytracing/utils/utils.h>

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

        // Returns true if the vector is close to zero in all dimensions
        bool near_zero() const {
            float s = 1e-8;
            bool isClose = false;
            if(std::fabs(_x) < s && std::fabs(_y) < s && std::fabs(_z) < s) {
                isClose = true;
            }

            return isClose;
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

/*
 * This function creates a vec3 with random x,y,x
 * components
 * 
 * @param None
 * @return vec3
 */
inline vec3 random_vec() {
    return vec3(random_float(), random_float(), random_float());
}

/*
 * This function creates a vec3 with random x,y,x
 * components within a specified min and max
 * 
 * @param float min and max
 * @return vec3
 */
inline vec3 random_vec(float min, float max) {
    return vec3(random_float(min, max), random_float(min, max), random_float(min, max));
}

/*
 * This function attempts to find a vector
 * to work with that falls within the sphere.
 * Once it does, it generates a normal unit vector
 * 
 * @param None
 * @return vec3
 */
inline vec3 random_unit_vector() {
    bool foundVec = false;
    vec3 result;

    while(!foundVec) {
        vec3 point = random_vec(-1, 1);
        float mag = point.length_squared();
        if (1e-160 < mag && mag <= 1) {
            result = multiply(point, (1 / sqrt(mag)));
            foundVec = true;
        }
    }

    return result;
}

/*
 * This function tests to see what side of the 
 * sphere the unit vector is pointing. If the
 * dot is positive, it points outwards. If the
 * dot is negative, it points inwards and must
 * be reversed
 * 
 * @param The object's normal
 * @return vec3
 */
inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    vec3 result;

    if(dot(on_unit_sphere, normal) > 0.0) {
        result = on_unit_sphere;
    }
    else {
        result = on_unit_sphere.negate();
    }

    return result;
}

/*
 * This function converts our linear space
 * colors to gamma space. Computers expect
 * images to be gamma space, but, without
 * this function, we produce colors in
 * linear space.
 * 
 * @param The object's normal
 * @return vec3
 */
inline float linear_to_gamma(float linear_component) {
    float result = 0;

    if (linear_component > 0) {
        result = sqrt(linear_component);
    }

    return result;

}

/*
 * This function finds the reflection
 * vector from a vector and a normal
 * 
 * @param A vector and an object's normal
 * @return vec3
 */
inline vec3 reflect(const vec3& v, const vec3& n) {
    return subtract(v, multiply(n, 2 * dot(v, n)));
}


#endif