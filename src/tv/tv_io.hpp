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

    // Bicycle model yaw rate estimate
    const float EPS = 1.0e-4f;
    const float steer_road_rad = steer * (M_PI / 180.0f) / steering_ratio;
    float omega_est = 0.0f;
    if (fabsf(velocity) > EPS && fabsf(wheelbase) > EPS) {
      omega_est       = velocity * tanf(steer_road_rad) / wheelbase;
      yaw_rate_target = omega_est * (180.0f / M_PI);
    } else {
      yaw_rate_target = 0.0f;
    }

    // Approximate desired yaw moment based on inputs
    const float yaw_rate_err = yaw_rate_target - yaw_rate;
    const float apex_mod = 1.0f - k_apex * expf(-yaw_accel * yaw_accel / (apex_threshold + EPS));
    mz_des = k_steer * steer + k_yaw_err * yaw_rate_err * apex_mod;

    int mz_dir = (mz_des > 0.0f) ? 1 : 0;
    float dif_f = (trq_max * gear) - trq[0 + mz_dir];
    float dif_r = (trq_max * gear) - trq[2 + mz_dir];
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
    const float trq_limit = trq_max * gear;
    for (int i = 0; i < 4; ++i) {
      float slip_delta = wheel_speed[i] - expected_speed[i];
      trq[i] -= k_rpm * slip_delta;
      if (trq[i] >  trq_limit) trq[i] =  trq_limit;
      if (trq[i] < -trq_limit) trq[i] = -trq_limit;
    }

    // Re-distribute clamping loss to preserve total torque request
    float total_after = trq[0] + trq[1] + trq[2] + trq[3];
    float correction = (total_before - total_after) * 0.25f;
    for (int i = 0; i < 4; ++i) {
      trq[i] += correction;
      if (trq[i] >  trq_limit) trq[i] =  trq_limit;
      if (trq[i] < -trq_limit) trq[i] = -trq_limit;
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
