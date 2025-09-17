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
  float track; // track width 
  float steer; // approx -110:110
  float sideslip; // approx. -15:15
  float velocity; // >0
  float brake; // 0-100
  float throttle; // 0-100
  float rad_wheel;
  float front_torque; // 0-100
  float front_brake; // 0-100
  float brk_max; // Nm
  float fx_max; // N

  // motor limits
  float trq_max; 
  float gear;
  float mz_des; // Nm
  // traction limits
  // desired yaw moment 
  //
  // wheel torques 
  float trq[4];
  float trq_fwd[4];
  float trq_reg[4];
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
    trq_max = 31.6; // Nm, per motor
    gear = 7; // reduction 
    track = 1.194; // Track width [m]
    steer = 0; // deg at steering wheel 
    sideslip = 0; // deg 
    velocity = 0; // m/s 
    brake = 0; // % 
    throttle = 0; // % 
    front_torque = 30; // %
    front_brake = 70; // % 
    rad_wheel = 0.200; // m
    brk_max = 600; // Nm, max braking torque at 100% front pressure
    fx_max = 2000; // N, range of progress bars

    mz_des = 0;

    col_bg = ImVec4(0.1f, 0.1f, 0.5f, 1.0f);
    col_regen = ImVec4(0.9f, 0.8f, 0.0f, 1.0f);
    col_motor = ImVec4(0.3f, 0.8f, 0.4f, 1.0f);
    col_brake = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
    col_none = ImVec4(0.8f, 0.3f, 0.4f, 1.0f);
    col_corn = ImVec4(0.7f, 0.1f, 0.7f, 1.0f);
  }

  // Calculate torques, this algorithm should be capable to embed on a microcontroller
  void calculate_torques(){
    
    // Determine even torque/brake distribution per wheel 
    for (int i = 0; i < 4; ++i){
    
      if (i < 2){
     
        trq[i] = throttle / 100 * front_torque / (100 - front_torque) * trq_max * gear;
        trq[i] += brake / 100 * (1) * -trq_max * gear;
        brk[i] = brake / 100 * (1) * brk_max;
     
      }
    
      else {
    
        trq[i] = throttle / 100 * (1) * trq_max * gear;
        trq[i] += brake / 100 * ((100 - front_brake) / front_brake) * -trq_max * gear;
        brk[i] = brake / 100 * ((100 - front_brake) / front_brake) * brk_max;
    
      }
    
    }

    // Approximate desired yaw moment based on inputs 
    mz_des = 1.19 * steer; 

    int mz_dir = (mz_des > 0); // mz direction for which wheels to check max torque allowed 
    float dif_f = (trq_max * gear) - trq[0 + mz_dir]; // difference in front used vs front max 
    float dif_r = (trq_max * gear) - trq[2 + mz_dir]; // (rear)
    float mz_rat = dif_f / (dif_f + dif_r); // mz ratio between front and rear 
    float trq_dlt = mz_des * track * rad_wheel;
    float trq_dlt_f = trq_dlt * mz_rat;
    float trq_dlt_r = trq_dlt * (1 - mz_rat);

    trq[0] -= trq_dlt_f;
    trq[1] += trq_dlt_f;
    trq[2] -= trq_dlt_r;
    trq[3] += trq_dlt_r;
    
    // Assign numerical values for GUI outputs
    for (int i = 0; i < 4; ++i){
      
      trq_fwd[i] = (trq[i] > 0) * trq[i];
      trq_reg[i] = (trq[i] < 0) * -trq[i];
   
      tpb_fwd[i] = trq_fwd[i] / (trq_max * gear);
      tpb_reg[i] = 1 - trq_reg[i] / (trq_max * gear);
      
      bpb[i] = 1 - (trq_reg[i] + brk[i]) / (brk_max + trq_max * gear);
    
    }
  }
};

#endif
