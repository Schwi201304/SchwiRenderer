#include <limits>
#include "our_gl.h"
#include "image/image.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
//#include"shader/GouraudShader.h"
#include"shader/PhongShader.h"
using namespace schwi;
using namespace std;

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main() {
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

	// Create window with graphics context
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Schwi Rasterization", NULL, NULL);
	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	Image framebuffer(width, height, Image::RGB); // the output image
	lookat(eye, center, up);                            // build the ModelView matrix
	viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4); // build the Viewport matrix
	projection((eye - center).norm());                    // build the Projection matrix
	std::vector<double> zbuffer(width * height, std::numeric_limits<double>::max());
	//
	//std::string pace = "../obj/african_head/african_head.obj";
	//Model model(pace);
	//PhongShader shader(model);
	//for (int i = 0; i < model.nfaces(); i++) { // for every triangle
	//	vec4 clip_vert[3]; // triangle coordinates (clip coordinates), written by VS, read by FS
	//	for (int j : {0, 1, 2})
	//		shader.vertex(i, j, clip_vert[j]); // call the vertex shader for each triangle vertex
	//	triangle(clip_vert, shader, framebuffer, zbuffer); // actual rasterization routine call
	//}
	//Image输出测试
	//for (int i = 0; i < 100; i++) {
	//	for (int j = 0; j < 200; j++) {
	//		framebuffer.setColor(i, j, Color(255, 0, 0));
	//	}
	//}
	//framebuffer.write_file("framebuffer.png", true); // the vertical flip is moved inside the function
	framebuffer.load_file("framebuffer.png");
	static GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, framebuffer.data());
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		{			
			// //设置窗口的padding为0是图片控件充满窗口
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			//设置窗口的边框大小为0
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
			// 改变窗口的背景颜色
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(120, 143, 100, 255));

			ImGui::Begin("Image_Window", NULL, ImGuiWindowFlags_NoBringToFrontOnFocus);
			ImTextureID image_id = (GLuint*)textureID;

			// 第一个参数：生成的纹理的id
			// 第2个参数：Image的大小
			// 第3，4个参数：UV的起点坐标和终点坐标，UV是被规范化到（0，1）之间的坐标
			// 第5个参数：图片的色调
			// 第6个参数：图片边框的颜色
			ImGui::Image(image_id, ImGui::GetContentRegionAvail(), ImVec2(0.0, 0.0), ImVec2(1.0, 1.0), ImVec4(0, 0, 255, 1), ImVec4(0, 255, 0, 1));
			ImGui::End();

			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(1);
		}
				
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// Show control window.        
		{
			ImGui::Begin("Control Window"); 
			ImGui::Text("Hello from another window!");
			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}