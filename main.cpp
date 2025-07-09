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
#include <nlohmann/json.hpp>
using namespace arma;
using json = nlohmann::json;

// Includes - Objects
//#include "src/obj/fx.hpp"
//#include "src/obj/fy.hpp"
#include "src/obj/fz.hpp"
#include "src/obj/vehicle.hpp"
#include "src/obj/tire_data.hpp"

// Includes - Tire
#include "src/tire/load_dat_cornering.hpp"
#include "src/tire/gui_tire.hpp"

// Includes - YMD
#include "src/ymd/load_json_file.hpp"
#include "src/ymd/load_vehicle.hpp"
#include "src/ymd/gui_ymd.hpp"
#include "src/gui_base.hpp"

// Includes - LTS
#include "src/lts/gui_lts.hpp"

// Includes - Common
#include "src/gui_backend.hpp"
#include "src/which_gui.hpp"
#include "src/gui_base.hpp"

int main(){
	vehicle car;
	tire_data td;
	start_window();
	while (!glfwWindowShouldClose(graphics_window)) {
		new_frame();
		switch (current_gui) {
			case base:
				gui_base(car, td);
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