#ifndef GUI_BASE_HPP
#define GUI_BASE_HPP

inline void gui_base(gui_io &io){
	
	ImGui::Text("Base GUI");
	
	static char car_path[128] = "";
	ImGui::InputTextWithHint("##vehicle.json", "Put vehicle .json filepath here", car_path, IM_ARRAYSIZE(car_path));
	if(ImGui::Button("Load from file##car")){load_vehicle(io.car, car_path);}
	ImGui::Text("%.1f", io.car.fw);
	
	static char tc_path[128] = "";
	ImGui::InputTextWithHint("##crun.dat", "Put cornering run.dat filepath here", tc_path, IM_ARRAYSIZE(tc_path));
	if(ImGui::Button("Load from file##tc")){load_dat_cornering(io.td, tc_path, 82.7);}
	if (io.td.c_ia(0, 0).n_elem) {
		ImGui::Text("%.1f", io.td.c_ia(0, 2)(0));
		io.flags.is_tc_loaded = true;	
	}
	else
		ImGui::Text("n/a");
	
	static char td_path[128] = "";
	ImGui::InputTextWithHint("##dbrun.dat", "Put drive/brake run.dat filepath here", td_path, IM_ARRAYSIZE(td_path));
	if(ImGui::Button("Load from file##tdb")){load_dat_drivebrake(io.td, td_path, 82.7);}
	if (io.td.db_ia(0, 0).n_elem) {
		ImGui::Text("%.1f", io.td.db_ia(0, 2)(0));
		io.flags.is_tdb_loaded = true;
	}
	else
		ImGui::Text("n/a");
	
	if (!(io.flags.is_tc_loaded && io.flags.is_tdb_loaded)){ImGui::BeginDisabled();}
	if(ImGui::Button("Tire GUI")){current_gui = tire;}
	if (!(io.flags.is_tc_loaded && io.flags.is_tdb_loaded)){ImGui::EndDisabled();}
	//ImGui::SameLine();
	//if(ImGui::Button("Yaw Moment GUI")){current_gui = ymd;}
	//ImGui::SameLine();
	//if(ImGui::Button("Laptime Sim GUI")){current_gui = lts;}
}

#endif