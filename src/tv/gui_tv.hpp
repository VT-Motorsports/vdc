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
  int median_h[] = {65, 145, 250 + window_h - 720 - pad, 200, 35};

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
  ImGui::Text("battery usage");
  char pb_bat[32];
  sprintf(pb_bat, "%.1f N.m", 69.4);
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.3f, 0.8f, 0.4f, 1.0f));
  ImGui::ProgressBar(69.4/200 + 0.5f, ImVec2(ImGui::GetContentRegionAvail().x, 0.0f), pb_bat); 
  ImGui::PopStyleColor();
  ImGui::EndChild();

  // Inputs section
  ImGui::SetCursorPos(ImVec2(wheel_child_x[0] + wheel_child_w + pad, ImGui::GetCursorPosY() + pad/2));
  ImGui::BeginChild("##inputs", ImVec2(median_w, median_h[1]), true);
  ImGui::Text("inputs");
  ImGui::Text("Steer / Sideslip");
  ImGui::SliderAngle("##Steer", &io.steer, -110, 110);
  ImGui::SliderAngle("##Sideslip", &io.sideslip, -15, 15);
  ImGui::SliderFloat("##Brake", &io.brake, 0, 100, "%.0f%%"); ImGui::SameLine();
  ImGui::SliderFloat("##Throttle", &io.throttle, 0, 100, "%.0f%%");
  ImGui::Text("Brake / Throttle");
  ImGui::EndChild();

  // Dynamic outputs (stretchy section)
  ImGui::SetCursorPos(ImVec2(wheel_child_x[0] + wheel_child_w + pad, ImGui::GetCursorPosY() + pad/2));
  ImGui::BeginChild("##dynout", ImVec2(median_w, median_h[2]), true);
  ImGui::Text("dynamic outputs");
  ImGui::EndChild();

  // Tuning parameters (constants)
  ImGui::SetCursorPos(ImVec2(wheel_child_x[0] + wheel_child_w + pad, ImGui::GetCursorPosY() + pad/2));
  ImGui::BeginChild("##tooning", ImVec2(median_w, median_h[3]), true);
  ImGui::Text("tuning parameters");
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
