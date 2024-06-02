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
    auto m_yellow = make_shared<lambertian>(color(0.6, 0.6, 0.0));
    auto m_red = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    auto m_dielectric = make_shared<dielectric>(0.4);
    auto m_metal = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100, m_yellow));
    world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5, m_red));
    world.add(make_shared<Sphere>(point3(-1.0, 0, -1), 0.5, m_dielectric));
    world.add(make_shared<Sphere>(point3(1.0, 0, -1), 0.5, m_metal));

    // Camera
    Camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 800;
    cam.samples_per_pixel = 30;
    cam.max_depth = 10;
    cam.render(world, "initial_image_fhd1");
    m_dielectric = make_shared<dielectric>(0.5);
    cam.render(world, "initial_image_fhd2");
    m_dielectric = make_shared<dielectric>(0.6);
    cam.render(world, "initial_image_fhd3");
    m_dielectric = make_shared<dielectric>(0.7);
    cam.render(world, "initial_image_fhd4");
    m_dielectric = make_shared<dielectric>(0.8);
    cam.render(world, "initial_image_fhd8");
    m_dielectric = make_shared<dielectric>(1.1);
    cam.render(world, "initial_image_fhd5");
    m_dielectric = make_shared<dielectric>(1.2);
    cam.render(world, "initial_image_fhd6");
    m_dielectric = make_shared<dielectric>(2);
    cam.render(world, "initial_image_fhd7");
    m_dielectric = make_shared<dielectric>(2.1);
    return 0;
}
