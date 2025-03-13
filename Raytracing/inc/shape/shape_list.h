/********************************************************************************
 * Name: Lucas Robenolt
 * 
 * Last Updated: 3/12/2025
 * 
 * Email: robenoltl@gmail.com
 *
 * Purpose: This class is used to create a list of objects where each object is
 *          an instance of the shape class. This list represents our world and 
 *          the hittable objects within it. 
 *          
 ********************************************************************************/


#ifndef SHAPE_LIST_H
#define SHAPE_LIST_H

#include <vec/vec3.h>
#include <vec/ray.h>
#include <shape/shape.h>
#include <shape/interval.h>

#include <memory>
#include <vector>

using std::make_shared; // Returns a shared pointer of the given type
using std::shared_ptr; // A smart pointer that increments a reference counter every time a new instance is allocated

class shape_list {
    private:
        std::vector<shared_ptr<shape>> _objects; // Main list to hold shapes
        shared_ptr<shape> _temp_object; // Temp storage to hold info of current shape in question; used in camera class

    public:

        // Constructors
        shape_list() {} // Default
        shape_list(shared_ptr<shape> object) { add(object); }

        // Clear the shape list
        void clear() { _objects.clear(); }

        // Add new object to the back of the list
        void add(shared_ptr<shape> object) {
            _objects.push_back(object);
        }

        // For each object in the object list, see what is the closest object to camera
        bool hit(const ray& r, interval ray_t) {
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

