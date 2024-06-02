#include "utility.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "color.h"
#include "camera.h"
#include <fstream>

int main() {
    // World
    HittableList world;
    //world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100));

    // Camera
/*    Camera cam;
    cam.aspect_ratio = 16.0 / 2.0; 
    cam.image_width = 400; 
    cam.samples_per_pixel = 1;
    cam.render(world, "sa");*/ // 2 files will be generated, bmp and ppm. 
    // You can use online PPM viewers
    // Or directly open bmp using windows photo or most ide

    Camera cam;
    cam.aspect_ratio = 1.0 / 1.0;
    cam.image_width = 300;
    cam.samples_per_pixel = 100;

    world.add(make_shared<Sphere>(point3(3, 0, -1), 0.7));
    world.add(make_shared<Sphere>(point3(-3, 0, -3), 0.7));
    world.add(make_shared<Sphere>(point3(0, 0, -1), 0.7));
    //cam.render(world, "image_initial");

    //cam.move(vec3(-2, 0, 0));
    cam.render(world, "image_moved");

    cam.lookat = point3(0, 0, -10);
    cam.render(world, "image_moved");



    return 0;
}
