/********************************************************************************
 * Name: Lucas Robenolt
 * 
 * Last Updated: 4/23/2025
 * 
 * Email: robenoltl@gmail.com
 *
 * Purpose: This camera class constructs and dispatches rays into the world.
 *          Additionally, it uses the results (what was hit) to construct
 *          the rendered image.
 *
 ********************************************************************************/

#ifndef CAMERA_H
#define CAMERA_H

#include <raytracing/vec/vec3.h>
#include <raytracing/vec/ray.h>
#include <raytracing/shape/sphere.h>
#include <raytracing/shape/shape.h>
#include <raytracing/shape/shape_list.h>
#include <raytracing/shape/interval.h>
#include <raytracing/utils/utils.h>

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <limits>
#include <cmath>

using std::make_shared; // Returns a shared pointer of the given type
using std::shared_ptr;  // A smart pointer that increments a reference counter every time a new instance is allocated


class camera {
    private:
        std::ofstream ofs;

        //Image
        float _aspect_ratio;
        int _img_width;
        int _img_height;
        int _samples_per_pixel;
        float _pixel_samples_scale; // Color scale factor for a sum of pixel samples

        // Camera setup
        float _focal_length;
        vec3 _camera_center;

        // Viewport setup
        float _viewport_height;
        float _viewport_width;

        // Vectors to go along the edges of the viewport
        vec3 _viewport_u;
        vec3 _viewport_v;

        // Spacing between each pixel
        vec3 _pixel_du;
        vec3 _pixel_dv;

        // Upper-left hand pixel of viewport
        vec3 _viewport_upper_left;
        vec3 _first_pix_loc; //Location of pixel (0,0)

        // This function is called within the reader function and before an image can be rendered
        // in order to set all image and viewport properties
        void initialize() {
            _img_height = _img_width / _aspect_ratio; // Calculate _img_height based on width to maintain aspect ratio
            if (_img_height < 1)
                _img_height = 1; 

            _pixel_samples_scale = 1.0 / _samples_per_pixel;
            
            _focal_length = 1.0;
            _camera_center = vec3(0,0,1); // Camera starts at 0,0,0 in space

            _viewport_height = 2.0; // Arbitrary
            _viewport_width = _viewport_height * ((float)_img_width / _img_height); // Calculating aspect ratio with width and height for accuracy
            
            _viewport_u = vec3(_viewport_width, 0, 0); // Goes from left to right
            _viewport_v = vec3(0, -_viewport_height, 0); // Goes from top to bottom (must be negative)

            // Calculate the exact spacing between each pixel center based on the current image size 
            // and viewport horizontal and vertical vectors
            _pixel_du = multiply(_viewport_u, (float)1/_img_width); 
            _pixel_dv = multiply(_viewport_v, (float)1/_img_height);

            // Calculates the very top left corner of the viewport
            _viewport_upper_left = subtract(subtract(subtract(_camera_center, vec3(0, 0, _focal_length)), multiply(_viewport_u, 0.5)), multiply(_viewport_v, 0.5));

            // The first pixel is inset by half the spacing between each pixel
            // This ensures that the viewport is evenly divided
            // In this instance, for pixels, we are using the little square method. This would be the center of the square
            _first_pix_loc = add(_viewport_upper_left, multiply(add(_pixel_du, _pixel_dv), 0.5));

        }

        /*
         * This function determines the pixel color that will be
         * based on what is hit by a ray sent into the world. If an
         * object is hit, we return the shape's color. If nothing
         * was hit, then we return the background color.
         * 
         * @param r is a ray being shot into the scene and
         * world which contains all out objects
         * @return a color vector
         */
        vec3 rayColor(const ray &r, shape_list &world)
        {
            vec3 result;
        
            if(world.hit(r, interval(0, infinity))) {
                vec3 white(1,1,1);
                result = multiply(add(world.getTempObject()->getNormal(), white), 0.5); // 0.5 * (shape's normal + white(1,1,1))
            }
            else {
                vec3 unit_direction = normalize(r.getDirection());
                float alpha = 0.5 * (unit_direction.getY() + 1.0);
                vec3 color1(1, 1, 1);
                vec3 color2(0.3, 0.5, 1.0);
                result = add(multiply(color1, (1.0 - alpha)), multiply(color2, alpha));
            }
        
            return result;
        }

        /*
         * This function takes in pixels with three color 
         * components, converts them to [0,255], writes to
         * standard output
         * 
         * @param pixel_color is a vector with RGB values
         * @return none
         */
        void writeColor(const vec3& pixel_color) {
            // Save color values from vector
            float r = pixel_color.getX();
            float g = pixel_color.getY();
            float b = pixel_color.getZ();
        
            // Translate all the [0,1] values to RGB [0,255]
            interval intensity(0.000, 0.999);
            int rByte = (int)(255.999 * intensity.clamp(r));
            int gByte = (int)(255.999 * intensity.clamp(g));
            int bByte = (int)(255.999 * intensity.clamp(b));
        
            ofs << rByte << ' ' << gByte << ' ' << bByte << "\n";
        
        }

        /*
         * This function creates a ray directed towards 
         * a randomly sampled point around the pixel
         * defined by location i, j
         * 
         * @param i and j are the 2D components of a pixel
         * @return ray
         */
        ray get_ray(int i, int j) const {
            vec3 offset = sample_square();
            vec3 pixel_sample = add(_first_pix_loc,
                                add(
                                    multiply(_pixel_du, i + offset.getX()), 
                                    multiply(_pixel_dv, j + offset.getY())
                                    )
                                );

            vec3 ray_origin = _camera_center;
            vec3 ray_dir = subtract(pixel_sample, ray_origin);

            return ray(ray_origin, ray_dir);
        }

        /*
         * This function generates a random point within a square
         * Will be used to sample points in a unit square
         * 
         * @param None
         * @return vec3
         */
        vec3 sample_square() const {
            return vec3(random_float() - 0.5, random_float() - 0.5, 0);
        }

    public:

        void setAspect(float aspect) { _aspect_ratio = aspect; }
        void setImgWidth(int width) { _img_width = width; }
        void setPixSamples(int samples) {_samples_per_pixel = samples; }

        void render(shape_list& world) {

            ofs.open("images/output.ppm");

            // Set all variables
            initialize();

            // P3 states we are using ASCII for our colors
            // Image size is base on _img_width x _img_height
            // Max color will be 255
            ofs << "P3\n" << _img_width << ' ' << _img_height << "\n255\n";

            // For every pixel of the image from left to right, top to bottom,
            // shoot a ray from the camera into the center of the viewport pixel.
            // Determine the ray direction by taking the difference between the
            // pixel center and camera center. Then create a new ray and determine
            // the color based on what is hit within the scene.
            for (int i = 0; i < _img_height; i++) {
                for (int j = 0; j < _img_width; j++) {
                    vec3 pixel_color(0,0,0);
                    for (int sample = 0; sample < _samples_per_pixel; sample++) {
                        ray r = get_ray(j, i);
                        pixel_color = add(pixel_color, rayColor(r, world));
                    }
                
                    // Send pixel color to file for output
                    writeColor(multiply(pixel_color, _pixel_samples_scale));
                
                }
            }

            ofs.close();

        }

};

#endif