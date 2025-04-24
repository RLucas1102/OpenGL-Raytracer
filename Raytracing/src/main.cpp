#include <raytracing/vec/vec3.h>
#include <raytracing/vec/ray.h>
#include <raytracing/shape/sphere.h>
#include <raytracing/shape/shape.h>
#include <raytracing/shape/shape_list.h>
#include <raytracing/shape/interval.h>
#include <raytracing/camera/camera.h>
#include <raytracing/materials/material.h>

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

    // Materials
    shared_ptr<lambertian> mat_ground = make_shared<lambertian>(vec3(0.8, 0.8, 0.0));
    shared_ptr<lambertian> mat_ball = make_shared<lambertian>(vec3(0.1, 0.2, 0.5));
    shared_ptr<dielectric> mat_glassBall = make_shared<dielectric>(1.50);
    shared_ptr<dielectric> mat_airBall = make_shared<dielectric>(1.00 / 1.50);
    shared_ptr<metal> mat_metalBall2 = make_shared<metal>(vec3(0.8, 0.6, 0.2), 1.0);

    // Add a sphere and a "plane" to the scene
    world.add(make_shared<sphere>(vec3(0,0,-1.2), 0.5, mat_ball));
    world.add(make_shared<sphere>(vec3(0,-100.5,-1), 100, mat_ground));
    world.add(make_shared<sphere>(vec3(-1.0, 0.0, -1.0), 0.5, mat_glassBall));
    world.add(make_shared<sphere>(vec3(-1.0, 0.0, -1.0), 0.4, mat_airBall));
    world.add(make_shared<sphere>(vec3(1.0, 0.0, -1.0), 0.5, mat_metalBall2));

    MyCamera.render(world);

    return 0;
}