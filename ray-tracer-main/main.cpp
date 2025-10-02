#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <iostream>
#include <vector>
#include <random>

int main(){

    std::vector<WorldObject> world;
    //ground
    Material material_ground = Lambertian(Color(0.5, 0.5, 0.5));
    world.push_back(WorldObject(Sphere(Point(0.0f, -1000.0f, 0.0f), 1000.0f, material_ground)));
    xorwow_state_t rng = make_random_state();
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = random_float(rng);
            Point center(a + 0.9f * random_float(rng), 0.2f, b + 0.9f * random_float(rng));
            if ((center - Point(4, 0.2, 0)).length() > 0.9f) {
                Material sphere_material;
                if (choose_mat < 0.8f) {
                    // Diffuse
                    Color albedo = randomvec(0.0f,1.0f,rng) * randomvec(0.0f,1.0f,rng);
                    sphere_material = Lambertian(albedo);
                } else if (choose_mat < 0.95f) {
                    // Metal
                    Color albedo = randomvec(0.5f, 1.0f,rng);
                    float fuzz = random_float(0.0f, 0.5f,rng);
                    sphere_material = Metal(albedo, fuzz);
                } else {
                    // Dielectric (glass)
                    sphere_material = Dielectric(1.5f);
                }
                world.push_back(WorldObject(Sphere(center, 0.2f, sphere_material)));
            }
        }
    }

    Material material1 = Dielectric(1.5f);
    world.push_back(WorldObject(Sphere(Point(0, 1, 0), 1.0f, material1)));

    Material material2 = Lambertian(Color(0.4f, 0.2f, 0.1f));
    world.push_back(WorldObject(Sphere(Point(-4, 1, 0), 1.0f, material2)));

    Material material3 = Metal(Color(0.7f, 0.6f, 0.5f), 0.0f);
    world.push_back(WorldObject(Sphere(Point(4, 1, 0), 1.0f, material3)));


    // camera
    Camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_height       = 400;
    cam.samples_per_pixel = 40;
    cam.max_depth         = 10;

    cam.vfov     = 20;
    cam.lookfrom = Point(13,2,3);
    cam.lookat   = Point(0,0,0);
    cam.vup      = Vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;
    render(cam,world);
}