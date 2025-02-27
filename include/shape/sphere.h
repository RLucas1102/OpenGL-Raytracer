#ifndef SPHERE_H
#define SPHERE_H

#include <shape/shape.h>
#include <vec/vec3.h>
#include <vec/ray.h>

class sphere : public shape {
    private:
        vec3 _center;
        float _radius;

    public:
        //Constructor
        sphere(const vec3& center, float radius) : _center(center), _radius(std::fmax(0, radius)) {}

        //Implemented virtual functions
        bool hit(const ray& r, float ray_tmin, float ray_tmax) override {
            
            bool hitSuccess = true;

            vec3 CQ = subtract( _center, r.getOrigin() );
            float a = r.getDirection().length_squared();
            float b = -2.0 * dot( r.getDirection(), CQ );
            float c = CQ.length_squared() - _radius * _radius;
            float discriminant = b*b - 4*a*c;

            if(discriminant < 0) {
                hitSuccess = false;
            }
            else {

                float t;

                t = (-b - std::sqrt(discriminant))/(2.0*a);

                if (t <= ray_tmin || t >= ray_tmax) {
                    t = (-b + std::sqrt(discriminant))/(2.0*a);
                    hitSuccess = false;
                }

                setT(t);
                setHitPoint( r.at(t) );
                setNormal( multiply(subtract(getHitPoint(), _center), 1/_radius) ); // _normal = (_hit_point - _center) / _radius

            }
        
            return hitSuccess;
        }
};


#endif