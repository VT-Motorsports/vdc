#ifndef GUI_TIRE_HPP
#define GUI_TIRE_HPP

inline void gui_tire(gui_io &io){
	// Setup flags 
	static const ImPlotSubplotFlags_ subplots_flags = ImPlotSubplotFlags_NoResize;
	//static const ImPlotPlotFlags_ plot_flags = 
	const ImPlotAxisFlags_ x_axis_flags = ImPlotAxisFlags_AutoFit;
	const ImPlotAxisFlags_ y_axis_flags = ImPlotAxisFlags_AutoFit;
	const ImVec4 color_data = ImVec4(0.22f, 0.51f, 0.85f, 1.00f);
	const ImVec4 color_model = ImVec4(0.22f, 0.65f, 0.31f, 1.00f);
	const ImVec4 color_unscaled = ImVec4(0.85f, 0.22f, 0.31f, 1.00f);
		
	const ImVec2 subplots_frame = ImVec2(ImGui::GetContentRegionAvail().x * 1.00f, ImGui::GetContentRegionAvail().y - 130);
	ImGui::BeginChild("##tire_plots", subplots_frame, false, ImGuiWindowFlags_HorizontalScrollbar);
	const ImVec2 subplots_inner = ImVec2(ImGui::GetContentRegionAvail().x * io.td.load.n_elem / 3, ImGui::GetContentRegionAvail().y * io.td.camber.n_elem / 2);
	ImPlot::BeginSubplots("##tire_subplots", io.td.camber.n_elem, io.td.load.n_elem, subplots_inner, subplots_flags);
	for (int j = 0; j < io.td.camber.n_elem; ++j) {
		for (int i = 0; i < io.td.load.n_elem; ++i) {
			vec x_data, y_data;
			vec x_model, y_model, y_model_unscaled;
			const string plot_title = (io.td.load_label[i] + " @ " + io.td.camber_label[j]);
			ImPlot::BeginPlot(plot_title.c_str());
			switch (io.flags.tire_plot_type) {
				case 0: // FX
					ImPlot::SetupAxis(ImAxis_X1, "SL [-]", x_axis_flags);
					ImPlot::SetupAxis(ImAxis_Y1, "FX [N]", y_axis_flags);
					x_data = io.td.db_sr(i, j);
					y_data = io.td.db_fx(i, j);
					x_model = linspace(-0.14, 0.14, 100);
					y_model = io.tm.get_fx(io.td.load(i), io.td.camber(j), x_model);
					y_model_unscaled = y_model / io.tm.sb;
					break;
				case 1: // FY
					ImPlot::SetupAxis(ImAxis_X1, "SA [°]", x_axis_flags);
					ImPlot::SetupAxis(ImAxis_Y1, "FY [N]", y_axis_flags);
					x_data = io.td.c_sa(i, j);
					y_data = io.td.c_fy(i, j);
					x_model = linspace(-12, 12, 100);
					y_model = io.tm.get_fy(io.td.load(i), io.td.camber(j), x_model);
					y_model_unscaled = y_model / io.tm.sa;
					break;
				case 2: // MZ
					ImPlot::SetupAxis(ImAxis_X1, "SA [°]", x_axis_flags);
					ImPlot::SetupAxis(ImAxis_Y1, "MZ [N.m]", y_axis_flags);
					x_data = io.td.c_sa(i, j);
					y_data = io.td.c_mz(i, j);
					x_model = linspace(-12, 12, 100);
					y_model = io.tm.get_mz(io.td.load(i), io.td.camber(j), x_model);
					y_model_unscaled = y_model / io.tm.sc;
					break;
				default: 
					break;
			}
			const vector<double> x_data_arr = conv_to<vector<double>>::from(x_data);
			const vector<double> y_data_arr = conv_to<vector<double>>::from(y_data);
			const vector<double> x_model_arr = conv_to<vector<double>>::from(x_model);
			const vector<double> y_model_arr = conv_to<vector<double>>::from(y_model);
			const vector<double> y_model_unscaled_arr = conv_to<vector<double>>::from(y_model_unscaled);
			ImPlot::PushStyleVar(ImPlotStyleVar_MarkerSize, 0.5f);
			ImPlot::PushStyleColor(ImPlotCol_Line, color_data);
			ImPlot::PlotScatter("Data", x_data_arr.data(), y_data_arr.data(), x_data_arr.size());
			ImPlot::PopStyleVar();
			ImPlot::PopStyleColor();
			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 2.5f);
			ImPlot::PushStyleColor(ImPlotCol_Line, color_unscaled);
			ImPlot::PlotLine("Real", x_model_arr.data(), y_model_arr.data(), x_model_arr.size());
			ImPlot::PopStyleVar();
			ImPlot::PopStyleColor();
			ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 2.5f);
			ImPlot::PushStyleColor(ImPlotCol_Line, color_model);
			ImPlot::PlotLine("Fit", x_model_arr.data(), y_model_unscaled_arr.data(), x_model_arr.size());
			ImPlot::PopStyleVar();
			ImPlot::PopStyleColor();
			ImPlot::EndPlot();
		}
	}
	ImPlot::EndSubplots();
	ImGui::EndChild();
	
	
	const ImVec2 selector_frame = ImVec2(130, ImGui::GetContentRegionAvail().y);
	ImGui::BeginChild("##", selector_frame, true);
	{
		ImGui::RadioButton("fx Fit", &io.flags.tire_plot_type, 0);
		ImGui::RadioButton("fy Fit", &io.flags.tire_plot_type, 1);
		ImGui::RadioButton("mz Fit", &io.flags.tire_plot_type, 2);
	}
	ImGui::EndChild();
	ImGui::SameLine();
	
	const ImVec2 variables_frame = ImGui::GetContentRegionAvail();
	ImGui::BeginChild("##", selector_frame, true);
	{
		switch (io.flags.tire_plot_type) {
			case 0: // FX
				break;
			case 1: // FY
				break;
			case 2: // MZ
				break;
			default: 
				break;
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
}

#endif