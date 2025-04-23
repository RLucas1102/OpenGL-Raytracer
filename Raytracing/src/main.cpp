#include <raytracing/vec/vec3.h>
#include <raytracing/vec/ray.h>
#include <raytracing/shape/sphere.h>
#include <raytracing/shape/shape.h>
#include <raytracing/shape/shape_list.h>
#include <raytracing/shape/interval.h>
#include <raytracing/camera/camera.h>

#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include <cmath>

using std::make_shared;
using std::shared_ptr;

int main() {

    // Create camera object
    camera MyCamera;

    // Set image aspect ratio and width
    MyCamera.setAspect(16.0/9.0);
    MyCamera.setImgWidth(400);
    MyCamera.setPixSamples(100);
    MyCamera.setDepth(50);
    
    // World setup
    shape_list world;

    // Add a sphere and a "plane" to the scene
    world.add(make_shared<sphere>(vec3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(vec3(0,-100.5,-1), 100));

    MyCamera.render(world);

    return 0;
}