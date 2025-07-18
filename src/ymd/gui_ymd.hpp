#ifndef GUI_YMD_HPP
#define GUI_YMD_HPP

inline void gui_ymd(gui_io &io){
  ImGui::Dummy(ImVec2(437, 178));
  ImGui::Dummy(ImVec2(437, 0));
  ImGui::SameLine(0);
	ImGui::Text(
    "  ___  _____  _____    ___  ___   ________ \n"
    "  | | / / _ \\/ ___/__ / _ )/ _ | / __/ __/\n"
    "  | |/ / // / /__ /_// _  / __ |_\\ \\/ _/ \n"
    "  |___/____/\\___/   /____/_/ |_|___/___/    "
    );
}

#endif
