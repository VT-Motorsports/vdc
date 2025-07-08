#ifndef GUI_TIRE_HPP
#define GUI_TIRE_HPP

inline void gui_tire(){
	ImGui::Text("Tire GUI");
	if(ImGui::Button("Cycle GUI")){current_gui = ymd;}
}

#endif