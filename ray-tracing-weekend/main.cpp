#include <iostream>
#include <fstream>
#include <cfloat>
#include <stdlib.h>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001f, FLT_MAX, rec)) {
        ray scattered = {};
        vec3 attenuation = {};
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return vec3(0.0f, 0.0f, 0.0f);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5f * (unit_direction.y() + 1.0f);
        return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
    }
}

int main() {
    int nx = 200;
    int ny = 100;
    int ns = 100;
    std::ofstream ppm;
    ppm.open("image.ppm");
    ppm << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *list[5] = {};
    //float R = cos(PI / 4.0f);
    list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, new lambertian(vec3(0.1f, 0.2f, 0.5f)));
    list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, new lambertian(vec3(0.8f, 0.8f, 0.0f)));
    list[2] = new sphere(vec3(1.0f, 0.0f, -1.0f), 0.5f, new metal(vec3(0.8f, 0.6f, 0.2f), 0.6f));
    list[3] = new sphere(vec3(-1.0f, 0.0f, -1.0f), 0.5f, new dielectric(1.5f));
    list[4] = new sphere(vec3(-1.0f, 0.0f, -1.0f), -0.45f, new dielectric(1.5f));
    hitable *world = new hitable_list(list, 5);
    camera cam(vec3(-2.0f, 2.0f, 1.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), 50.0f, float(nx) / float(ny));
    for (int i = ny - 1; i >= 0; i--) {
        for (int j = 0; j < nx; j++) {
            vec3 col(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < ns; s++) {
                float u = float(j + rand()/(float)RAND_MAX) / float(nx);
                float v = float(i + rand()/(float)RAND_MAX) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0f);
                col += color(r, world, 0);
            }
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            ppm << ir << " " << ig << " " << ib << "\n";
        }
    }
    ppm.close();
}