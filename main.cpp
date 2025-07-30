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
#include "src/obj/misc_classes.hpp"
#include "src/obj/vehicle.hpp"
#include "src/obj/tire_data.hpp"
#include "src/obj/tire_model.hpp"
#include "src/obj/io_flags.hpp"
#include "src/obj/gui_io.hpp"
#include "src/obj/which_gui.hpp"

// Includes - Tire
#include "src/tire/load_dat_drivebrake.hpp"
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
#include "src/gui_base.hpp"
#include "src/check_key_press.hpp"

int main(){
	gui_io io;
  // io.ymdio.update = true;
  // io.ymdio.ax_plot = 1;
	
	start_window();
	while (!glfwWindowShouldClose(graphics_window)) {
		new_frame();
    check_key_press(io);
		switch (current_gui) {
			case base:
				gui_base(io);
				break;
			case tire:
				gui_tire(io);
				break;
			case ymd:
        if (io.ymdio.update){io.car.get_ymd_const_v(io.ymdio);}
        // io.car.p94_s.print();
				gui_ymd(io);
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
