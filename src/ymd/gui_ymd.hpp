#ifndef GUI_YMD_HPP
#define GUI_YMD_HPP

inline void gui_ymd_controls(){
  ImVec2 sz = ImVec2(325, ImGui::GetContentRegionAvail().y);
  ImGui::BeginChild("Controls", sz, true);
  ImGui::EndChild();
  ImGui::SameLine();
}

inline void gui_ymd_plot(){
  static int tt_ht = 40;
  ImVec2 sz = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - tt_ht);
  ImGui::BeginGroup();
  ImGui::BeginChild("Plot", sz, true);
  ImGui::EndChild();
}

inline void gui_ymd_hints(){
  static int ind = 30;
  // ImVec2 sz = ImVec2(ImGui::GetContentRegionAvail().x, tt_ht);
  ImVec2 sz = ImGui::GetContentRegionAvail();

  ImGui::BeginChild("Hints", ImGui::GetContentRegionAvail(), true);
  ImGui::Text("None: Basic");
  ImGui::SameLine(0, ind); 
  ImGui::Text("Shift: Tires");
  ImGui::SameLine(0, ind); 
  ImGui::Text("Alt: Forces");
  ImGui::SameLine(0, ind); 
  ImGui::Text("Ctrl: Alignment");
  ImGui::SameLine(0, ind); 
  ImGui::Text("CtrlAlt: N/A");
  ImGui::SameLine(0, ind); 
  ImGui::Text("CtrlShift: N/A");
  ImGui::EndChild();
  ImGui::EndGroup();
}

inline void gui_ymd(gui_io &io){
  // ImGui::Text("Hello :)");
  // Left Section: Controls
  gui_ymd_controls();
  // Right Section: Plots
  gui_ymd_plot();
  // Below Right Section: Keybinds
  gui_ymd_hints();
}

#endif
