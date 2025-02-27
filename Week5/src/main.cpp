#include <vec/vec3.h>
#include <vec/ray.h>
#include <shape/sphere.h>
#include <shape/shape.h>
#include <shape/shape_list.h>

#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include <cmath>
#include <fstream>

using std::make_shared;
using std::shared_ptr;

#define infinity std::numeric_limits<double>::infinity()
#define pi 3.1415926535897932385

// Prototypes
// ----------
void writeColor(std::ofstream& outputFile, const vec3& pixel_color);

vec3 rayColor(const ray& r, shape_list& world);

float degreesToRadians(float degrees);

int main() {

    // File
    std::ofstream myFile;
    myFile.open("image.ppm");

    // Image
    float aspect_ratio = 16.0/9.0;
    int img_width  = 400;
    int img_height = img_width / aspect_ratio; // Calculate img_height based on width to maintain aspect ratio
    if (img_height < 1)
        img_height = 1; 

    std::cout << "Img Dimensions: " << img_width << "x" << img_height << std::endl;

    // World setup
    shape_list world;

    world.add(make_shared<sphere>(vec3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(vec3(0,-100.5,-1), 100));

    // Camera setup
    float focal_length = 1.0;
    vec3 camera_center(0,0,0);

    // Viewport setup
    float viewport_height = 2.0;
    float viewport_width = viewport_height * ((float)img_width / img_height);

    std::cout << "Viewport Dimensions: " << viewport_width << "x" << viewport_height << std::endl;

    // Vectors to go along the edges of the viewport
    vec3 viewport_u = vec3(viewport_width, 0, 0); // Goes from left to right
    vec3 viewport_v = vec3(0, -viewport_height, 0); // Goes from top to bottom (must be negative)
    
    std::cout << "Viewport edge vectors (u,v): " << std::endl;
    viewport_u.print();
    viewport_v.print();

    // Calculate the spacing between each pixel
    vec3 pixel_du = multiply(viewport_u, (float)1/img_width); 
    vec3 pixel_dv = multiply(viewport_v, (float)1/img_height);

    std::cout << "Pixel spacing (stride) (u,v)" << std::endl;
    pixel_du.print();
    pixel_dv.print();

    // Upper-left hand pixel of viewport
    // camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2
    vec3 viewport_upper_left = subtract(subtract(subtract(camera_center, vec3(0, 0, focal_length)), multiply(viewport_u, 0.5)), multiply(viewport_v, 0.5));

    // viewport_upper_left + 0.5 * (pixel_du + pixel_dv)
    vec3 first_pix_loc = add(viewport_upper_left, multiply(add(pixel_du, pixel_dv), 0.5));

    std::cout << "Upper left corner: ";
    viewport_upper_left.print();
    std::cout << "First pixel location: ";
    first_pix_loc.print();

    // Render
    myFile << "P3\n" << img_width << ' ' << img_height << "\n255\n";

    for (int i = 0; i < img_height; i++) {
        for (int j = 0; j < img_width; j++) {

            // first_pix_loc + (i * pixel_du) + (j * pixel_dv)
            vec3 pixel_center = add(add(first_pix_loc, multiply(pixel_du, j)), multiply(pixel_dv, i));
            vec3 ray_dir = subtract(pixel_center, camera_center);
            ray r(camera_center, ray_dir);

            // Send pixel color to file for output
            vec3 pixel_color = rayColor(r, world);
            writeColor(myFile, pixel_color);

        }
    }

    myFile.close();

    return 0;
}

// Function definitions
// --------------------

// Takes in pixels with three color components, converts them to [0,255], writes to file
void writeColor(std::ofstream& outputFile, const vec3& pixel_color) {
    float r = pixel_color.getX();
    float g = pixel_color.getY();
    float b = pixel_color.getZ();

    // Translate all the [0,1] values to RGB [0,255]
    int rByte = (int)255.999 * r;
    int gByte = (int)255.999 * g;
    int bByte = (int)255.999 * b;

    outputFile << rByte << ' ' << gByte << ' ' << bByte << "\n";

}

// Determine the pixel color that will be output based on ray intersection
// Returns pixel_color
vec3 rayColor(const ray &r, shape_list &world)
{
    vec3 result;

    if(world.render(r, 0, infinity)) {
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

float degreesToRadians(float degrees)
{
    return (degrees * pi) / 180.0;
}
