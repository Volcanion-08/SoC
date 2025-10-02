    #ifndef CAMERA_H
#define CAMERA_H

#include "general.h"
#include "worldtype.h"
#include "interval.h"
#include "random.h"
#include "hitlist.h"
#include "scatter.h"
#include <random>

struct Camera{
    float aspect_ratio = 1.0f; // width/height
    int image_height = 100;
    int samples_per_pixel = 10;   // Count of random samples for each pixel
    int max_depth = 5;

    float vfov = 90;                 
    Point lookfrom = Point(0,0,0);  
    Point lookat   = Point(0,0,-1);  
    Vec3   vup      = Vec3(0,1,0);   

    float defocus_angle = 0.0f; 
    float focus_dist = 10.0; 

    int image_width; 
    Point camera_center; 
    Point initpixel; 
    Vec3 delx; 
    Vec3 dely; 
    Vec3 defocus_disk_u;  
    Vec3 defocus_disk_v;       
};


void initialise(Camera& cam){
    cam.image_width = int(400 * cam.aspect_ratio);

    // Camera dimensions
    auto theta = degrees_to_radians(cam.vfov);
    auto h = std::tan(theta/2);
    auto viewport_height = 2 * h * cam.focus_dist;
    auto viewport_width = viewport_height*cam.aspect_ratio;
    cam.camera_center = cam.lookfrom;

    auto camfor = unit_vector(cam.lookfrom-cam.lookat);
    auto camx = cross(cam.vup,camfor);
    auto camy = cross(camfor,camx);

    // vectors along viewport axes
    Vec3 viewport_x = viewport_width * camx; 
    Vec3 viewport_y = viewport_height * -camy;

    // vectors to the next pixel
    cam.delx = viewport_x/cam.image_width;
    cam.dely = viewport_y/cam.image_height;

    Point viewportupperleft = cam.camera_center - cam.focus_dist * camfor - viewport_x/2 - viewport_y/2;
    cam.initpixel = viewportupperleft + cam.delx/2 + cam.dely/2;

    auto defocus_radius = cam.focus_dist * std::tan(degrees_to_radians(cam.defocus_angle / 2));
    cam.defocus_disk_u = camx * defocus_radius;
    cam.defocus_disk_v = camy * defocus_radius;

}

inline bool hit_checker(const Ray& r,const WorldObject* world, int world_size,hit_record& rec){
    float t_min = 0.001f;
    float t_max = INFINITY; // hello
    float closest_t = t_max;
    bool hit_anything = false;
    for(int i=0;i<world_size;i++){
        const WorldObject& obj = world[i];
        interval range = interval(t_min,closest_t);
        if (obj.hit(r, range, rec)){
            closest_t = rec.t;
            hit_anything = true;
        }
    }
    return hit_anything;
}


inline Color ray_color(const Ray& r, int max_depth, const WorldObject* world, int world_size, xorwow_state_t& rng) { 
    Ray curray = r;
    Color netColor(1.0, 1.0, 1.0);
    for (int i = 0; i < max_depth; ++i) {
        hit_record rec;
        if (hit_checker(curray, world,world_size, rec)) {
            Ray scattered;
            Color attenuation;
            if (scatter(curray, rec, attenuation, scattered,rng)) {
                netColor = netColor * attenuation;
                curray = scattered;
            } else {
                netColor = Color(0, 0, 0);
                break;
            }
        } else {
            Vec3 unit_direction = unit_vector(curray.direction());
            auto t = 0.5 * (unit_direction[1] + 1.0);
            Color background = (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
            netColor = netColor * background;
            break;
        }
    }
    return netColor;    
}



inline Vec3 sample_square(xorwow_state_t& rng){
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
    return Vec3(random_float(rng) - 0.5, random_float(rng) - 0.5, 0);
}

inline Point defocus_disk_sample(xorwow_state_t& rng,Camera& cam){
    // Returns a random point in the camera defocus disk.
    auto p = random_in_unitdisk(rng);
    return cam.camera_center + (p[0] * cam.defocus_disk_u) + (p[1] * cam.defocus_disk_v);
}

inline Ray getray(int i,int j, xorwow_state_t& rng,Camera& cam){
    auto offset = sample_square(rng);
    
    auto pixel_sample = cam.initpixel + ((i+offset[0]) * cam.delx) + ((j+offset[1]) * cam.dely);
    auto ray_origin = (cam.defocus_angle <= 0) ? cam.camera_center : defocus_disk_sample(rng,cam);
    auto ray_direction = pixel_sample - ray_origin;

    Ray r(ray_origin, ray_direction);
    return r;
}




void render(Camera& cam,std::vector<WorldObject>& world){
    initialise(cam);
    int image_width = cam.image_width;
    int image_height = cam.image_height;

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    size_t N = image_height * image_width; 
    std::vector<xorwow_state_t> host_rngs(N);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist;

    for (size_t i = 0; i < N; ++i) {
        xorwow_state_t state;
        state.x = dist(gen);
        state.y = dist(gen);
        state.z = dist(gen);
        state.w = dist(gen);
        state.v = dist(gen);
        state.d = 362437;
        host_rngs[i] = state;
    }

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            Color pixel_color = Color(0.0f,0.0f,0.0f);
            for(int k = 0;k<cam.samples_per_pixel;k++){
                Ray r = getray(i,j,host_rngs[i+j*image_width],cam);
                Color sample_pixel_color = ray_color(r,cam.max_depth,world.data(),world.size(),host_rngs[i+j*image_width]); // what is the color gonna be from this ray
                pixel_color = pixel_color + sample_pixel_color;
            }
            pixel_color = pixel_color / cam.samples_per_pixel;
            write_color(std::cout, pixel_color);
        }
    }
}



#endif