#ifndef SHAPE_H
#define SHAPE_H

#include <vec/vec3.h>
#include <vec/ray.h>

class shape {
    private:
        vec3 _hit_point;
        vec3 _normal;
        float _t;

    public:
        // Destructor
        virtual ~shape() = default;

        // Hit detection function (Will be different for every shape)
        virtual bool hit(const ray& r, float ray_tmin, float ray_tmax) = 0;

        // Accessors
        const vec3& getHitPoint() const { return _hit_point; }
        const vec3& getNormal() const { return _normal; }
        float getT() const { return _t; }
    
        // Mutators 
        void setHitPoint(const vec3& point) { _hit_point = point; }
        void setNormal(const vec3& norm) { _normal = norm; }
        void setT(float distance) { _t = distance; }

};


#endif