#ifndef GUI_BASE_HPP
#define GUI_BASE_HPP

inline void gui_base(){
	ImGui::Text("Base GUI");
	if(ImGui::Button("Tire GUI")){current_gui = tire;}
	ImGui::SameLine();
	if(ImGui::Button("Yaw Moment GUI")){current_gui = ymd;}
	ImGui::SameLine();
	if(ImGui::Button("Laptime Sim GUI")){current_gui = lts;}
}

#endif