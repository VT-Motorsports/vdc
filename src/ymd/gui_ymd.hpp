#ifndef GUI_YMD_HPP
#define GUI_YMD_HPP

inline void gui_ymd_controls(){
  static int tt_ht = 40;
  ImVec2 sz = ImVec2(325, ImGui::GetContentRegionAvail().y - tt_ht);
  ImGui::BeginChild("Controls", sz, true);
  ImGui::EndChild();
  ImGui::SameLine();
}

inline void gui_ymd_plot(){
  static int tt_ht = 40;
  ImVec2 sz = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - tt_ht);
  ImGui::BeginChild("Plot", sz, true);
  ImGui::EndChild();
}

inline void gui_ymd_hints(){
  int ind = ImGui::GetContentRegionAvail().x / 5 - 182;
  // ImVec2 sz = ImVec2(ImGui::GetContentRegionAvail().x, tt_ht);
  ImVec2 sz = ImGui::GetContentRegionAvail();
  static Col<string> tt_label = {
    "None: Dynamics",
    "Ctrl: Tires/TV",
    "Shift: Forces",
    "Alt: Alignment",
    "CtrlShift: Forces+",
    "CtrlAlt: Alignment+"
  } 
  int key_held;
  if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_LeftAlt)){
    key_held = 5;
  }
  else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_LeftShift)){
    key_held = 4;
  }
  else if (ImGui::IsKeyDown(ImGuiKey_LeftAlt)){
    key_held = 3;
  }
  else if (ImGui::IsKeyDown(ImGuiKey_LeftShift)){
    key_held = 2;
  }
  else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)){
    key_held = 1;
  }
  else {
    key_held = 0;
  }

  ImGui::BeginChild("Hints", ImGui::GetContentRegionAvail(), true);
  if () 
    ImGui::Text(tt_label(0));
  else 
    ImGui::Text(tt_label(0));
  ImGui::SameLine(0, ind); 
  ImGui::SameLine(0, ind); 
  ImGui::SameLine(0, ind); 
  ImGui::SameLine(0, ind); 
  ImGui::SameLine(0, ind); 
  ImGui::EndChild();
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
