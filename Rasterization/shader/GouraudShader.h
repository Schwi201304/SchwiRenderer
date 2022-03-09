#ifndef GOURAUDSHADER_H
#define GOURAUDSHADER_H

#include"IShader.h"
#include "../model.h"
#include "../our_gl.h"
namespace schwi {
	class GouraudShader : public IShader {
	public:
		const Model& model;

        mat<2, 3> varying_uv;  // same as above
        mat<4, 4> uniform_M;   //  Projection*ModelView
        mat<4, 4> uniform_MIT; // (Projection*ModelView).invert_transpose()
		GouraudShader(const Model& m) : model(m) {
            uniform_M = Projection * ModelView;
            uniform_MIT = (Projection * ModelView).invert_transpose();
        }


        virtual void vertex(int iface, int nthvert, vec4& gl_Position) {
            varying_uv.set_col(nthvert, model.uv(iface, nthvert));
            vec4 gl_Vertex = embed<4>(model.vert(iface, nthvert)); // read the vertex from .obj file
            gl_Position=Projection * ModelView * gl_Vertex; // transform it to screen coordinates
        }

        virtual bool fragment(const vec3 bar, Color& color) {
            vec2 uv = varying_uv * bar;                 // interpolate uv for the current pixel
            vec3 n = proj<3>(uniform_MIT * embed<4>(model.normal(uv))).normalize();
            vec3 l = proj<3>(uniform_M * embed<4>(light_dir)).normalize();
            double intensity = std::max(.0, n * l);
            //if(intensity!=0)std::cout << intensity << std::endl;
            color = sample2D(model.diffuse(), uv) *intensity;
            return false;                              // no, we do not discard this pixel
            //vec2 uv = varying_uv * bar;
            //vec3 n = proj<3>(uniform_MIT * embed<4>(model.normal(uv))).normalize();
            //vec3 l = proj<3>(uniform_M * embed<4>(light_dir)).normalize();
            ////for (int i = 0; i < 3; i++) {
            ////    std::cout << n[i] << " " << l[i] << std::endl;
            ////}
            //vec3 r = (n * (n * l * 2.f) - l).normalize();   // reflected light
            //double spec = pow(std::max(r.z, .0), sample2D(model.specular(),uv)[0]);
            //double diff = std::max(.0, n * l);
            //Color c = sample2D(model.diffuse(),uv);
            //color = c;
            //for (int i = 0; i < 3; i++) color[i] = std::min<BYTE>(5 + c[i] * (diff + 0.6 * spec), 255);
            //return false;                            // no, we do not discard this pixel
        }
	};
}
#endif
