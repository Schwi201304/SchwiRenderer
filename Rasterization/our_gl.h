#ifndef OUR_GL_H
#define OUR_GL_H

#include "image/image.h"
#include "geometry/geometry.h"
#include"shader/IShader.h"

namespace schwi {
    constexpr int width = 800; // output image size
    constexpr int height = 800;

    const vec3 light_dir(1, 1, 1); // light source
    const vec3       eye(1, 1, 3); // camera position
    const vec3    center(0, 0, 0); // camera direction
    const vec3        up(0, 1, 0); // camera up vector

    extern mat<4, 4> ModelView; // "OpenGL" state matrices
    extern mat<4, 4> Viewport;//Screen
    extern mat<4, 4> Projection;

    void viewport(const int x, const int y, const int w, const int h);
    void projection(const double coeff = 0); // coeff = -1/c
    void lookat(const vec3 eye, const vec3 center, const vec3 up);

    
    void triangle(const vec4 clip_verts[3], IShader& shader, Image& image, std::vector<double>& zbuffer);
}
#endif