#pragma once
#include "ray.h"

const float PI = 3.1415f;

vec3 randon_in_unit_disk() {
    vec3 p = {};
    do {
        p = 2.0f*vec3(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, 0.0f) - vec3(1.0f, 1.0f, 0.0f);
    } while (dot(p, p) >= 1.0f);
    return p;
}

class camera {
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist) { //vfov is the top to bottom in degrees (vertical field of view)
        lens_radius = aperture / 2;
        float theta = vfov * PI / 180.0f;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist* v - focus_dist * w;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height* focus_dist * v;
    }
    ray get_ray(float s, float t) {
        vec3 rd = lens_radius * randon_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};