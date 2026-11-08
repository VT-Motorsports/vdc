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
  float brk_max; // Nm, front, per wheel
  float brk_max_r; // Nm, rear, per wheel
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
  bool mz_off; // when true, no yaw moment is requested

  // power limits and yaw accel state
  bool endurance; // true: 15 kW per motor / 60 kW total, false: 25 kW / 80 kW
  float dt; // [s], time between calculate_torques calls
  float yaw_rate_prev; // [°/s]
  bool have_yaw_prev;

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
    gear = 12.875; // reduction
    track = 1.194; // Track width [m]
    steer = 0; // deg at steering wheel
    sideslip = 0; // deg
    velocity = 0; // m/s
    brake = 0; // %
    throttle = 0; // %
    front_torque = 30; // %
    front_brake = 70; // %
    rad_wheel = 0.2032f; // m
    brk_max = 599.3f; // Nm, front, per wheel
    brk_max_r = 213.5f; // Nm, rear, per wheel
    fx_max = 2000; // N, range of progress bars

    mz_des = 0;

    for (int i = 0; i < 4; ++i) rpm[i] = 0.0f;
    wheelbase = 1.537f; // m
    steering_ratio = 5.312f; // [-]
    k_rpm = 5.0f; // Nm·s/m

    yaw_rate = 0.0f;
    yaw_accel = 0.0f;
    yaw_rate_target = 0.0f;
    k_steer = 1.6965f;
    k_yaw_err = 2.8513f;
    k_apex = 0.5f;
    apex_threshold = 100.0f; // °²/s⁴

    for (int i = 0; i < 4; ++i) { fz[i] = 0.0f; trq_cap[i] = 0.0f; }
    use_ext_loads = false;
    for (int i = 0; i < 4; ++i) ext_fz[i] = 0.0f;
    use_ext_ax = false;
    ext_ax = 0.0f;
    mz_off = false;
    endurance = true;
    dt = 0.001f; // s
    yaw_rate_prev = 0.0f;
    have_yaw_prev = false;
    mass = 294.8f; // kg
    h_cg = 0.234f; // m
    weight_dist = 0.492f; // front fraction
    c_f = 32000.0f; // N/rad, front axle
    c_r = 38000.0f; // N/rad, rear axle
    mu = 1.7f; // [-]
    grav = 9.81f; // m/s²

    col_bg = ImVec4(0.1f, 0.1f, 0.5f, 1.0f);
    col_regen = ImVec4(0.9f, 0.8f, 0.0f, 1.0f);
    col_motor = ImVec4(0.3f, 0.8f, 0.4f, 1.0f);
    col_brake = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);
    col_none = ImVec4(0.8f, 0.3f, 0.4f, 1.0f);
    col_corn = ImVec4(0.7f, 0.1f, 0.7f, 1.0f);
  }

  // Motor shaft torque [Nm] per motor against motor rpm, zero above the rev limit
  float motor_trq(float rpm_m) const {
    static const float bp[]  = { 0.0f, 11000.0f, 12000.0f, 14000.0f, 16000.0f, 18000.0f, 20000.0f };
    static const float trq_bp[] = { 31.6f, 31.6f, 31.0f, 29.0f, 25.0f, 21.0f, 18.0f };
    const float r = fabsf(rpm_m);
    if (r > bp[6]) return 0.0f;
    for (int k = 0; k < 6; ++k) {
      if (r <= bp[k + 1]) return trq_bp[k] + (r - bp[k]) / (bp[k + 1] - bp[k]) * (trq_bp[k + 1] - trq_bp[k]);
    }
    return trq_bp[6];
  }

  // Calculate torques, this algorithm should be capable to embed on a microcontroller
  void calculate_torques(){

    const float EPS = 1.0e-4f;
    const float RPM_TO_RADPS = 2.0f * M_PI / 60.0f;
    const float p_motor = endurance ? 15000.0f : 25000.0f; // W
    const float p_total = endurance ? 60000.0f : 80000.0f; // W

    if (have_yaw_prev && dt > EPS) yaw_accel = (yaw_rate - yaw_rate_prev) / dt;
    yaw_rate_prev = yaw_rate;
    have_yaw_prev = true;

    // Determine even torque/brake distribution per wheel
    for (int i = 0; i < 4; ++i) {
      if (i < 2) {
        trq[i]  = throttle / 100.0f * (front_torque / fmaxf(100.0f - front_torque, EPS)) * trq_max * gear;
        trq[i] += brake    / 100.0f * (-1.0f) * trq_max * gear;
        brk[i]  = brake    / 100.0f * brk_max;
      } else {
        trq[i]  = throttle / 100.0f * trq_max * gear;
        trq[i] += brake    / 100.0f * ((100.0f - front_brake) / fmaxf(front_brake, EPS)) * (-trq_max * gear);
        brk[i]  = brake    / 100.0f * brk_max_r;
      }
    }

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
      const float w_motor = fabsf(rpm[i]) * RPM_TO_RADPS;
      float trq_motor = motor_trq(rpm[i]);
      if (w_motor > EPS && trq_motor > p_motor / w_motor) trq_motor = p_motor / w_motor;
      const float act_lim = trq_motor * gear;
      if (trq_cap[i] > act_lim) trq_cap[i] = act_lim;
    }

    // Single-track yaw rate target with understeer gradient and friction cap
    const float steer_road_rad = steer * (M_PI / 180.0f) / steering_ratio;
    float omega_est = 0.0f; // kinematic estimate, reused for wheel-speed expectations
    if (fabsf(velocity) > EPS && fabsf(wheelbase) > EPS) {
      omega_est = velocity * tanf(steer_road_rad) / wheelbase;
      // Understeer gradient from axle cornering stiffness [rad·s²/m]
      const float k_us = mass * (weight_dist / c_f - (1.0f - weight_dist) / c_r);
      const float den_us = fmaxf(wheelbase + k_us * velocity * velocity, EPS); // K_us < 0 guard
      float omega_ss = velocity * steer_road_rad / den_us;
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
    if (fabsf(velocity) < EPS || mz_off) mz_des = 0.0f; // no torque split at standstill or when disabled

    int mz_dir = (mz_des > 0.0f) ? 1 : 0;
    float dif_f = trq_cap[0 + mz_dir] - trq[0 + mz_dir];
    float dif_r = trq_cap[2 + mz_dir] - trq[2 + mz_dir];
    if (dif_f < 0.0f) dif_f = 0.0f; // unloaded corner offers no headroom
    if (dif_r < 0.0f) dif_r = 0.0f;
    float denom = dif_f + dif_r;
    float mz_rat = (fabsf(denom) > EPS) ? dif_f / denom : 0.5f;
    float trq_dlt = (track > EPS) ? mz_des * rad_wheel / track : 0.0f;

    // Per-axle limit: gaining wheel's room up to +cap, losing wheel's room down to -cap
    float lim_f = fminf(dif_f, trq[1 - mz_dir] + trq_cap[1 - mz_dir]);
    float lim_r = fminf(dif_r, trq[3 - mz_dir] + trq_cap[3 - mz_dir]);
    if (lim_f < 0.0f) lim_f = 0.0f;
    if (lim_r < 0.0f) lim_r = 0.0f;
    const float mz_sgn = (trq_dlt < 0.0f) ? -1.0f : 1.0f;
    float trq_dlt_f = fabsf(trq_dlt) * mz_rat;
    float trq_dlt_r = fabsf(trq_dlt) - trq_dlt_f;
    float spill = 0.0f;
    if (trq_dlt_f > lim_f) { spill += trq_dlt_f - lim_f; trq_dlt_f = lim_f; }
    if (trq_dlt_r > lim_r) { spill += trq_dlt_r - lim_r; trq_dlt_r = lim_r; }
    float give = fminf(spill, lim_f - trq_dlt_f);
    trq_dlt_f += give;
    spill -= give;
    trq_dlt_r += fminf(spill, lim_r - trq_dlt_r); // remainder beyond both limits is dropped

    trq[0] -= mz_sgn * trq_dlt_f;
    trq[1] += mz_sgn * trq_dlt_f;
    trq[2] -= mz_sgn * trq_dlt_r;
    trq[3] += mz_sgn * trq_dlt_r;

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

    // Slip backoff reduces the total and is not refunded; only clamp loss is redistributed
    bool slipping[4];
    float lost = 0.0f;
    for (int i = 0; i < 4; ++i) {
      float slip_delta = wheel_speed[i] - expected_speed[i];
      slipping[i] = (trq[i] > 0.0f && slip_delta > 0.0f) || (trq[i] < 0.0f && slip_delta < 0.0f);
      float clamped = trq[i];
      if (clamped >  trq_cap[i]) clamped =  trq_cap[i];
      if (clamped < -trq_cap[i]) clamped = -trq_cap[i];
      lost += trq[i] - clamped;
      trq[i] = clamped - k_rpm * slip_delta; // backoff from the clamped torque
      if (trq[i] >  trq_cap[i]) trq[i] =  trq_cap[i];
      if (trq[i] < -trq_cap[i]) trq[i] = -trq_cap[i];
    }

    // Give clamp loss to non-slipping wheels in proportion to their room in that direction
    for (int pass = 0; pass < 4 && fabsf(lost) > EPS; ++pass) {
      const float sgn = (lost > 0.0f) ? 1.0f : -1.0f;
      float room[4];
      float room_sum = 0.0f;
      for (int i = 0; i < 4; ++i) {
        room[i] = slipping[i] ? 0.0f : trq_cap[i] - sgn * trq[i];
        if (room[i] < 0.0f) room[i] = 0.0f;
        room_sum += room[i];
      }
      if (room_sum <= EPS) break;
      const float frac = fminf(fabsf(lost) / room_sum, 1.0f);
      for (int i = 0; i < 4; ++i) {
        trq[i] += sgn * room[i] * frac;
        lost   -= sgn * room[i] * frac;
      }
    }

    // Total drive power limit, sum of T_wheel * w_wheel over the driving corners
    float p_drive = 0.0f;
    for (int i = 0; i < 4; ++i) {
      if (trq[i] > 0.0f) p_drive += trq[i] * fabsf(rpm[i]) * RPM_TO_RADPS / gear;
    }
    if (p_drive > p_total) {
      const float scale = p_total / p_drive;
      for (int i = 0; i < 4; ++i) {
        if (trq[i] > 0.0f) trq[i] *= scale;
      }
    }

    // Assign numerical values for GUI outputs
    for (int i = 0; i < 4; ++i){
      trq_fwd[i] = (trq[i] > 0.0f) ?  trq[i] : 0.0f;
      trq_reg[i] = (trq[i] < 0.0f) ? -trq[i] : 0.0f;
      tpb_fwd[i] = trq_fwd[i] / (trq_max * gear);
      tpb_reg[i] = 1.0f - trq_reg[i] / (trq_max * gear);
      bpb[i] = 1.0f - (trq_reg[i] + brk[i]) / (((i < 2) ? brk_max : brk_max_r) + trq_max * gear);
    }
  }
};

#endif
