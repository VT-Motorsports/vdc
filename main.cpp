// C++ Standard Libraries
#include <iostream>
#include <string>
using namespace std;

// External Libraries
#define ARMA_DONT_USE_BLAS
#include <armadillo>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <implot.h>
using namespace arma;

// Includes
#include "src/gui_backend.hpp"

int main(){
	start_window();
	while (!glfwWindowShouldClose(graphics_window)) {
		new_frame();
		// GUI here
		render_frame();
	}
	end_window();
	return 0;
}