//
// File: engine_map.cpp
//
// Minimal ChassisSim Engine component for debugging the DLL pipeline: the step
// function is nothing but throttle x a torque looked up against engine rpm,
// split evenly over the four wheels. No torque vectoring, no load transfer, no
// yaw logic, and no dependency on tv_io or tv_engine.
//
// ChassisSim binds the component by symbol name
// (InputVarName/OutputVarName/FunctionName), which requires flat exported
// symbols, so the structs and functions are global with C linkage.
//

#include "engine_map.h"

// Wide-open-throttle total wheel torque against engine rpm. Monotonically
// increasing breakpoints; values between points are linearly interpolated and
// values outside the ends are clamped to the first / last entry.
static const real_T kRpmBp[]  = {    0.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 6000.0, 7000.0 };
static const real_T kTrqWot[] = {  200.0,  600.0,  900.0, 1050.0, 1100.0, 1050.0,  900.0,  600.0 }; // [Nm] total at the wheels
static const int kNumBp = (int)(sizeof(kRpmBp) / sizeof(kRpmBp[0]));

// Linear interpolation of the table above at the given rpm, clamped at both ends.
static real_T lookup_wot_torque(real_T rpm)
{
  if (rpm <= kRpmBp[0]) {
    return kTrqWot[0];
  }
  if (rpm >= kRpmBp[kNumBp - 1]) {
    return kTrqWot[kNumBp - 1];
  }

  for (int i = 0; i < kNumBp - 1; ++i) {
    if (rpm <= kRpmBp[i + 1]) {
      const real_T span = kRpmBp[i + 1] - kRpmBp[i];
      const real_T frac = (span > 0.0) ? (rpm - kRpmBp[i]) / span : 0.0;
      return kTrqWot[i] + frac * (kTrqWot[i + 1] - kTrqWot[i]);
    }
  }

  // Unreachable given the clamps above; keep the last value for safety.
  return kTrqWot[kNumBp - 1];
}

extern "C" {

// Exported signal-exchange instances (flat doubles), bound by name in engine_map.cmp.
ENGINE_MAP_API engine_map_in_t  map_in  = {};
ENGINE_MAP_API engine_map_out_t map_out = {};

// Initialize function. InitFnFlg=0 in the component file means ChassisSim does
// not call this; the table is static and needs no setup.
ENGINE_MAP_API void map_initialize(void)
{
  // (no initialization code required)
}

// Step function: read map_in, evaluate the map, write map_out.
ENGINE_MAP_API void map_step(void)
{
  // Throttle arrives as 0..1; guard against out-of-range demands.
  real_T throttle = map_in.throttle;
  if (throttle < 0.0) throttle = 0.0;
  if (throttle > 1.0) throttle = 1.0;

  const real_T trq_total   = throttle * lookup_wot_torque(map_in.engine_rpm);
  const real_T trq_corner  = 0.25 * trq_total;

  // Even split across the four wheels, order FL FR RL RR.
  map_out.trq_fl = trq_corner;
  map_out.trq_fr = trq_corner;
  map_out.trq_rl = trq_corner;
  map_out.trq_rr = trq_corner;
}

} // extern "C"
