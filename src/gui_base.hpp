#ifndef GUI_BASE_HPP
#define GUI_BASE_HPP

inline void gui_base(vehicle &car){
	ImGui::Text("Base GUI");
	static char car_path[128] = "";
	ImGui::InputTextWithHint("##vehicle.json", "Put vehicle .json filepath here", car_path, IM_ARRAYSIZE(car_path));
	if(ImGui::Button("Load from file")){load_vehicle(car, car_path);}
	ImGui::Text("%.1f", car.fw);
	//if(ImGui::Button("Tire GUI")){current_gui = tire;}
	//ImGui::SameLine();
	//if(ImGui::Button("Yaw Moment GUI")){current_gui = ymd;}
	//ImGui::SameLine();
	//if(ImGui::Button("Laptime Sim GUI")){current_gui = lts;}
}

#endif