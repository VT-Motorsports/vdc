#ifndef DRAW_PLOT_HPP
#define DRAW_PLOT_HPP
void draw_plot(ymd_v_io &io) {
	for (int k = 0; k < io.n_slices; ++k) {
		// Style lineweights according to selected plot
		if (k == io.ax_plot) {ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 1.2f);}
		else {ImPlot::PushStyleVar(ImPlotStyleVar_LineWeight, 0.7f);}
		// Style line colors
		ImVec4 line_color = ImPlot::SampleColormap(static_cast<float>(k + 1) / io.n_slices, ImPlotColormap_Jet);
		if (k == io.ax_plot){line_color.w = 1.0;}
		else {line_color.w = 0.3;}
		ImPlot::PushStyleColor(ImPlotCol_Line, line_color);
		// Yaw Isolines
		for (int i = 0; i < io.n_rows; ++i) {
			double yaw_iso_ay[io.n_cols];
			double yaw_iso_aa[io.n_cols];
			for (int j = 0; j < io.n_cols; ++j) {
				yaw_iso_ay[j] = io.ay(i, j, k);
				yaw_iso_aa[j] = io.aa(i, j, k);
			}
			ImPlot::PlotLine("##", yaw_iso_ay, yaw_iso_aa, io.n_cols);
		}
		// Steer Isolines
		for (int j = 0; j < io.n_cols; ++j) {
			double steer_iso_ay[io.n_rows];
			double steer_iso_aa[io.n_rows];
			for (int i = 0; i < io.n_rows; ++i) {
				steer_iso_ay[i] = io.ay(i, j, k);
				steer_iso_aa[i] = io.aa(i, j, k);
			}
			ImPlot::PlotLine("##", steer_iso_ay, steer_iso_aa, io.n_rows);
		}
		ImPlot::PopStyleVar();
		ImPlot::PopStyleColor();
		// Stability/Control Scatter
		if (k == io.ax_plot) {
			for (int j = 0; j < io.n_cols; ++j) {
				for (int i = 0; i < io.n_rows; ++i) {
					ImVec4 fill_color;
					if (io.plot_type == 1 && !isnan(io.stb_norm(i, j, k))) { // Color by stability
						fill_color = ImPlot::SampleColormap(io.stb_norm(i, j, k), ImPlotColormap_Jet);

					}
					else if (io.plot_type == 2 && !isnan(io.cnt_norm(i, j, k))) { // Color by control
						fill_color = ImPlot::SampleColormap(io.cnt_norm(i, j, k), ImPlotColormap_Jet);
					}
					else {
						fill_color = ImPlot::SampleColormap(1.0f, ImPlotColormap_Greys);
					}
					ImPlot::PushStyleColor(ImPlotCol_MarkerFill, fill_color);
					ImPlot::PlotScatter("##scatter", &io.ay(i, j, k), &io.aa(i, j, k), 1);
					ImPlot::PopStyleColor();
				}
			}
		}
	}
}
#endif
