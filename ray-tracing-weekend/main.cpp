#include <iostream>
#include <fstream>
#include <cfloat>
#include <stdlib.h>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"


vec3 color(const ray& r, hitable *world) {
    hit_record rec = {};
    if (world->hit(r, 0.0f, FLT_MAX, rec)) {
        return 0.5f*vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
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

    hitable *list[2] = {};
    list[0] = new sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f);
    list[1] = new sphere(vec3(0.0f, -100.5f, -1.0f), 100);
    hitable *world = new hitable_list(list, 2);
    camera cam = {};
    for (int i = ny - 1; i >= 0; i--) {
        for (int j = 0; j < nx; j++) {
            vec3 col(0.0f, 0.0f, 0.0f);
            for (int s = 0; s < ns; s++) {
                float u = float(j + rand()/(float)RAND_MAX) / float(nx);
                float v = float(i + rand()/(float)RAND_MAX) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0f);
                col += color(r, world);
            }
            col /= float(ns);
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            ppm << ir << " " << ig << " " << ib << "\n";
        }
    }
    ppm.close();
}