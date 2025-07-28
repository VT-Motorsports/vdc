#ifndef GUI_YMD_HPP
#define GUI_YMD_HPP

inline void gui_ymd_controls(gui_io &io){
  
  static int tt_ht = 40;

  static vector<string> section_label = {
    "Powertrain", // front torque, switch EV vs IC
    "Aerodynamics", // cxa, cza, front aero
    "Tires",
    "Vehicle Mass", // m, i, front %, cg height
    "Suspension (Tuning)", // ack, cam, toe, rh, 
    "Suspension (Design)" // m, 
  };
  
  static field<vector<string>> tree_label(section_label.size(), 1);
  static field<vector<double*>> tree_vars(section_label.size(), 1);
  static field<vector<double>> tree_precis(section_label.size(), 1);
  static vector<double*> tree_var_next;

  tree_label(0) = {

  };
  tree_var_next = { 
  
  };
  tree_vars(0, 0) = tree_var_next;
  tree_precis(0, 0) = {
  
  };

  tree_label(1) = {
    "cxa",
    "cza_f",
    "cza_r"
  };
  tree_var_next = {
    &io.car.cxa,
    &io.car.cza_f,
    &io.car.cza_r
  };
  tree_vars(1, 0) = tree_var_next;
  tree_precis(1, 0) = {
    +2,
    +2,
    +2
  };
  
  tree_label(2) = {
    "scale - fy",
    "scale - fx",
    "scale - mz"
  };
  tree_var_next = { 
    &io.car.p94_s(0),
    &io.car.p94_s(1),
    &io.car.p94_s(2)
  };
  tree_vars(2, 0) = tree_var_next;
  tree_precis(2, 0) = {
    +2,
    +2,
    +2
  };
  
  tree_label(3) = {
    "mass [kg]",
    "inertia [kg.m²]",
    "front weight [-]",
    "CG height [m]"
  };
  tree_var_next = { 
    &io.car.m,
    &io.car.i_zz,
    &io.car.fw,
    &io.car.h_s
  };
  tree_vars(3, 0) = tree_var_next;
  tree_precis(3, 0) = {
    +0,
    +0,
    +3,
    +3
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

      for (int j = 0; j < tree_label(i, 0).size(); ++j){
        ImGui::InputDouble(
            tree_label(i, 0)[j].c_str(), 
            tree_vars(i, 0)[j], 
            pow(10, -tree_precis(i, 0)[j]), 
            pow(10, 1 - tree_precis(i, 0)[j]), 
            "%.3f");
      }

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
  gui_ymd_controls(io);
  // Right Section: Plots
  gui_ymd_plot();
  // Below Right Section: Keybinds
  gui_ymd_hints(io);
}

#endif
