#ifndef PLOT_TV_HPP
#define PLOT_TV_HPP

void plot_tv(tv_io &io, int i){
  // Generate ovals for Fx, Fy limits
  // Rotate wheel traction oval about each wheel steer angle
  // Progress bar scalar for torque as function of max possible
  // can use framebg or histogram for either direction
  
  // Plots 
  static ImPlotFlags fl_plot = ImPlotFlags_Equal;
  static ImPlotAxisFlags fl_xaxis = ImPlotAxisFlags_NoMenus | ImPlotAxisFlags_NoTickLabels;
  static ImPlotAxisFlags fl_yaxis = fl_xaxis;
  
  ImVec2 sz_plot = ImGui::GetContentRegionAvail();
  sz_plot.y -= 42;
  
  static float x_range = 1600;
  // static float y_range = x_range;

  ImPlot::BeginPlot(("##tire_" + to_string(i)).c_str(), sz_plot, fl_plot); 
  ImPlot::SetupAxisLimits(ImAxis_X1, -x_range, x_range);
  // ImPlot::SetupAxisLimits(ImAxis_Y1, -y_range, y_range);
  ImPlot::SetupAxes("fx [N]", "fy [N]", fl_xaxis, fl_yaxis);
  ImPlot::SetupFinish();
  // y_range = x_range*(ImPlot::GetPlotSize().y / ImPlot::GetPlotSize().x);
  
  


  ImPlot::EndPlot();

  float wid_pb = ImGui::GetContentRegionAvail().x * 0.5 - 4;
  float hei_pb = 10;
  ImVec4 col_bg = ImGui::GetStyleColorVec4(ImGuiCol_FrameBg);
  col_bg.x *= 0.45;
  col_bg.y *= 0.45;
  col_bg.z *= 0.45;
  col_bg.w = 1.0f;
  float placeholder = 0.5f;

  // Regen bar 
  char buf_regen[32];
  ImVec4 col_regen = ImVec4(0.9f, 0.8f, 0.0f, 1.0f);
  // sprintf(buf_regen, "%.1f N.m", 69.4);
  sprintf(buf_regen, "");
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, col_bg);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, col_regen);
  ImGui::ProgressBar(placeholder, ImVec2(wid_pb, hei_pb), buf_regen);
  ImGui::PopStyleColor(2);

  ImGui::SameLine();

  // Motor bar 
  char buf_motor[32];
  ImVec4 col_motor = ImVec4(0.3f, 0.8f, 0.4f, 1.0f);
  sprintf(buf_motor, "");
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, col_motor);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, col_bg);
  ImGui::ProgressBar(placeholder, ImVec2(wid_pb, hei_pb), buf_motor);
  ImGui::PopStyleColor(2);

  // Brake bar 
  char buf_brake[32];
  ImVec4 col_brake = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
  sprintf(buf_brake, "");
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, col_bg);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, col_brake);
  ImGui::ProgressBar(placeholder, ImVec2(wid_pb, hei_pb), buf_brake);
  ImGui::PopStyleColor(2);

  ImGui::SameLine();

  // No bar 
  char buf_none[32];
  ImVec4 col_none = ImVec4(0.8f, 0.3f, 0.4f, 1.0f);
  sprintf(buf_none, "");
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, col_none);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, col_bg);
  ImGui::ProgressBar(0.0f, ImVec2(wid_pb, hei_pb), buf_none);
  ImGui::PopStyleColor(2);

  // Cornering bars 
  char buf_corn[32];
  ImVec4 col_corn = ImVec4(0.7f, 0.1f, 0.7f, 1.0f);
  sprintf(buf_corn, "");
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, col_bg);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, col_corn);
  ImGui::ProgressBar(placeholder, ImVec2(wid_pb, hei_pb), buf_corn);
  ImGui::PopStyleColor(2);
  
  ImGui::SameLine();
  
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, col_corn);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, col_bg);
  ImGui::ProgressBar(placeholder, ImVec2(wid_pb, hei_pb), buf_corn);
  ImGui::PopStyleColor(2);


}

#endif
