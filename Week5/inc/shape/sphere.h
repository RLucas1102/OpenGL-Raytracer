/********************************************************************************
 * Name: Lucas Robenolt
 * 
 * Last Updated: 3/12/2025
 * 
 * Email: robenoltl@gmail.com
 *
 * Purpose: This is a derived class of shape and holds the information that 
 *          is specific to a sphere. Additionally, it provides the hit function
 *          that is specific to a sphere.
 *          
 ********************************************************************************/

#ifndef SPHERE_H
#define SPHERE_H

#include <shape/interval.h>
#include <shape/shape.h>
#include <vec/vec3.h>
#include <vec/ray.h>

class sphere : public shape {
    private:
        vec3 _center;
        float _radius;

    public:
        // Constructor
        sphere(const vec3& center, float radius) : _center(center), _radius(std::fmax(0, radius)) {}

        /*
         * This function takes a ray being shot into the scene
         * and a valid interval for t to detemine if the single
         * ray has hit a sphere within our scene.
         * 
         * @param r is a ray being shot into the scene and
         * ray_t is the valid interval for t that a hit can occur
         * @return true or false if a hit happened
         */
        bool hit(const ray& r, interval ray_t) override {
            
            bool hitSuccess = true;

            // Calculate the discriminant of (C−(Q+td))⋅(C−(Q+td))=r^2
            // to solve for two and find 0-2 roots
            vec3 CQ = subtract( _center, r.getOrigin() );
            float a = r.getDirection().length_squared();
            float b = -2.0 * dot( r.getDirection(), CQ );
            float c = CQ.length_squared() - _radius * _radius;
            float discriminant = b*b - 4*a*c;

            // Discriminant is negative resulting in no hits (the ray passes the object)
            if(discriminant < 0) {
                hitSuccess = false;
            }
            else {

                float t;
                vec3 outward_normal;

                // Solve for t 
                t = (-b - std::sqrt(discriminant))/(2.0*a);

                // If t does not fall within the given interval for t, check other root
                // If neither fall within the interval, a hit is unsuccessful
                if (!ray_t.surrounds(t)) {
                    t = (-b + std::sqrt(discriminant))/(2.0*a);
                    if(!ray_t.surrounds(t)) {
                        hitSuccess = false;
                    }
                }

                setT(t); // Set t for where the hit point occurs
                setHitPoint( r.at(t) ); // Set hit point

                // The normal for a sphere is hit point (P) - center (C)
                // To get a normal with unit length, we simply divide the difference by the radius
                outward_normal = multiply(subtract(getHitPoint(), _center), 1/_radius); 
                setFaceNormal(r, outward_normal);

            }
        
            return hitSuccess;
        }
};


#endif