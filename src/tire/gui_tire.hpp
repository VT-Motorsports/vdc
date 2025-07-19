#ifndef GUI_TIRE_HPP
#define GUI_TIRE_HPP

inline void gui_tire(gui_io &io){
	// Setup flags 
	static const ImPlotSubplotFlags_ subplots_flags = ImPlotSubplotFlags_NoResize;
	//static const ImPlotPlotFlags_ plot_flags = 
	const ImPlotAxisFlags_ x_axis_flags = ImPlotAxisFlags_AutoFit;
	const ImPlotAxisFlags_ y_axis_flags = ImPlotAxisFlags_AutoFit;
	const ImVec4 color_data = ImVec4(0.22f, 0.51f, 0.85f, 1.00f);
	const ImVec4 color_unscaled = ImVec4(0.22f, 0.65f, 0.31f, 1.00f);
	const ImVec4 color_model = ImVec4(0.85f, 0.22f, 0.31f, 1.00f);
	
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
					ImPlot::SetupLegend(ImPlotLocation_SouthEast);
					x_data = io.td.db_sr(i, j);
					y_data = io.td.db_fx(i, j);
					x_model = linspace(-0.17, 0.14, 100);
					y_model = io.tm.get_fx(io.td.load(i), io.td.camber(j), x_model);
					y_model_unscaled = y_model / io.tm.bs;
					break;
				case 1: // FY
					ImPlot::SetupAxis(ImAxis_X1, "SA [°]", x_axis_flags);
					ImPlot::SetupAxis(ImAxis_Y1, "FY [N]", y_axis_flags);
					ImPlot::SetupLegend(ImPlotLocation_NorthEast);
					x_data = io.td.c_sa(i, j);
					y_data = io.td.c_fy(i, j);
					x_model = linspace(-12, 12, 100);
					y_model = io.tm.get_fy(io.td.load(i), io.td.camber(j), x_model);
					y_model_unscaled = y_model / io.tm.as;
					break;
				case 2: // MZ
					ImPlot::SetupAxis(ImAxis_X1, "SA [°]", x_axis_flags);
					ImPlot::SetupAxis(ImAxis_Y1, "MZ [N.m]", y_axis_flags);
					ImPlot::SetupLegend(ImPlotLocation_NorthWest);
					x_data = io.td.c_sa(i, j);
					y_data = io.td.c_mz(i, j);
					x_model = linspace(-12, 12, 100);
					y_model = io.tm.get_mz(io.td.load(i), io.td.camber(j), x_model);
					y_model_unscaled = y_model / io.tm.cs;
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
	
	
	const ImVec2 selector_frame = ImVec2(110, ImGui::GetContentRegionAvail().y);
	ImGui::BeginChild("##selectors", selector_frame, true);
	{
		static int spacing = 5.0;
		ImGui::RadioButton("fx Fit", &io.flags.tire_plot_type, 0);
		ImGui::Dummy(ImVec2(0, spacing));
		ImGui::RadioButton("fy Fit", &io.flags.tire_plot_type, 1);
		ImGui::Dummy(ImVec2(0, spacing));
		ImGui::RadioButton("mz Fit", &io.flags.tire_plot_type, 2);
	}
	ImGui::EndChild();
	ImGui::SameLine();
	
	const ImVec2 variables_frame = ImGui::GetContentRegionAvail();
	ImGui::BeginChild("##variables", variables_frame, true);
	{
		static int ind = 10;
    static int num_consts;
    static ivec precision;
    static fvec add_min, add_max;
    static field<string> num_label;
    static field<string> input_label;
    static field<string> tip_label;
		ImGui::PushItemWidth(130);
		switch (io.flags.tire_plot_type) {
			case 0: // FX
        // Test out if vectors are more easily usable for repeated code
        precision = ivec({
            +2,
            +2,
            +2,
            -3,

            +0,
            +2,
            +1,
            +1,

            +1,
            +3,
            +3,
            +1,

            +1,
            +1,
            +3,
            +2
            }); 
        tip_label = {
          "b0: Shape factor [-]",
          "b1: Load influence on longitudinal friction coefficient [-/kN]",
          "b2: Longitudinal friction coefficient [-]",
          "b3: Curvature factor of stiffness/load [N/-/kN+2]",
          "b4: Change of stiffness with slip [N/-]",
          "b5: Change of progressivity of stiffness / load [-/kN]",
          "b6: Curvature change with load^2 [-/kN+2]",
          "b7: Curvature change with load [-/kN]",
          "b8: Curvature factor [-]",
          "b9: Load influence on horizontal shift [-/kN]",
          "b10: Horizontal shift [-]",
          "b11: Vertical shift [N]",
          "b12: Vertical shift at load = 0 [N]",
          "b13: Curvature shift",
          "(Custom) b14: Camber influence on longitudinal friction coefficient [-/°+2]",
          "(Custom) b15: Curvature change with camber [-/°]",
        }; 
        num_consts = io.tm.b.n_elem; 
        num_label.set_size(num_consts);
        add_min = arma::pow(conv_to<fvec>::from(10 * ones(num_consts)), conv_to<fvec>::from(-precision));
        add_max = 10.0f * add_min;
        for (int i = 0; i < num_consts; ++i){
          if (precision(i) >= 0){
            num_label(i) = "%." + to_string(+precision(i)) + "f";
          }
          else {
            num_label(i) = "%.0f";
          }
          if (!(i % 4)){
            ImGui::BeginGroup();
            ImGui::Indent(ind);
          }
          ImGui::InputDouble(("b" + to_string(i)).c_str(), &io.tm.b[i], add_min(i), add_max(i), num_label(i).c_str());
          ImGui::SetItemTooltip(tip_label(i).c_str());
          if (!((i + 1) % 4) || i == num_consts - 1){
            ImGui::EndGroup();
            ImGui::SameLine();
          }
        }
				break;
			case 1: // FY
        precision = ivec({
            +2,
            +2,
            +2,
            -1,

            +1,
            +2,
            +0,
            +1,

            +1,
            +1,
            +1,
            +1,

            -2,
            -2,
            -2,
            +3,

            +2,
            +2
            }); 
        tip_label = {
          "a0: Shape factor [-]",
          "a1: Load influence on lateral friction coefficient [-/kN]",
          "a2: Lateral friction coefficient [-]",
          "a3: Change of stiffness with slip [N/°]",
          "a4: Change of progressivity of stiffness / load [-/kN]",
          "a5: Camber influence on stiffness [-/°]",
          "a6: Curvature change with load [-/N]",
          "a7: Curvature factor [-]",
          "a8: Load influence on horizontal shift [°/kN]",
          "a9: Horizontal shift at load = 0 and camber = 0 [°]",
          "a10: Camber influence on horizontal shift [°/°]",
          "a11: Load influence on vertical shift [N/N]",
          "a12: Vertical shift at load = 0 [N]",
          "a13: Camber influence on vertical shift, load-dependent [N/°/kN]",
          "a14: Camber influence on vertical shift [N/°]",
          "a15: Camber influence on lateral friction coefficient [-/°+2]",
          "a16: Curvature change with camber [-/°]",
          "a17: Curvature shift [-]"
        }; 
        num_consts = io.tm.a.n_elem; 
        num_label.set_size(num_consts);
        add_min = arma::pow(conv_to<fvec>::from(10 * ones(num_consts)), conv_to<fvec>::from(-precision));
        add_max = 10.0f * add_min;
        for (int i = 0; i < num_consts; ++i){
          if (precision(i) >= 0){
            num_label(i) = "%." + to_string(+precision(i)) + "f";
          }
          else {
            num_label(i) = "%.0f";
          }
          if (!(i % 4)){
            ImGui::BeginGroup();
            ImGui::Indent(ind);
          }
          ImGui::InputDouble(("a" + to_string(i)).c_str(), &io.tm.a[i], add_min(i), add_max(i), num_label(i).c_str());
          ImGui::SetItemTooltip(tip_label(i).c_str());
          if (!((i + 1) % 4) || i == num_consts - 1){
            ImGui::EndGroup();
            ImGui::SameLine();
          }
        }
				break;
			case 2: // MZ
        precision = ivec({
            +2,
            +3,
            +3,
            -1,

            +0,
            +2,
            +1,
            +1,

            +1,
            +1,
            +1,
            +3,

            +0,
            +0,
            +1,
            +2,

            +1,
            +1
            }); 
        tip_label = {
          "c0: Shape factor [-]",
          "c1: Load influence on aligning moment coefficient [-/kN]",
          "c2: Aligning moment coefficient [-]",
          "c3: Change of stiffness with slip [N/°]",
          "c4: Change of progressivity of stiffness / load [-/kN]",
          "c5: Camber influence on stiffness [-/°]",
          "c6: Curvature change with load [-/N]",
          "c7: Curvature factor [-]",
          "c8: Load influence on horizontal shift [°/kN]",
          "c9: Horizontal shift at load = 0 and camber = 0 [°]",
          "c10: Camber influence on horizontal shift [°/°]",
          "c11: Load influence on vertical shift [N/N]",
          "c12: Vertical shift at load = 0 [N]",
          "c13: Camber influence on vertical shift, load-dependent [N/°/kN]",
          "c14: Camber influence on vertical shift [N/°]",
          "c15: Camber influence on aligning moment coefficient [-/°+2]",
          "c16: Curvature change with camber [-/°]",
          "c17: Curvature shift [-]"
        }; 
        num_consts = io.tm.a.n_elem; 
        num_label.set_size(num_consts);
        add_min = arma::pow(conv_to<fvec>::from(10 * ones(num_consts)), conv_to<fvec>::from(-precision));
        add_max = 10.0f * add_min;
        for (int i = 0; i < num_consts; ++i){
          if (precision(i) >= 0){
            num_label(i) = "%." + to_string(+precision(i)) + "f";
          }
          else {
            num_label(i) = "%.0f";
          }
          if (!(i % 4)){
            ImGui::BeginGroup();
            ImGui::Indent(ind);
          }
          ImGui::InputDouble(("c" + to_string(i)).c_str(), &io.tm.c[i], add_min(i), add_max(i), num_label(i).c_str());
          ImGui::SetItemTooltip(tip_label(i).c_str());
          if (!((i + 1) % 4) || i == num_consts - 1){
            ImGui::EndGroup();
            ImGui::SameLine();
          }
        }

				break;
			default: 
				break;
		}
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
}

#endif
