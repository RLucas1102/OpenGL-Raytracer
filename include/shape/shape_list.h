#ifndef SHAPE_LIST_H
#define SHAPE_LIST_H

#include <vec/vec3.h>
#include <vec/ray.h>
#include <shape/shape.h>
#include <shape/interval.h>

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class shape_list {
    private:
        std::vector<shared_ptr<shape>> _objects; // Main list to hold shapes
        shared_ptr<shape> _temp_object;

    public:

        // Constructors
        shape_list() {} // Default
        shape_list(shared_ptr<shape> object) { add(object); }

        // List functions
        void clear() { _objects.clear(); }

        void add(shared_ptr<shape> object) {
            _objects.push_back(object);
        }

        // For each object in the object list, see what is the closest object to camera
        bool render(const ray& r, interval ray_t) {
            bool hitSuccess = false;
            float closest = ray_t.getMax();

            for(const shared_ptr<shape>& object : _objects) {
                if(object->hit(r, interval(ray_t.getMin(), closest))) {
                    hitSuccess = true;
                    closest = object->getT();
                    _temp_object = object;
                }
            }

            return hitSuccess;

        }

        // Accessor
        shared_ptr<shape> getTempObject() const {
            return _temp_object;
        }


};

#endif

