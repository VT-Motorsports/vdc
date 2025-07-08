#ifndef GUI_LTS_HPP
#define GUI_LTS_HPP

inline void gui_lts(){
	ImGui::Text("LTS GUI");
	if(ImGui::Button("Cycle GUI")){current_gui = base;}
}

#endif