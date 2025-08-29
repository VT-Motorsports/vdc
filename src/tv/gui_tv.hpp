#ifndef GUI_TV_HPP
#define GUI_TV_HPP


class tv_io {
public:
  double sideslip;
  double steer;
  double speed;

  tv_io(){

  }
};

void gui_tv(){
 
  int window_w = ImGui::GetContentRegionAvail().x;
  int window_h = ImGui::GetContentRegionAvail().y;
  int pad = ImGui::GetStyle().ItemSpacing.x;

  int wheel_child_w[4] = {450, 450, 450, 450};
  int wheel_child_h[4] = {0.5*window_h - pad/2, 0.5*window_h - pad/2, 0.5*window_h - pad/2, 0.5*window_h - pad/2};
  int wheel_child_x[4] = {pad, pad + window_w - wheel_child_w[0], pad, pad + window_w - wheel_child_w[0]};
  int wheel_child_y[4] = {pad, pad, 2*pad + wheel_child_h[0], 2*pad + wheel_child_h[0]};

  int median_w = (window_w - 2*wheel_child_w[0] - 2*pad);
  int median_h[] = {60, 150, 250 + 720-window_h, 205, 30};

  for (int i = 0; i < 4; ++i){
    ImGui::SetCursorPos(ImVec2(wheel_child_x[i], wheel_child_y[i]));
    ImGui::BeginChild(("##wheel" + to_string(i)).c_str(), ImVec2(wheel_child_w[i], wheel_child_h[i]), true);
    ImGui::Text(to_string(i).c_str());
    ImGui::EndChild();
  }

  ImGui::SetCursorPos(ImVec2(wheel_child_x[0] + wheel_child_w[0] + pad, wheel_child_y[0]));
  ImGui::BeginChild("##battery", ImVec2(median_w, median_h[0]), true);
  ImGui::Text("battery");
  ImGui::EndChild();

}

#endif 
