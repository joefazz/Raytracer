//
//  main.cpp
//  Raytracer
//
//  Created by Joe Fazzino on 05/07/2021.
//

#include "rtweekend.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include "material_instances.hpp"
#include <chrono>

#include <iostream>

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;
    
    // No infinite loops pls
    if (depth <= 0) {
        return color(0, 0, 0);
    }
    
    // 0.001 is treatment for shadow acne
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return color(0, 0, 0); // Changing this actually changes the color of the shadows which is fun
    }
    
    // Paint Background
    vec3 unit_direction = unit_vector(r.direction());
    
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

/*
 Okay so what is happening in this whole shebang to my understanding is...
 We started drawing every pixel on the screen by just going one by one and then saying "you are this colour"
 made it a lovely lerpy blue blackground which you can see here.
 ```
 vec3 unit_direction = unit_vector(r.direction());
 
 auto t = 0.5 * (unit_direction.y() + 1.0);
 return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
 ```
 
 After that was working we added the concept of hittable things so that the ray could actually
 intersect with things in the world, hitable things contain information (in the hit record) that is
 written to by any object we want to be in the world, importantly we get the point in the object that was hit
 and the normal of that point.
 

 */

hittable_list random_scene() {
    hittable_list world;
    
    vec3 colors[] = {
                     vec3(187, 139, 65), // Gold
                     vec3(255, 102, 94), // Red
                     vec3(210, 81, 94), // Marine Red
                     vec3(98, 168, 229), // Cornflower
                     vec3(35, 91, 168), // Lake
                     vec3(22, 155, 98), // Ivy
                     vec3(246, 160, 77) // Apricot
    };
    
    auto ground_mat = make_shared<lambertian>(color(0.5, 0.5 , 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_mat));
    
    int limiter = -12;
    
    for (int a = limiter; a < fabs(limiter); a++) {
        for (int b = limiter; b < fabs(limiter); b++) {
            auto choose_mat = random_double();
            auto rand_radius = random_double(0.1,0.25);
            point3 center(a + 0.9 * random_double(), rand_radius, b + 0.9*random_double());
            
            if ((center - point3(4, rand_radius, 0)).length() > 0.9) {
                shared_ptr<material> sphere_mat;
                
                if (choose_mat < 0.7) {
                    // diffuse
                    auto albedo = colors[rand() %  7] / 255  ;
                    sphere_mat = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, rand_radius, sphere_mat));
                } else if (choose_mat < 0.9) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_mat = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, rand_radius, sphere_mat));
                } else {
                    // glass
                    sphere_mat = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, rand_radius, sphere_mat));
                }
            }
        }
    }
    
    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
    
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
    
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    
    return world;
}

void set_flag(int* param, char* arg) {
    std::string sval = arg;
    *param = std::stoi(sval);
}

int main(int argc, char** argv) {
    srand(300);
    
    // Image
     auto aspect_ratio = 4.0 / 3.0;
     int image_width = 1080;
     int image_height = static_cast<int>(image_width / aspect_ratio);
     int samples_per_pixel = 50;
    const int max_depth = 50;
    
//    for (int i = 0; i < argc; i++) {
//        std::cerr << argv[i] << std::endl;
//    }
    
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--width") == 0) {
            set_flag(&image_width, argv[i+1]);
            std::cerr << "Width set to: " << image_width << std::endl;
            continue;
        }
        
        if (strcmp(argv[i], "--height") == 0) {
            set_flag(&image_height, argv[i+1]);
            std::cerr << "Height set to: " << image_height << std::endl;
            
            aspect_ratio = image_height / image_width;
            continue;
        }
        
        if (strcmp(argv[i], "--samples") == 0) {
            set_flag(&samples_per_pixel, argv[i+1]);
            std::cerr << "Samples per pixel: " << samples_per_pixel << std::endl;
            continue;
        }
        
        if (strcmp(argv[i], "--seed") == 0) {
            std::string arg = argv[i+1];
            srand(std::stoi(arg));
            std::cerr << "Rando seed set" << std::endl;
            continue;
        }
    }
    
    // World
    hittable_list world;
    
//    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
//    auto material_center = make_shared<lambertian>(color(0.0, 0.8, 0.4));
//    auto material_left = make_shared<dielectric>(1.5);
//    auto material_right = make_shared<metal>(color(1, 1, 1), 0);
//
//    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));
//    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, material_left));
//    world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, material_right));
//    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, material_center));
    
    world = random_scene();
    
    // Camera
    point3 lookfrom(13,2,0.2);
    point3 lookat(0,0,4);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    
    auto start_time = std::chrono::system_clock::now();
    
    for (int j = image_height - 1; j >= 0; --j) {
        
        if (j % 10 == 0) {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        }
        
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    
    std::chrono::seconds diff = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start_time);
    
    std::cerr << "\nDone!\n";
    std::cerr << diff.count() / 60 << " minutes " << diff.count() % 60 << " seconds to complete render!" << std::endl;
}
