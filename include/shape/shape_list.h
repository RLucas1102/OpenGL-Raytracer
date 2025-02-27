#ifndef SHAPE_LIST_H
#define SHAPE_LIST_H

#include <vec/vec3.h>
#include <vec/ray.h>
#include <shape/shape.h>

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class shape_list {
    private:
        std::vector<shared_ptr<shape>> objects; // Main list to hold shapes

    public:

        // Constructors
        shape_list() {} // Default
        shape_list(shared_ptr<shape> object) { add(object); }

        // List functions
        void clear() { objects.clear(); }

        void add(shared_ptr<shape> object) {
            objects.push_back(object);
        }

        // For each object in the object list, see what is the closest object to camera
        bool render(const ray& r, float ray_tmin, float ray_tmax) const {
            bool hitSuccess = false;
            float closest = ray_tmax;

            for(const shared_ptr<shape>& object : objects) {
                if(object->hit(r, ray_tmin, closest)) {
                    hitSuccess = true;
                    closest = object->getT();
                }
            }

            return hitSuccess;

        }


};

#endif

