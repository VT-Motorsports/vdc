#ifndef GUI_BASE_HPP
#define GUI_BASE_HPP

inline void gui_base(){
	ImGui::Text("Base GUI");
	if(ImGui::Button("Cycle GUI")){current_gui = tire;}
}

#endif