#include <limits>
#include "our_gl.h"
//#include"shader/GouraudShader.h"
#include"shader/PhongShader.h"

using namespace schwi;
using namespace std;
int main() {
	Image framebuffer(width, height, Image::RGB); // the output image
	lookat(eye, center, up);                            // build the ModelView matrix
	viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4); // build the Viewport matrix
	projection((eye - center).norm());                    // build the Projection matrix
	std::vector<double> zbuffer(width * height, std::numeric_limits<double>::max());

	std::string pace = "../obj/african_head/african_head.obj";
	Model model(pace);
	PhongShader shader(model);
	for (int i = 0; i < model.nfaces(); i++) { // for every triangle
		vec4 clip_vert[3]; // triangle coordinates (clip coordinates), written by VS, read by FS
		for (int j : {0, 1, 2})
			shader.vertex(i, j, clip_vert[j]); // call the vertex shader for each triangle vertex
		triangle(clip_vert, shader, framebuffer, zbuffer); // actual rasterization routine call
	}
	//ImageÊä³ö²âÊÔ
	//for (int i = 0; i < 100; i++) {
	//	for (int j = 0; j < 200; j++) {
	//		framebuffer.setColor(i, j, Color(255, 0, 0));
	//	}
	//}
	framebuffer.write_file("framebuffer.png", true); // the vertical flip is moved inside the function

	return 0;
}

