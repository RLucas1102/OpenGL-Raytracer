/********************************************************************************
 * Name: Lucas Robenolt
 * 
 * Last Updated: 4/23/2025
 * 
 * Email: robenoltl@gmail.com
 *
 * Purpose: This class is an abstract class for any derived classes that
 *          represent a shape. The shape class is a general class to tracks hits
 *          and face normals of shapes within the rendered scene. Simply, shapes
 *          can just be something that are hit.
 *          
 ********************************************************************************/

#ifndef SHAPE_H
#define SHAPE_H

#include <raytracing/shape/interval.h>
#include <raytracing/vec/vec3.h>
#include <raytracing/vec/ray.h>

using std::shared_ptr;

class material; // Breaks circular dependencies; tells compiler we will define material later

class shape {
    private:
        vec3 _hit_point;
        vec3 _normal;
        float _t;
        bool _isFront;
        shared_ptr<material> _mat;

    public:
        // Destructor
        virtual ~shape() = default;

        // Hit detection function (Will be different for every shape)
        virtual bool hit(const ray& r, interval ray_t) = 0;

        // Sets the shape's normal vector
        // Outward_normal is assumed to be of unit length
        void setFaceNormal(const ray& r, const vec3& outward_normal) {
            if (dot(r.getDirection(), outward_normal) > 0.0) {
                // Ray is intersecting from inside the shape
                _normal = outward_normal.negate();
                _isFront = false;
            }
            else {
                // Ray is intersecting from outside the shape
                _normal = outward_normal;
                _isFront = true;
            }

        }

        // Accessors
        const vec3& getHitPoint() const { return _hit_point; }
        const vec3& getNormal() const { return _normal; }
        float getT() const { return _t; }
        const shared_ptr<material> getMat() {return _mat; }
    
        // Mutators 
        void setHitPoint(const vec3& point) { _hit_point = point; }
        void setNormal(const vec3& norm) { _normal = norm; }
        void setT(float distance) { _t = distance; }
        void setMat(const shared_ptr<material> mat) {_mat = mat; }

};


#endif