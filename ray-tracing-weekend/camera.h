#pragma once
#include "ray.h"

const float PI = 3.1415f;

class camera {
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) { //vfov is the top to bottom in degrees (vertical field of view)
        vec3 u, v, w = {};
        float theta = vfov * PI / 180.0f;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = origin - half_width * u - half_height * v - w;
        horizontal = 2 * half_width*u;
        vertical = 2 * half_height*v;
    }
    ray get_ray(float u, float v) {
        return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
    }
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};