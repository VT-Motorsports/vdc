#ifndef CHECK_KEY_PRESS_HPP
#define CHECK_KEY_PRESS_HPP

void check_key_press(gui_io &io){
  if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_LeftAlt) && ImGui::IsKeyDown(ImGuiKey_LeftShift)){
    io.flags.key_held = 7;
  }
  else if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsKeyDown(ImGuiKey_LeftAlt)){
    io.flags.key_held = 6;
  }
  else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_LeftAlt)){
    io.flags.key_held = 5;
  }
  else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_LeftShift)){
    io.flags.key_held = 4;
  }
  else if (ImGui::IsKeyDown(ImGuiKey_LeftAlt)){
    io.flags.key_held = 3;
  }
  else if (ImGui::IsKeyDown(ImGuiKey_LeftShift)){
    io.flags.key_held = 2;
  }
  else if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl)){
    io.flags.key_held = 1;
  }
  else {
    io.flags.key_held = 0;
  }
}

#endif
