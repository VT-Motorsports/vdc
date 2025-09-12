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
  float brake; // 0-100
  float throttle; // 0-100
  float front_brake; // 0-100
  float max_brake_torque; // Nm
  float max_force_scale; // N

  // motor limits
  float trq_max; 
  float gear;
  // traction limits
  // desired yaw moment 
  //
  // wheel torques 
  float trq[4];
  float brk[4]; 
  // fx in corner frame
  // fy in corner frame 
  // mz in corner frame
  
  // progress bars 
  float tpb_fwd[4];
  float tpb_reg[4];
  float bpb[4]; 


  // Initialize all values, meant for C++ GUI implementation and not embedded C
  tv_io(){
    trq_max = 31.6;
    gear = 7;
    steer = 0;
    sideslip = 0;
    velocity = 0;
    brake = 0;
    throttle = 0;
    front_brake = 70;
    rad_wheel = 0.200; // m
    max_brake_torque = 600; // Nm, max braking torque at 100% front pressure
    max_force_scale = 2000; // N, range of progress bars
    
    for (int i = 0; i < 4; ++i){
      trq[i] = 0;
      tpb_fwd[i] = 0; // torque progress bars
      tpb_reg[i] = 0;
    }

    col_bg = ImVec4(0.1f, 0.1f, 0.5f, 1.0f);
    col_regen = ImVec4(0.9f, 0.8f, 0.0f, 1.0f);
    col_motor = ImVec4(0.3f, 0.8f, 0.4f, 1.0f);
    col_brake = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
    col_none = ImVec4(0.8f, 0.3f, 0.4f, 1.0f);
    col_corn = ImVec4(0.7f, 0.1f, 0.7f, 1.0f);
  }

  // Calculate torques, this algorithm should be capable to embed on a microcontroller
  void calculate_torques(){
    for (int i = 0; i < 4; ++i){
      trq[i] = throttle / 100 * trq_max;
      trq_fwd[i] = (trq[i] > 0) * trq[i];
      trq_reg[i] = (trq[i] < 0) * -trq[i];
      tpb_fwd[i] = trq_fwd[i] / trq_max;
      brk[i] = (brake / 100) * ((i < 2) * front_brake / 100 + (i >= 2) * (1 - front_brake / 100)) * max_brake_torque;
      bpb[i] = tpb_reg[i] + 1 - (brk[i] / max_brake_torque);
    }
  }
};

#endif
