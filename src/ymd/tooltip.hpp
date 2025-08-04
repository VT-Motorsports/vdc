#ifndef TOOLTIP_HPP 
#define TOOLTIP_HPP 

string num_to_str_1(const double var, const int length, const int precision, const int offset){
  ostringstream oss;
  for (int i = 0; i < offset; ++i){
    oss << " "; 
  }
  oss << showpos << fixed << setprecision(precision) << setfill(' ') << setw(length) << 
    var;
  return oss.str();
}

string num_to_str_4(const vec var, const int length, const int precision){
  ostringstream oss;
  oss << showpos << fixed << setprecision(precision) << setfill(' ') << setw(length) << 
    var(0) << " " << setw(length) << var(1) << endl << setw(length) << var(2) << " " << setw(length) << var(3);
  return oss.str();
}

bool is_hovering(ymd_v_io &io) {
	bool hovering = false;
	double min = 10;
	for (int j = 0; j < io.n_cols; ++j) {
		for (int i = 0; i < io.n_rows; ++i) {
			const ImVec2 mouse_pos = ImPlot::PlotToPixels(ImPlot::GetPlotMousePos());
			const ImVec2 point_pos = ImPlot::PlotToPixels(ImPlotPoint(io.ay(i, j, io.ax_plot), io.aa(i, j, io.ax_plot)));
			const double dx = mouse_pos.x - point_pos.x;
			const double dy = mouse_pos.y - point_pos.y;
			if (sqrt(dx * dx + dy * dy) < min) {
				min = sqrt(dx * dx + dy * dy);
				io.i_tt = i;
				io.j_tt = j;
				hovering = true;
			}
		}
	}
	return hovering;
}

void tooltip(gui_io &io) {

  vector<string> names;
  vector<string> numbers;
  vector<string> units;
  int row_width, single_offset;
  int i = io.ymdio.i_tt;
  int j = io.ymdio.j_tt;
  int k = io.ymdio.ax_plot;

  // Define tooltips  
  switch (io.flags.key_held) {
    // None 
    case 0:
      row_width = 6;
      single_offset = 0;
      names = {
        "sideslip",
        "steer",
        "accel x",
        "accel y",
        "ang acc",
        "control",
        "stability",
      };
      numbers = {
        num_to_str_1(io.ymdio.yaw(i, j, k), row_width, 1, single_offset),
        num_to_str_1(io.ymdio.steer(i, j, k), row_width, 0, single_offset),
        num_to_str_1(io.ymdio.ax(i, j, k) / 9.81, row_width, 3, single_offset),
        num_to_str_1(io.ymdio.ay(i, j, k) / 9.81, row_width, 3, single_offset),
        num_to_str_1(io.ymdio.aa(i, j, k), row_width, 0, single_offset),
        num_to_str_1(io.ymdio.cnt(i, j, k), row_width, 1, single_offset),
        num_to_str_1(io.ymdio.stb(i, j, k), row_width, 1, single_offset),
        // num_to_str_1(io.ymdio.usgr(i, j, k), row_width, 3, single_offset),
      };
      units = {
        "°",
        "°",
        "G",
        "G",
        "°/s²",
        "°/s²/°",
        "°/s²/°",
      };
      break;
    
    // Ctrl
    case 1:
      break;

    // Shift
    case 2:
      row_width = 6;
      single_offset = 0;
      names = {
        "fx",
        "fy",
        "fz",
        "mz",
      };
      numbers = {
        num_to_str_4(io.ymdio.x(i, j, k).t, row_width, 0),
        num_to_str_4(io.ymdio.y(i, j, k).t, row_width, 0),
        num_to_str_4(io.ymdio.z(i, j, k)(), row_width, 0),
        num_to_str_4(io.ymdio.mz(i, j, k).t, row_width, 0),
      };
      units = {
        "N",
        "N",
        "N",
        "N.m",
      };
      break;
  }
   
  ImGui::BeginTooltip();
  
  if(ImGui::BeginTable("tt_table", 3, ImGuiTableFlags_Borders)){
    for (int row = 0; row < names.size(); ++row){
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text((names[row]+":").c_str());
      ImGui::TableNextColumn();
      ImGui::Text(numbers[row].c_str());
      ImGui::TableNextColumn();
      ImGui::Text(("["+units[row]+"]").c_str());
      
    }
    ImGui::EndTable();
  }

  ImGui::EndTooltip();

}

#endif
