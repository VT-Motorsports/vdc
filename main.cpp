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
#include "src/which_gui.hpp"
#include "src/gui_base.hpp"
#include "src/tire/gui_tire.hpp"
#include "src/ymd/gui_ymd.hpp"
#include "src/lts/gui_lts.hpp"
		
int main(){
	start_window();
	while (!glfwWindowShouldClose(graphics_window)) {
		new_frame();
		switch (current_gui) {
			case base:
				gui_base();
				break;
			case tire:
				gui_tire();
				break;
			case ymd:
				gui_ymd();
				break;
			case lts:
				gui_lts();
				break;
		}
		render_frame();
	}
	end_window();
	return 0;
}