#ifndef GUI_YMD_HPP
#define GUI_YMD_HPP

inline void gui_ymd(){
	ImGui::Text("YMD GUI");
	if(ImGui::Button("Cycle GUI")){current_gui = lts;}
}

#endif