#ifndef PLOT_TV_HPP
#define PLOT_TV_HPP

void plot_tv(tv_io &io, int i){
  // Generate ovals for Fx, Fy limits
  // Rotate wheel traction oval about each wheel steer angle
  // Progress bar scalar for torque as function of max possible
  // can use framebg or histogram for either direction
  
  // Generate torque response
  io.calculate_torques();



  // Plots 
  static ImPlotFlags fl_plot = ImPlotFlags_Equal;
  static ImPlotAxisFlags fl_xaxis = ImPlotAxisFlags_NoMenus | ImPlotAxisFlags_NoTickLabels;
  static ImPlotAxisFlags fl_yaxis = fl_xaxis;
  
  ImVec2 sz_plot = ImGui::GetContentRegionAvail();
  sz_plot.y -= 42;
  
  static float x_range = 1600;
  // static float y_range = x_range;

  ImPlot::BeginPlot(("##tire_" + to_string(i)).c_str(), sz_plot, fl_plot);
  ImPlot::SetupAxisLimits(ImAxis_X1, -x_range, x_range, ImPlotCond_Always);
  ImPlot::SetupAxisLimits(ImAxis_Y1, -x_range, x_range, ImPlotCond_Always);
  ImPlot::SetupAxes("fx [N]", "fy [N]", fl_xaxis, fl_yaxis);
  ImPlot::SetupFinish();

  // Friction circle
  static const int N = 61;
  float cx[N], cy[N], ox[N], oy[N];
  for (int k = 0; k < N; k++) {
    float a = k * 2.0f * M_PI / (N - 1);
    cx[k] = io.fx_max * cosf(a);
    cy[k] = io.fx_max * sinf(a);
  }

  float net_trq = io.trq_fwd[i] - io.trq_reg[i];
  float util = fabsf(net_trq) / (io.trq_max * io.gear);
  if (util > 1.0f) util = 1.0f;

  ImVec4 col_inner = (io.brk[i] > 0.0f) ? io.col_brake :
                     (net_trq >= 0.0f)   ? io.col_motor : io.col_regen;

  for (int k = 0; k < N; k++) {
    float a = k * 2.0f * M_PI / (N - 1);
    ox[k] = util * io.fx_max * cosf(a);
    oy[k] = util * io.fx_max * sinf(a);
  }

  // Outer traction limit
  ImPlot::SetNextLineStyle(ImVec4(0.4f, 0.5f, 0.7f, 0.6f), 1.5f);
  ImPlot::PlotLine(("##tc_" + to_string(i)).c_str(), cx, cy, N);

  // Operating envelope
  ImPlot::SetNextLineStyle(col_inner, 2.0f);
  ImPlot::PlotLine(("##oe_" + to_string(i)).c_str(), ox, oy, N);

  // Operating point
  float px = (net_trq - io.brk[i]) / io.rad_wheel;
  float py = 0.0f;
  ImPlot::SetNextMarkerStyle(ImPlotMarker_Diamond, 6.0f, col_inner, 1.0f, col_inner);
  ImPlot::PlotScatter(("##op_" + to_string(i)).c_str(), &px, &py, 1);

  ImPlot::EndPlot();

  float wid_pb = ImGui::GetContentRegionAvail().x * 0.5 - 4;
  float hei_pb = 10;
  float placeholder = 0.5f;

  // Regen bar 
  char buf_regen[32];
  sprintf(buf_regen, "");
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, io.col_bg);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, io.col_regen);
  ImGui::ProgressBar(io.tpb_reg[i], ImVec2(wid_pb, hei_pb), buf_regen);
  ImGui::PopStyleColor(2);

  ImGui::SameLine();

  // Motor bar 
  char buf_motor[32];
  sprintf(buf_motor, "");
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, io.col_motor);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, io.col_bg);
  ImGui::ProgressBar(io.tpb_fwd[i], ImVec2(wid_pb, hei_pb), buf_motor);
  ImGui::PopStyleColor(2);

  // Brake bar 
  char buf_brake[32];
  sprintf(buf_brake, "");
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, io.col_bg);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, io.col_brake);
  ImGui::ProgressBar(io.bpb[i], ImVec2(wid_pb, hei_pb), buf_brake);
  ImGui::PopStyleColor(2);

  ImGui::SameLine();

  // No bar 
  char buf_none[32];
  sprintf(buf_none, "");
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, io.col_none);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, io.col_bg);
  ImGui::ProgressBar(0.0f, ImVec2(wid_pb, hei_pb), buf_none);
  ImGui::PopStyleColor(2);

  // Cornering bars 
  char buf_corn[32];
  sprintf(buf_corn, "");
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, io.col_bg);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, io.col_corn);
  ImGui::ProgressBar(placeholder, ImVec2(wid_pb, hei_pb), buf_corn);
  ImGui::PopStyleColor(2);
  
  ImGui::SameLine();
  
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, io.col_corn);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, io.col_bg);
  ImGui::ProgressBar(placeholder, ImVec2(wid_pb, hei_pb), buf_corn);
  ImGui::PopStyleColor(2);


}

#endif
