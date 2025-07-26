#ifndef GUI_YMD_HPP
#define GUI_YMD_HPP

inline void gui_ymd_controls(){
  
  static int tt_ht = 40;
  static vector<string> section_label = {
    "Powertrain###1", // front torque, switch EV vs IC
    "Aerodynamics", // cxa, cza, front aero
    "Tires",
    "Vehicle Mass", // m, i, front %, cg height
    "Suspension (Tuning)", // ack, cam, toe, rh, 
    "Suspension (Design)" // m, 
  };

  ImVec2 sz = ImVec2(325, ImGui::GetContentRegionAvail().y - tt_ht);

  ImGui::BeginChild("Controls", sz, true);

  ImGui::SeparatorText("Plot Controls");
  
  // Generate button
  // Active Plot
  // Plot Count
  // Velocity 
  // Min, Max Accel, Yaw, Steer

  ImGui::SeparatorText("Vehicle Parameters");
  
  for (int i = 0; i < section_label.size(); ++i){
    
    if (ImGui::CollapsingHeader(section_label[i].c_str(), ImGuiTreeNodeFlags_None)){

      //

    }
    
  }

  ImGui::EndChild();

  ImGui::SameLine();

}

inline void gui_ymd_plot(){
  static int tt_ht = 40;
  ImVec2 sz = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - tt_ht);
  ImGui::BeginChild("Plot", sz, true);
  ImGui::EndChild();
}

inline void gui_ymd_hints(gui_io &io){
  
  static vector<string> tt_label = {
    "None: Dynamics",
    "Ctrl: Tires/TV",
    "Shift: Forces",
    "Alt: Alignment",
    "CtrlShift: Forces+",
    "CtrlAlt: Alignment+"
  }; 
  static ImVec4 color_pressed = ImVec4(0.8f, 0.7f, 0.0f, 1.0f);
  
  ImVec2 sz = ImGui::GetContentRegionAvail();
  int spac = sz.x / 5 - 182;

  ImGui::BeginChild("Hints", ImGui::GetContentRegionAvail(), true);
  for (int i = 0; i < tt_label.size(); ++i){

    if (io.flags.key_held == i) 
      ImGui::TextColored(color_pressed, tt_label[i].c_str());
    else 
      ImGui::Text(tt_label[i].c_str());
    
    ImGui::SameLine(0, spac);

  }
  ImGui::EndChild();
}

inline void gui_ymd(gui_io &io){
  // ImGui::Text("Hello :)");
  // Left Section: Controls
  gui_ymd_controls();
  // Right Section: Plots
  gui_ymd_plot();
  // Below Right Section: Keybinds
  gui_ymd_hints(io);
}

#endif
