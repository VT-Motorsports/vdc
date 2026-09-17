//
// File: tv_engine.cpp
//
// ChassisSim Engine component wrapper around the standalone tv_io torque
// vectoring controller. This is a thin translation layer only: it converts the
// Engine's SI signals into the units tv_io expects, runs calculate_torques on a
// persistent instance, and publishes the four wheel torques. tv_io stays self
// contained and calculate_torques is not reimplemented here.
//
// The reference example (Double_Trial_ert_rtw) wraps its ExtU/ExtY structs and
// step()/initialize() inside a C++ class, but ChassisSim binds the component by
// symbol name (InputVarName/OutputVarName/FunctionName), which requires flat
// exported symbols. So the structs and functions are global with C linkage.
//

// M_PI and the float math helpers tv_io.hpp relies on.
#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// tv_io.hpp uses ImVec4 only for GUI colour fields. Provide a tiny shim so the
// controller header compiles into the DLL without pulling in ImGui; the embedded
// build has no GUI. This does not touch tv_io itself.
struct ImVec4 {
  float x, y, z, w;
  ImVec4(float a = 0, float b = 0, float c = 0, float d = 0) : x(a), y(b), z(c), w(d) {}
};

#include "../tv/tv_io.hpp"
#include "tv_engine.h"

extern "C" {

// Exported signal-exchange instances (flat doubles), bound by name in tv_engine.cmp.
TV_ENGINE_API tv_engine_in_t  tv_in  = {};
TV_ENGINE_API tv_engine_out_t tv_out = {};

// Persistent controller, constructed once with the same defaults as the desktop app.
static tv_io tv;

// Initialize function. InitFnFlg=0 in the component file means ChassisSim does
// not call this; the persistent tv_io constructor already sets every parameter.
TV_ENGINE_API void tv_initialize(void)
{
  // (no initialization code required)
}

// Step function: read tv_in, translate to tv_io units, run the controller,
// write tv_out. All Engine signals are SI.
TV_ENGINE_API void tv_step(void)
{
  // --- pedals: Engine sends 0..1, the controller works in 0..100 ---
  tv.throttle = (float)(tv_in.throttle * 100.0);
  tv.brake    = (float)(tv_in.brake    * 100.0);

  // --- wheel speeds [rad/s] at the wheel, order FL FR RL RR ---
  const float w_fl = (float)tv_in.wheel_speed_fl;
  const float w_fr = (float)tv_in.wheel_speed_fr;
  const float w_rl = (float)tv_in.wheel_speed_rl;
  const float w_rr = (float)tv_in.wheel_speed_rr;

  // Vehicle speed from the average wheel angular speed and the rolling radius.
  const float v_avg = 0.25f * (w_fl + w_fr + w_rl + w_rr) * tv.rad_wheel;
  tv.velocity = v_avg;

  // Kinematic yaw rate from the left-to-right linear-speed difference over the
  // track. Left corners are FL/RL, right corners are FR/RR.
  const float v_left  = 0.5f * (w_fl + w_rl) * tv.rad_wheel;
  const float v_right = 0.5f * (w_fr + w_rr) * tv.rad_wheel;
  const float yaw_rate_radps = (tv.track > 1.0e-4f) ? (v_right - v_left) / tv.track : 0.0f;
  // tv_io carries yaw_rate in deg/s.
  // NOTE: tv_in.yaw_rate is reserved for an unresolved Engine index; until that
  // index is confirmed the yaw rate is derived from the wheel speeds here.
  tv.yaw_rate = yaw_rate_radps * (180.0f / (float)M_PI);

  // Per-corner motor rpm the slip term expects. Internally tv_io reconstructs a
  // wheel linear speed as rpm*(2*pi/60)/gear*rad_wheel; choosing rpm so that
  // value equals the actual wheel linear speed gives rpm = omega*gear*60/(2*pi).
  const float rpm_per_radps = tv.gear * 60.0f / (2.0f * (float)M_PI);
  tv.rpm[0] = w_fl * rpm_per_radps;
  tv.rpm[1] = w_fr * rpm_per_radps;
  tv.rpm[2] = w_rl * rpm_per_radps;
  tv.rpm[3] = w_rr * rpm_per_radps;

  // --- real per-corner vertical loads [N] feed the grip caps directly, in place
  // of tv_io's internal load-transfer model (order FL FR RL RR) ---
  tv.use_ext_loads = true;
  tv.ext_fz[0] = (float)tv_in.wheel_load_fl;
  tv.ext_fz[1] = (float)tv_in.wheel_load_fr;
  tv.ext_fz[2] = (float)tv_in.wheel_load_rl;
  tv.ext_fz[3] = (float)tv_in.wheel_load_rr;

  // --- measured longitudinal acceleration [m/s^2] replaces the internal estimate ---
  tv.use_ext_ax = true;
  tv.ext_ax = (float)tv_in.ax;

  // --- steer angle at the tyre [rad]. tv_io expects steering-wheel degrees and
  // divides by steering_ratio internally, so scale back up to keep the road-wheel
  // angle it derives equal to the supplied tyre angle.
  // NOTE: tv_in.steer is reserved for an unresolved Engine index; defaulted to 0
  // (its zero-initialised value) until that index is confirmed.
  tv.steer = (float)tv_in.steer * (180.0f / (float)M_PI) * tv.steering_ratio;

  // NOTE: tv_in.lat_accel is reserved for an unresolved Engine index and is not
  // yet consumed (tv_io has no external lateral-accel hook); defaulted to 0.

  tv.calculate_torques();

  // --- publish wheel torques [Nm], order FL FR RL RR ---
  tv_out.trq_fl = tv.trq[0];
  tv_out.trq_fr = tv.trq[1];
  tv_out.trq_rl = tv.trq[2];
  tv_out.trq_rr = tv.trq[3];
}

} // extern "C"
