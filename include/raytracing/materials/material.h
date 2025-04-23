/********************************************************************************
 * Name: Lucas Robenolt
 * 
 * Last Updated: 4/23/2025
 * 
 * Email: robenoltl@gmail.com
 *
 * Purpose: This class is an abstract class for any derived classes that
 *          represent a material. It holds the derived classes for metal, matte,
 *          and glass materials.
 *          
 ********************************************************************************/

#ifndef MATERIAL_H
#define MATERIAL_H

#include <raytracing/shape/shape.h>
#include <raytracing/vec/ray.h>

using std::shared_ptr;

class material {
    public:
        virtual ~material() = default;

        virtual bool scatter(const ray& r_in, const shared_ptr<shape> hit_shape, vec3& attenuation, ray& scattered) const {
            return false;
        }
};

class lambertian : public material {
    private:
        vec3 _albedo;

    public:
        lambertian(const vec3& albedo) : _albedo(albedo) {}

        bool scatter(const ray& r_in, const shared_ptr<shape> hit_shape, vec3& attenuation, ray& scattered) const  override {
            vec3 scatter_dir = add(hit_shape->getNormal(), random_unit_vector());

            if (scatter_dir.near_zero()) {
                scatter_dir = hit_shape->getNormal();
            }

            scattered = ray(hit_shape->getHitPoint(), scatter_dir);
            attenuation = _albedo;
            return true;
        }
};

class metal : public material {
    private:
        vec3 _albedo;
        float _fuzz;

    public:
        metal(const vec3& albedo, float fuzz) : _albedo(albedo) {
            if (fuzz > 1) {
                _fuzz = 1;
            }
            else {
                _fuzz = fuzz;
            }
        }

        bool scatter(const ray& r_in, const shared_ptr<shape> hit_shape, vec3& attenuation, ray& scattered) const  override {
            bool result = false;

            vec3 reflect_dir = reflect(r_in.getDirection(), hit_shape->getNormal());
            reflect_dir = add(normalize(reflect_dir), (multiply(random_unit_vector(), _fuzz)));
            scattered = ray(hit_shape->getHitPoint(), reflect_dir);
            attenuation = _albedo;
            
            if(dot(scattered.getDirection(), hit_shape->getNormal()) > 0) {
                result = true;
            }

            return result;
        }
};

#endif