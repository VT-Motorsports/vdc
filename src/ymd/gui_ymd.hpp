#ifndef GUI_YMD_HPP
#define GUI_YMD_HPP

inline void gui_ymd_controls(gui_io &io){
  
  static int tt_ht = 40;

  static vector<string> section_label = {
    "Chassis", // m, i, front %, cg height
    "Aerodynamics", // cxa, cza, front aero
    "Powertrain", // front torque, switch EV vs IC
    "Tires", // p94_s, dia, kp
    "Suspension (Design)", // l, t_f, t_r, asq, adi, rat,
    "Suspension (Tuning)", // ack, cam, toe, rh, fb, ks, ka, kp
  };
  
  static field<vector<string>> tree_label(section_label.size(), 1);
  static field<vector<double*>> tree_vars(section_label.size(), 1);
  static field<vector<double>> tree_precis(section_label.size(), 1);
  static vector<double*> tree_var_next;
  
  tree_label(0, 0) = {
    "mass [kg]",
    "inertia [kg.m²]",
    "front weight [-]",
    "CG height [m]",
  };
  tree_var_next = { 
    &io.car.m,
    &io.car.i_zz,
    &io.car.fw,
    &io.car.h_s,
  };
  tree_vars(0, 0) = tree_var_next;
  tree_precis(0, 0) = {
    +0,
    +0,
    +3,
    +3,
  };

  tree_label(1, 0) = {
    "cxa",
    "cza",
    "front aero [%]",
  };
  tree_var_next = {
    &io.car.cxa,
    &io.car.cza,
    &io.car.fa
  };
  tree_vars(1, 0) = tree_var_next;
  tree_precis(1, 0) = {
    +2,
    +2,
    +3,
  };
  
  tree_label(2, 0) = {
    "front torque [-]",
  };
  tree_var_next = { 
    &io.car.ft,
  };
  tree_vars(2, 0) = tree_var_next;
  tree_precis(2, 0) = {
    +2,
  };

  tree_label(3, 0) = {
    "tire diameter [m]",
    "scale - fy",
    "scale - fx",
    "scale - mz",
    "tire stiffness (f) [N/m]",
    "tire stiffness (r) [N/m]",
  };
  tree_var_next = { 
    &io.car.dia,
    &io.car.p94_s(0),
    &io.car.p94_s(1),
    &io.car.p94_s(2),
    &io.car.kp_f,
    &io.car.kp_r,
  };
  tree_vars(3, 0) = tree_var_next;
  tree_precis(3, 0) = {
    +3,
    +2,
    +2,
    +2,
    -5,
    -5,
  };

  tree_label(4, 0) = {
    "wheelbase [m]",
    "front track [m]",
    "rear track [m]",
    "anti-squat [-]",
    "anti-dive [-]",
    "ackermann [°/°²]",
    "steering ratio [-]",
    "front rc [m]",
    "rear rc [m]",
    "front camber gain [°/m]",
    "rear camber gain [°/m]",
    "front toe gain [°/m]",
    "rear toe gain [°/m]",
    "front motion ratio [-]",
    "rear motion ratio [-]",
  };
  tree_var_next = { 
    &io.car.l,
    &io.car.t_f,
    &io.car.t_r,
    &io.car.asq,
    &io.car.adi,
    &io.car.ack,
    &io.car.rat,
    &io.car.rc_f,
    &io.car.rc_r,
    &io.car.cag_f,
    &io.car.cag_r,
    &io.car.tog_f,
    &io.car.tog_r,
    &io.car.mrs_f,
    &io.car.mrs_r,
  };
  tree_vars(4, 0) = tree_var_next;
  tree_precis(4, 0) = {
    +3,
    +3,
    +3,
    +3,
    +3,
    +3,
    +3,
    +3,
    +3,
    -0,
    -0,
    +1,
    +1,
    +3,
    +3,
  };
  
  tree_label(5, 0) = {
    "camber (f) [°]",
    "camber (r) [°]",
    "toe (f) [°]",
    "toe (r) [°]",
    "ride height (f) [m]",
    "ride height (r) [m]",
    "front braking [-]",
    "spring stiffness (f) [N/m]",
    "spring stiffness (r) [N/m]",
    "arb stiffness (f) [N/m]",
    "arb stiffness (r) [N/m]",
  };
  tree_var_next = { 
    &io.car.cam_f,
    &io.car.cam_r,
    &io.car.toe_f,
    &io.car.toe_r,
    &io.car.rh_f,
    &io.car.rh_r,
    &io.car.fb,
    &io.car.ks_f,
    &io.car.ks_r,
    &io.car.ka_f,
    &io.car.ka_r,
  };
  tree_vars(5, 0) = tree_var_next;
  tree_precis(5, 0) = {
    +1,
    +1,
    +1,
    +1,
    +3,
    +3,
    +2,
    -4,
    -4,
    -3,
    -3,
  };

  ImVec2 sz = ImVec2(325, ImGui::GetContentRegionAvail().y - tt_ht);

  ImGui::BeginChild("Controls", sz, true);

  ImGui::SeparatorText("Plot Controls");
  
  // Generate button
  if(ImGui::Button("Generate")){io.ymdio.update = true;}
  // Active Plot
  // Plot Count
  // Velocity 
  // Min, Max Accel, Yaw, Steer

  ImGui::SeparatorText("Vehicle Parameters");
  
  for (int i = 0; i < section_label.size(); ++i){
    
    if (ImGui::CollapsingHeader(section_label[i].c_str(), ImGuiTreeNodeFlags_None)){
      
      static float input_width = 150;
      ImGui::PushItemWidth(input_width);

      for (int j = 0; j < tree_label(i, 0).size(); ++j){

        string num_format;
        if (tree_precis(i, 0)[j] >= 0){
          num_format = "%." + to_string(static_cast<int>(tree_precis(i, 0)[j])) + "f";
        }
        else {
          num_format = "%.0f";
        }

        ImGui::InputDouble(
            tree_label(i, 0)[j].c_str(), 
            tree_vars(i, 0)[j], 
            pow(10, -tree_precis(i, 0)[j]), 
            pow(10, 1 - tree_precis(i, 0)[j]), 
            num_format.c_str());
      }
      
      ImGui::PopItemWidth();

    }
    
  }

  ImGui::EndChild();

  ImGui::SameLine();

}

inline void gui_ymd_plot(){
  static int tt_ht = 40;
  ImVec2 sz = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - tt_ht);
  ImGui::BeginChild("Plots", sz, true);
	
  static const int cms_w = 130; // Colormap scale width
	ImPlot::BeginPlot("Yaw Moment Diagram", ImVec2(ImGui::GetContentRegionAvail().x - cms_w - 8, ImGui::GetContentRegionAvail().y));
  
  ImPlot::SetupAxis(ImAxis_X1, "AY [m.s-2]", ImPlotAxisFlags_None);
  ImPlot::SetupAxisLimits(ImAxis_X1, -20.0, 20.0, ImGuiCond_FirstUseEver);
  ImPlot::SetupAxis(ImAxis_Y1, "AA [rad.s-2]", ImPlotAxisFlags_None);
  ImPlot::SetupAxisLimits(ImAxis_Y1, -70.0, 70.0, ImGuiCond_FirstUseEver);

  ImPlot::EndPlot();

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
