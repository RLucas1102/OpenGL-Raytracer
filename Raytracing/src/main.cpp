#include <vec/vec3.h>
#include <vec/ray.h>
#include <shape/sphere.h>
#include <shape/shape.h>
#include <shape/shape_list.h>
#include <shape/interval.h>
#include <camera/camera.h>

#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include <cmath>

using std::make_shared;
using std::shared_ptr;

#define infinity std::numeric_limits<double>::infinity()
#define pi 3.1415926535897932385

// Prototypes
// ----------
float degreesToRadians(float degrees);

int main() {

    // Create camera object
    camera MyCamera;

    // Set image aspect ratio and width
    MyCamera.setAspect(16.0/9.0);
    MyCamera.setImgWidth(400);
    
    // World setup
    shape_list world;

    // Add a sphere and a "plane" to the scene
    world.add(make_shared<sphere>(vec3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(vec3(0,-100.5,-1), 100));

    MyCamera.render(world);

    return 0;
}

// Function definitions
// --------------------
float degreesToRadians(float degrees)
{
    return (degrees * pi) / 180.0;
}
