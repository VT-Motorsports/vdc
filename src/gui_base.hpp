#ifndef GUI_BASE_HPP
#define GUI_BASE_HPP

inline void gui_base(gui_io &io){
	
  float next_item_offset = 0.5f * (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("  ___  _____  _____    ___  ___   ________ ").x);

  ImGui::Dummy(ImVec2(0, 200));
  ImGui::Dummy(ImVec2(next_item_offset, 0));

  ImGui::SameLine(0, 0);

	ImGui::Text(
    "  ___  _____  _____    ___  ___   ________ \n"
    "  | | / / _ \\/ ___/__ / _ )/ _ | / __/ __/\n"
    "  | |/ / // / /__ /_// _  / __ |_\\ \\/ _/ \n"
    "  |___/____/\\___/   /____/_/ |_|___/___/    "
    );
	
	float input_box_width = 1.1f * ImGui::CalcTextSize("Put drive/brake run.dat (ASCII_SI) filepath here").x;
  float button_width = ImGui::CalcTextSize("Load from file").x;
  float spacing = 10;
  next_item_offset = 0.5f * (ImGui::GetContentRegionAvail().x - input_box_width - button_width);
  
  ImGui::Dummy(ImVec2(0, spacing)); 
  ImGui::Dummy(ImVec2(next_item_offset, 0));
  
  ImGui::SameLine(0, 0);
  
  static char tc_path[128] = "";
  ImGui::PushItemWidth(input_box_width);
	ImGui::InputTextWithHint("##crun.dat", "Put cornering run.dat (ASCII_SI) filepath here", tc_path, IM_ARRAYSIZE(tc_path));
  ImGui::PopItemWidth();
  
  ImGui::SameLine(0, spacing);
  
  ImGui::PushItemWidth(button_width);
	if(ImGui::Button("Load from file##tc")){load_dat_cornering(io.td, tc_path, 82.7);}
  ImGui::PopItemWidth();

	if (io.td.c_ia(0, 0).n_elem) {
    ImGui::SameLine(0, spacing);
    ImGui::Text(":)");
		io.flags.is_tc_loaded = true;
	}
	
  ImGui::Dummy(ImVec2(0, spacing)); 
  ImGui::Dummy(ImVec2(next_item_offset, 0));
  
  ImGui::SameLine(0, 0);
  
	static char td_path[128] = "";
  ImGui::PushItemWidth(input_box_width);
	ImGui::InputTextWithHint("##dbrun.dat", "Put drive/brake run.dat (ASCII_SI) filepath here", td_path, IM_ARRAYSIZE(td_path));
  ImGui::PopItemWidth();
  
  ImGui::SameLine(0, spacing);

  ImGui::PushItemWidth(button_width);
	if(ImGui::Button("Load from file##tdb")){load_dat_drivebrake(io.td, td_path, 82.7);}
  ImGui::PopItemWidth();

	if (io.td.db_ia(0, 0).n_elem) {
    ImGui::SameLine(0, spacing);
    ImGui::Text(":)");
		io.flags.is_tdb_loaded = true;
	}
	
  float gui_button_offset = 0.5f * (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Tire GUI").x);

	if (!(io.flags.is_tc_loaded && io.flags.is_tdb_loaded)){ImGui::BeginDisabled();}
  ImGui::Dummy(ImVec2(0, spacing)); 
  ImGui::Dummy(ImVec2(gui_button_offset, 0)); 
  ImGui::SameLine(0, 0);
	if(ImGui::Button("Tire GUI")){current_gui = tire;}
	if (!(io.flags.is_tc_loaded && io.flags.is_tdb_loaded)){ImGui::EndDisabled();}

  ImGui::Dummy(ImVec2(0, spacing)); 
  ImGui::Dummy(ImVec2(next_item_offset, 0));
  
  ImGui::SameLine(0, 0);

	static char car_path[128] = "";
  ImGui::PushItemWidth(input_box_width);
	ImGui::InputTextWithHint("##vehicle.json", "Put vehicle .json filepath here", car_path, IM_ARRAYSIZE(car_path));
  ImGui::PopItemWidth();
  
  ImGui::SameLine(0, spacing);

  ImGui::PushItemWidth(button_width);
	if(ImGui::Button("Load from file##car")){
    load_vehicle(io.car, car_path);
    io.flags.is_veh_loaded = true;
  }
  ImGui::PopItemWidth();
  

	if (io.flags.is_veh_loaded) {
    ImGui::SameLine(0, spacing);
		ImGui::Text(":)");
	}

	gui_button_offset = 0.5f * (ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("YMD GUI").x);

	if (!(io.flags.is_veh_loaded)){ImGui::BeginDisabled();}
  ImGui::Dummy(ImVec2(0, spacing)); 
  ImGui::Dummy(ImVec2(gui_button_offset, 0)); 
  ImGui::SameLine(0, 0);
	if(ImGui::Button("YMD GUI")){current_gui = ymd;}
	if (!(io.flags.is_veh_loaded)){ImGui::EndDisabled();}
	
}

#endif
