#ifndef TV_IO_HPP
#define TV_IO_HPP

class tv_io {
public:
  // shared color data 
  ImVec4 col_regen, col_motor, col_brake, col_none, col_corn, col_bg; 

  // steer
  // sideslip
  // velocity
  // etc. inputs to the algorithm
  float steer; // approx -110:110
  float sideslip; // approx. -15:15
  float velocity; // >0
  float brake; // 0-1
  float throttle; // 0-1

  // motor limits
  // traction limits
  // desired yaw moment 
  //
  // wheel torques 
  // fx in corner frame
  // fy in corner frame 
  // mz in corner frame
  tv_io(){
    steer = 0;
    sideslip = 0;
    velocity = 0;
    brake = 0;
    throttle = 0;
    col_bg = ImVec4(0.1f, 0.1f, 0.5f, 1.0f);
    col_regen = ImVec4(0.9f, 0.8f, 0.0f, 1.0f);
    col_motor = ImVec4(0.3f, 0.8f, 0.4f, 1.0f);
    col_brake = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
    col_none = ImVec4(0.8f, 0.3f, 0.4f, 1.0f);
    col_corn = ImVec4(0.7f, 0.1f, 0.7f, 1.0f);
  }
};

#endif
