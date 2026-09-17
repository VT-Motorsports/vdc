#ifndef GUI_TV_HPP
#define GUI_TV_HPP

void CenteredText(const char* fmt){

}

void gui_tv(tv_io &io){
 
  int window_w = ImGui::GetContentRegionAvail().x;
  int window_h = ImGui::GetContentRegionAvail().y;
  int pad = ImGui::GetStyle().ItemSpacing.x;

  io.col_bg = ImGui::GetStyleColorVec4(ImGuiCol_FrameBg);
  io.col_bg.x *= 0.50;
  io.col_bg.y *= 0.50;
  io.col_bg.z *= 0.45;
  io.col_bg.w = 1.0f;

  int median_w = 380;
  int median_h[] = {65, 195, window_h - 655 - 4*pad, 360, 35};

  int wheel_child_w = 0.5 * (window_w - median_w - 2*pad);
  int wheel_child_h[4] = {0.5*window_h - pad/2, 0.5*window_h - pad/2, 0.5*window_h - pad/2, 0.5*window_h - pad/2};
  int wheel_child_x[4] = {pad, pad + window_w - wheel_child_w, pad, pad + window_w - wheel_child_w};
  int wheel_child_y[4] = {pad, pad, 2*pad + wheel_child_h[0], 2*pad + wheel_child_h[0]};

  // int median_w = (window_w - 2*wheel_child_w[0] - 2*pad);

  for (int i = 0; i < 4; ++i){
    ImGui::SetCursorPos(ImVec2(wheel_child_x[i], wheel_child_y[i]));
    ImGui::BeginChild(("##wheel" + to_string(i)).c_str(), ImVec2(wheel_child_w, wheel_child_h[i]), true);
    // ImGui::Text(to_string(i).c_str());
    plot_tv(io, i);
    ImGui::EndChild();
  }

  // Battery Usage
  ImGui::SetCursorPos(ImVec2(wheel_child_x[0] + wheel_child_w + pad, wheel_child_y[0]));
  ImGui::BeginChild("##battery usage", ImVec2(median_w, median_h[0]), true);
  ImGui::Dummy(ImVec2(0.5 * (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Battery Usage").x), 0));
  ImGui::SameLine();
  ImGui::Text("Battery Usage");
  char pb_bat[32];
  sprintf(pb_bat, "%.1f N.m", 69.4);
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.3f, 0.8f, 0.4f, 1.0f));
  ImGui::ProgressBar(69.4/200 + 0.5f, ImVec2(ImGui::GetContentRegionAvail().x, 0.0f), pb_bat); 
  ImGui::PopStyleColor();
  ImGui::EndChild();

  // Inputs section
  ImGui::SetCursorPos(ImVec2(wheel_child_x[0] + wheel_child_w + pad, ImGui::GetCursorPosY() + pad/2));
  ImGui::BeginChild("##inputs", ImVec2(median_w, median_h[1]), true);
  // ImGui::Text("inputs");
  
  
  float wd_input = 0.5 * ImGui::GetContentRegionAvail().x - 4;

  ImGui::Dummy(ImVec2(0.5 * (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Steer / Sideslip").x), 0));
  ImGui::SameLine();
  ImGui::Text("Steer / Sideslip");
  ImGui::PushItemWidth(wd_input);
  ImGui::SliderFloat("##Steer", &io.steer, -110, 110, "%.0f deg"); ImGui::SameLine();
  ImGui::SliderFloat("##Sideslip", &io.sideslip, -15, 15, "%.0f deg");
  ImGui::PopItemWidth();
  
  ImGui::Dummy(ImVec2(0, 10));

  ImGui::Dummy(ImVec2(0.5 * (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Brake / Throttle").x), 0));
  ImGui::SameLine();
  ImGui::Text("Brake / Throttle");
  ImGui::PushItemWidth(wd_input);
  ImGui::SliderFloat("##Brake", &io.brake, 0, 100, "%.0f%%"); ImGui::SameLine();
  ImGui::SliderFloat("##Throttle", &io.throttle, 0, 100, "%.0f%%");
  ImGui::PopItemWidth();

  ImGui::Dummy(ImVec2(0, 10));

  ImGui::Dummy(ImVec2(0.5f * (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Velocity").x), 0));
  ImGui::SameLine();
  ImGui::Text("Velocity");
  ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
  ImGui::SliderFloat("##Velocity", &io.velocity, 0, 50, "%.1f m/s");
  ImGui::PopItemWidth();
  ImGui::EndChild();

  // Dynamic outputs (stretchy section)
  ImGui::SetCursorPos(ImVec2(wheel_child_x[0] + wheel_child_w + pad, ImGui::GetCursorPosY() + pad/2));
  ImGui::BeginChild("##dynout", ImVec2(median_w, median_h[2]), true);
  ImGui::Dummy(ImVec2(0.5f * (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Dynamic Outputs").x), 0));
  ImGui::SameLine();
  ImGui::Text("Dynamic Outputs");

  // Yaw state readouts
  ImGui::SeparatorText("Yaw State");
  ImGui::Text("Yaw Rate:        %+7.2f  deg/s", io.yaw_rate);
  ImGui::Text("Yaw Rate Target: %+7.2f  deg/s", io.yaw_rate_target);
  ImGui::Text("Yaw Accel:       %+7.2f  deg/s\xc2\xb2", io.yaw_accel);

  // Individual wheel RPM sliders
  ImGui::Dummy(ImVec2(0, 6));
  ImGui::SeparatorText("Wheel RPMs  [motor-side]");
  static const char* rpm_label[4] = {"FL##rpm", "FR##rpm", "RL##rpm", "RR##rpm"};
  float half_w = 0.5f * ImGui::GetContentRegionAvail().x - 4.0f;
  for (int i = 0; i < 4; ++i) {
    ImGui::PushItemWidth(half_w);
    ImGui::SliderFloat(rpm_label[i], &io.rpm[i], -6000.0f, 6000.0f, "%.0f");
    ImGui::PopItemWidth();
    if (i % 2 == 0) ImGui::SameLine();
  }
  ImGui::EndChild();

  // Tuning parameters (constants)
  ImGui::SetCursorPos(ImVec2(wheel_child_x[0] + wheel_child_w + pad, ImGui::GetCursorPosY() + pad/2));
  ImGui::BeginChild("##tooning", ImVec2(median_w, median_h[3]), true);
  ImGui::Dummy(ImVec2(0.5f * (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Tuning Parameters").x), 0));
  ImGui::SameLine();
  ImGui::Text("Tuning Parameters");

  // Yaw TV gains
  ImGui::SeparatorText("Yaw TV Gains");
  float full_w = ImGui::GetContentRegionAvail().x;
  ImGui::PushItemWidth(full_w);
  ImGui::SliderFloat("k_steer##tv",        &io.k_steer,        0.0f,  5.0f,   "k_steer  %.3f");
  ImGui::SliderFloat("k_yaw_err##tv",      &io.k_yaw_err,      0.0f, 10.0f,   "k_yaw_err %.3f");
  ImGui::SliderFloat("k_apex##tv",         &io.k_apex,         0.0f,  1.0f,   "k_apex   %.3f");
  ImGui::SliderFloat("apex_threshold##tv", &io.apex_threshold, 1.0f, 500.0f,  "thresh  %.1f deg\xc2\xb2/s\xe2\x81\xb4");
  ImGui::PopItemWidth();

  // RPM slip correction gain
  ImGui::SeparatorText("RPM Slip Correction");
  ImGui::PushItemWidth(full_w);
  ImGui::SliderFloat("k_rpm##tv", &io.k_rpm, 0.0f, 20.0f, "k_rpm  %.2f Nm.s/m");
  ImGui::PopItemWidth();

  // Load transfer and grip constants
  ImGui::SeparatorText("Load Transfer / Grip");
  ImGui::PushItemWidth(full_w);
  ImGui::SliderFloat("mass##tv",        &io.mass,           100.0f,   400.0f, "mass  %.0f kg");
  ImGui::SliderFloat("h_cg##tv",        &io.h_cg,             0.10f,    0.60f, "h_cg  %.3f m");
  ImGui::SliderFloat("weight_dist##tv", &io.weight_dist,      0.30f,    0.70f, "wt_f  %.3f");
  ImGui::SliderFloat("mu##tv",          &io.mu,               0.50f,    2.00f, "mu    %.2f");
  ImGui::SliderFloat("c_f##tv",         &io.c_f,          10000.0f, 100000.0f, "c_f   %.0f N/rad");
  ImGui::SliderFloat("c_r##tv",         &io.c_r,          10000.0f, 100000.0f, "c_r   %.0f N/rad");
  ImGui::PopItemWidth();
  ImGui::EndChild();

  // Modes (colored text)
  ImGui::SetCursorPos(ImVec2(wheel_child_x[0] + wheel_child_w + pad, ImGui::GetCursorPosY() + pad/2));
  ImGui::BeginChild("##modes", ImVec2(median_w, median_h[4]), true);
  static float pad_tc = 20.0f;
  ImGui::TextColored(io.col_regen, "Regen"); ImGui::SameLine(0.0f, pad_tc);
  ImGui::TextColored(io.col_motor, "Motor"); ImGui::SameLine(0.0f, pad_tc);
  ImGui::TextColored(io.col_brake, "Brake+Regen"); ImGui::SameLine(0.0f, pad_tc);
  ImGui::TextColored(io.col_corn, "Grip Limit");
  ImGui::EndChild();
}

#endif 
