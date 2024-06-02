// In real world, ray sources are lights. However, since not all of the light source rays will hit the camera, 
//      we do not need to use such a complicated and expensive approach.

// Instead, we use camera as the ray source. For each pixel, a ray is casted from the camera to the pixel's center. 

#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"
#include "hittable.h"
#include <iostream>
#include <fstream>

class Camera {
public:
    double aspect_ratio = 16.0 / 9.0;  
    int image_width = 400;   
    int samples_per_pixel = 100; //antialising

    void render(const Hittable& world, const std::string& file_name) {
            
        auto file_ppm = file_name + ".ppm";
        auto file_bmp = file_name + ".bmp";


        initialize();

// Output to //Debug/output.ppm
// use https://www.cs.rhodes.edu/welshc/COMP141_F16/ppmReader.html to view the output image

        std::ofstream outfile(file_ppm);
        if (!outfile.is_open()) {
            std::cerr << "Failed to open output file." << std::endl;
            return;
        }

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        outfile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        // Render pixels
        for (int j = image_height - 1; j >= 0; --j) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                
                color pixel_color(0, 0, 0);

                for (int s = 0; s < samples_per_pixel; ++s) {

                    auto u = (i + random_double()) / (image_width - 1);
                    auto v = (j + random_double()) / (image_height - 1);
                    ray r = get_ray(u, v);
                    pixel_color += ray_color(r, world);

                }

                pixel_color /= static_cast<double>(samples_per_pixel);

                //write_color(std::cout, pixel_color);
                write_color(outfile, pixel_color);
            }
        }

        std::clog << "\rDone.                 \n";
        outfile.close();
        
        convertPPMtoBMP(file_ppm, file_bmp);
        openBMPFile(file_bmp);
    }

private:
    int image_height;
    point3 origin;      
    point3 lower_left_corner; 
    vec3 horizontal;    
    vec3 vertical;      

    void initialize() {
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin = point3(0, 0, 0);
        horizontal = vec3(viewport_width, 0.0, 0.0);
        vertical = vec3(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
    }

    ray get_ray(double u, double v) const {
        return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }

    color ray_color(const ray& r, const Hittable& world) const {
        hit_record rec;
        if (world.hit(r, 0, inf, rec)) {
            return 0.5 * (rec.normal + color(1, 1, 1));
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
    }

    void write_color(std::ostream& out, color pixel_color) const {
        out << static_cast<int>(255.999 * pixel_color.x()) << ' '
            << static_cast<int>(255.999 * pixel_color.y()) << ' '
            << static_cast<int>(255.999 * pixel_color.z()) << '\n';
    }
};

#endif
