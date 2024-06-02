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
    world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100));

    // Camera
    Camera cam;
    cam.aspect_ratio = 16.0 / 9.0; 
    cam.image_width = 1000; 
    cam.render(world, "outputsss.ppm");

    return 0;
}
