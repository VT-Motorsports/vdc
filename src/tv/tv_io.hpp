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
  float track; // track width [m]
  float steer; // approx -110:110
  float sideslip; // approx. -15:15
  float velocity; // >0
  float brake; // 0-100
  float throttle; // 0-100
  float rad_wheel; // [m]
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

  // rpm per corner, wheelbase, steering ratio
  float rpm[4];
  float wheelbase; // [m]
  float steering_ratio; // [-]
  float k_rpm; // [Nm·s/m]

  // yaw state inputs and feedback gains
  float yaw_rate; // [°/s]
  float yaw_accel; // [°/s²]
  float yaw_rate_target; // [°/s]
  float k_steer; // [-]
  float k_yaw_err; // [-]
  float k_apex; // [-]
  float apex_threshold; // [°²/s⁴]

  // per-corner vertical load and grip-limited corner torque
  float fz[4]; // N, vertical load per corner (down-positive)
  float trq_cap[4]; // Nm, grip- or actuator-limited corner torque

  // external override hooks (set only by the embedded/ChassisSim wrapper; left
  // off for the desktop GUI and standalone use so behaviour is bit-for-bit unchanged)
  bool use_ext_loads; // when true, fz[] is taken from ext_fz[] not the internal transfer
  float ext_fz[4]; // N, per-corner vertical load supplied externally (FL FR RL RR)
  bool use_ext_ax; // when true, ax_est is taken from ext_ax not the internal estimate
  float ext_ax; // m/s², longitudinal acceleration supplied externally

  // load transfer and grip constants
  float mass; // [kg]
  float h_cg; // CG height [m]
  float weight_dist; // front weight fraction [-]
  float c_f; // front axle cornering stiffness [N/rad]
  float c_r; // rear axle cornering stiffness [N/rad]
  float mu; // tire-road friction coefficient [-]
  float grav; // [m/s²]


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

    for (int i = 0; i < 4; ++i) rpm[i] = 0.0f;
    wheelbase = 1.525f; // m
    steering_ratio = 3.5f; // [-]
    k_rpm = 5.0f; // Nm·s/m

    yaw_rate = 0.0f;
    yaw_accel = 0.0f;
    yaw_rate_target = 0.0f;
    k_steer = 1.19f;
    k_yaw_err = 2.0f;
    k_apex = 0.5f;
    apex_threshold = 100.0f; // °²/s⁴

    for (int i = 0; i < 4; ++i) { fz[i] = 0.0f; trq_cap[i] = 0.0f; }
    use_ext_loads = false;
    for (int i = 0; i < 4; ++i) ext_fz[i] = 0.0f;
    use_ext_ax = false;
    ext_ax = 0.0f;
    mass = 305.0f; // kg
    h_cg = 0.315f; // m
    weight_dist = 0.467f; // front fraction
    c_f = 40000.0f; // N/rad, front axle
    c_r = 50000.0f; // N/rad, rear axle
    mu = 1.5f; // [-]
    grav = 9.81f; // m/s²

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
    for (int i = 0; i < 4; ++i) {
      if (i < 2) {
        trq[i]  = throttle / 100.0f * (front_torque / (100.0f - front_torque)) * trq_max * gear;
        trq[i] += brake    / 100.0f * (-1.0f) * trq_max * gear;
        brk[i]  = brake    / 100.0f * brk_max;
      } else {
        trq[i]  = throttle / 100.0f * trq_max * gear;
        trq[i] += brake    / 100.0f * ((100.0f - front_brake) / front_brake) * (-trq_max * gear);
        brk[i]  = brake    / 100.0f * ((100.0f - front_brake) / front_brake) * brk_max;
      }
    }

    const float EPS = 1.0e-4f;

    // Per-corner vertical load from static split, lateral and longitudinal transfer
    static const float lat_sign_z[4]  = { -1.0f, +1.0f, -1.0f, +1.0f };
    static const float long_sign_z[4] = { -1.0f, -1.0f, +1.0f, +1.0f };
    float fx_net = 0.0f;
    for (int i = 0; i < 4; ++i) fx_net += (trq[i] - brk[i]) / rad_wheel;
    const float ax_est = use_ext_ax ? ext_ax : (fx_net / mass); // long. accel: external when supplied, else from net drive/brake demand
    const float ay_est = velocity * (yaw_rate * (M_PI / 180.0f)); // lat. accel from velocity & yaw rate
    const float dz_lat_f = weight_dist          * mass * ay_est * h_cg / track;
    const float dz_lat_r = (1.0f - weight_dist) * mass * ay_est * h_cg / track;
    const float dz_long  = mass * ax_est * h_cg / wheelbase * 0.5f;
    for (int i = 0; i < 4; ++i) {
      if (use_ext_loads) {
        fz[i] = ext_fz[i]; // real per-corner load supplied externally
      } else {
        float fz_st  = ((i < 2) ? weight_dist : (1.0f - weight_dist)) * mass * grav * 0.5f;
        float fz_lat = lat_sign_z[i]  * ((i < 2) ? dz_lat_f : dz_lat_r);
        float fz_lng = long_sign_z[i] * dz_long;
        fz[i] = fz_st + fz_lat + fz_lng;
      }
      if (fz[i] < 0.0f) fz[i] = 0.0f; // a lifted wheel carries no load
      // Grip-limited corner torque, never above the actuator ceiling
      trq_cap[i] = mu * fz[i] * rad_wheel;
      const float act_lim = trq_max * gear;
      if (trq_cap[i] > act_lim) trq_cap[i] = act_lim;
    }

    // Single-track yaw rate target with understeer gradient and friction cap
    const float steer_road_rad = steer * (M_PI / 180.0f) / steering_ratio;
    float omega_est = 0.0f; // kinematic estimate, reused for wheel-speed expectations
    if (fabsf(velocity) > EPS && fabsf(wheelbase) > EPS) {
      omega_est = velocity * tanf(steer_road_rad) / wheelbase;
      // Understeer gradient from axle cornering stiffness [rad·s²/m]
      const float k_us = mass * (weight_dist / c_f - (1.0f - weight_dist) / c_r);
      float omega_ss = velocity * steer_road_rad / (wheelbase + k_us * velocity * velocity);
      // Friction-limited yaw rate ceiling [rad/s]
      const float omega_max = mu * grav / fabsf(velocity);
      if (omega_ss >  omega_max) omega_ss =  omega_max;
      if (omega_ss < -omega_max) omega_ss = -omega_max;
      yaw_rate_target = omega_ss * (180.0f / M_PI);
    } else {
      yaw_rate_target = 0.0f;
    }

    // Approximate desired yaw moment based on inputs
    const float yaw_rate_err = yaw_rate_target - yaw_rate;
    const float apex_mod = 1.0f - k_apex * expf(-yaw_accel * yaw_accel / (apex_threshold + EPS));
    mz_des = k_steer * steer + k_yaw_err * yaw_rate_err * apex_mod;

    int mz_dir = (mz_des > 0.0f) ? 1 : 0;
    float dif_f = trq_cap[0 + mz_dir] - trq[0 + mz_dir];
    float dif_r = trq_cap[2 + mz_dir] - trq[2 + mz_dir];
    if (dif_f < 0.0f) dif_f = 0.0f; // unloaded corner offers no headroom
    if (dif_r < 0.0f) dif_r = 0.0f;
    float denom = dif_f + dif_r;
    float mz_rat = (fabsf(denom) > EPS) ? dif_f / denom : 0.5f;
    float trq_dlt = mz_des * track * rad_wheel;
    float trq_dlt_f = trq_dlt * mz_rat;
    float trq_dlt_r = trq_dlt * (1.0f - mz_rat);

    trq[0] -= trq_dlt_f;
    trq[1] += trq_dlt_f;
    trq[2] -= trq_dlt_r;
    trq[3] += trq_dlt_r;

    // RPM-adaptive slip correction per corner
    float wheel_speed[4];
    for (int i = 0; i < 4; ++i) {
      wheel_speed[i] = rpm[i] * (2.0f * M_PI / 60.0f) / gear * rad_wheel;
    }

    static const float lat_sign[4] = { -1.0f, +1.0f, -1.0f, +1.0f };
    float expected_speed[4];
    for (int i = 0; i < 4; ++i) {
      expected_speed[i] = (fabsf(velocity) > EPS)
          ? velocity + omega_est * (lat_sign[i] * track * 0.5f)
          : 0.0f;
    }

    float total_before = trq[0] + trq[1] + trq[2] + trq[3];
    for (int i = 0; i < 4; ++i) {
      float slip_delta = wheel_speed[i] - expected_speed[i];
      trq[i] -= k_rpm * slip_delta;
      if (trq[i] >  trq_cap[i]) trq[i] =  trq_cap[i];
      if (trq[i] < -trq_cap[i]) trq[i] = -trq_cap[i];
    }

    // Re-distribute clamping loss to preserve total torque request
    float total_after = trq[0] + trq[1] + trq[2] + trq[3];
    float correction = (total_before - total_after) * 0.25f;
    for (int i = 0; i < 4; ++i) {
      trq[i] += correction;
      if (trq[i] >  trq_cap[i]) trq[i] =  trq_cap[i];
      if (trq[i] < -trq_cap[i]) trq[i] = -trq_cap[i];
    }

    // Assign numerical values for GUI outputs
    for (int i = 0; i < 4; ++i){
      trq_fwd[i] = (trq[i] > 0.0f) ?  trq[i] : 0.0f;
      trq_reg[i] = (trq[i] < 0.0f) ? -trq[i] : 0.0f;
      tpb_fwd[i] = trq_fwd[i] / (trq_max * gear);
      tpb_reg[i] = 1.0f - trq_reg[i] / (trq_max * gear);
      bpb[i] = 1.0f - (trq_reg[i] + brk[i]) / (brk_max + trq_max * gear);
    }
  }
};

#endif
