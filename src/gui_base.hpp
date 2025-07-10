#ifndef GUI_BASE_HPP
#define GUI_BASE_HPP

inline void gui_base(vehicle &car, tire_data &td){
	
	ImGui::Text("Base GUI");
	
	static char car_path[128] = "";
	ImGui::InputTextWithHint("##vehicle.json", "Put vehicle .json filepath here", car_path, IM_ARRAYSIZE(car_path));
	if(ImGui::Button("Load from file##car")){load_vehicle(car, car_path);}
	ImGui::Text("%.1f", car.fw);
	
	static char tc_path[128] = "";
	ImGui::InputTextWithHint("##crun.dat", "Put cornering run.dat filepath here", tc_path, IM_ARRAYSIZE(tc_path));
	if(ImGui::Button("Load from file##tc")){load_dat_cornering(td, tc_path, 82.7);}
	if (td.c_ia(0, 0).n_elem)
		ImGui::Text("%.1f", td.c_ia(0, 2)(0));	
	else
		ImGui::Text("n/a");
	
	static char td_path[128] = "";
	ImGui::InputTextWithHint("##dbrun.dat", "Put drive/brake run.dat filepath here", td_path, IM_ARRAYSIZE(td_path));
	if(ImGui::Button("Load from file##tdb")){load_dat_drivebrake(td, td_path, 82.7);}
	if (td.db_ia(0, 0).n_elem)
		ImGui::Text("%.1f", td.db_ia(0, 2)(0));	
	else
		ImGui::Text("n/a");
	//if(ImGui::Button("Tire GUI")){current_gui = tire;}
	//ImGui::SameLine();
	//if(ImGui::Button("Yaw Moment GUI")){current_gui = ymd;}
	//ImGui::SameLine();
	//if(ImGui::Button("Laptime Sim GUI")){current_gui = lts;}
}

#endif