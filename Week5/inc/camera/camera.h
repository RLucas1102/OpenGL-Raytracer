/********************************************************************************
 * Name: Lucas Robenolt
 * 
 * Last Updated: 3/12/2025
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

#include <vec/vec3.h>
#include <vec/ray.h>
#include <shape/sphere.h>
#include <shape/shape.h>
#include <shape/shape_list.h>
#include <shape/interval.h>

#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include <cmath>

using std::make_shared; // Returns a shared pointer of the given type
using std::shared_ptr;  // A smart pointer that increments a reference counter every time a new instance is allocated

#define infinity std::numeric_limits<double>::infinity() 
#define pi 3.1415926535897932385

class camera {
    private:
        //Image
        float _aspect_ratio;
        int _img_width;
        int _img_height;

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

            _focal_length = 1.0;
            _camera_center = vec3(0,0,0); // Camera starts at 0,0,0 in space

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
            int rByte = (int)255.999 * r;
            int gByte = (int)255.999 * g;
            int bByte = (int)255.999 * b;
        
            std::cout << rByte << ' ' << gByte << ' ' << bByte << "\n";
        
        }

    public:

        void setAspect(float aspect) { _aspect_ratio = aspect; }
        void setImgWidth(int width) { _img_width = width; }

        void render(shape_list& world) {
            // Set all variables
            initialize();

            // P3 states we are using ASCII for our colors
            // Image size is base on _img_width x _img_height
            // Max color will be 255
            std::cout << "P3\n" << _img_width << ' ' << _img_height << "\n255\n";

            // For every pixel of the image from left to right, top to bottom,
            // shoot a ray from the camera into the center of the viewport pixel.
            // Determine the ray direction by taking the difference between the
            // pixel center and camera center. Then create a new ray and determine
            // the color based on what is hit within the scene.
            for (int i = 0; i < _img_height; i++) {
                for (int j = 0; j < _img_width; j++) {
                
                    // _first_pix_loc + (i * _pixel_du) + (j * _pixel_dv)
                    vec3 pixel_center = add(add(_first_pix_loc, multiply(_pixel_du, j)), multiply(_pixel_dv, i));
                    vec3 ray_dir = subtract(pixel_center, _camera_center);
                    ray r(_camera_center, ray_dir);
                
                    // Send pixel color to file for output
                    vec3 pixel_color = rayColor(r, world);
                    writeColor(pixel_color);
                
                }
            }

        }

};

#endif