#include "utility.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "color.h"
#include "material.h"
#include "sphere.h"
#include <fstream>

int main() {
    // World
    HittableList world;
    //world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
    //world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100));

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8), 0.3); //add another number at end for fuzziness ((x,y,z), fuzz_mutlip)
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.6); //add another number at end for fuzziness ((x,y,z), fuzz_mutlip)

    world.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera
    Camera cam;
    cam.aspect_ratio = 16.0 / 9.0; 
    cam.image_width = 400; 
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.render(world, "2_metal_shiny.ppm");

    return 0;
}
