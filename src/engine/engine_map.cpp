//
// File: engine_map.cpp
//
// Minimal ChassisSim Engine component for debugging the DLL pipeline: throttle
// x the power-limited motor torque at the given rpm, split evenly over the four
// wheels. No torque vectoring, no load transfer, no yaw logic, and no
// dependency on tv_io or tv_engine.
//
// ChassisSim binds the component by symbol name
// (InputVarName/OutputVarName/FunctionName), which requires flat exported
// symbols, so the structs and functions are global with C linkage.
//

#define _USE_MATH_DEFINES
#include <cmath>

#include "engine_map.h"

static const real_T kGear      = 12.875;
static const real_T kNumMotors = 4.0;
static const real_T kRevLimit  = 20000.0; // [rpm]

// true: 15 kW per motor / 60 kW total. false: 25 kW / 80 kW.
static const bool kEndurance = true;
static const real_T kPowerMotor = kEndurance ? 15000.0 : 25000.0; // [W]
static const real_T kPowerTotal = kEndurance ? 60000.0 : 80000.0; // [W]

// Per-motor torque at the motor shaft against motor rpm (engine_rpm input =
// motor rpm). Linearly interpolated, clamped below 0 rpm, zero above kRevLimit.
static const real_T kRpmBp[]    = { 0.0, 11000.0, 12000.0, 14000.0, 16000.0, 18000.0, 20000.0 };
static const real_T kTrqMotor[] = { 31.6,   31.6,    31.0,    29.0,    25.0,    21.0,    18.0 }; // [Nm]
static const int kNumBp = (int)(sizeof(kRpmBp) / sizeof(kRpmBp[0]));

static real_T lookup_motor_torque(real_T rpm)
{
  if (rpm > kRevLimit) {
    return 0.0;
  }
  if (rpm <= kRpmBp[0]) {
    return kTrqMotor[0];
  }

  for (int i = 0; i < kNumBp - 1; ++i) {
    if (rpm <= kRpmBp[i + 1]) {
      const real_T span = kRpmBp[i + 1] - kRpmBp[i];
      const real_T frac = (span > 0.0) ? (rpm - kRpmBp[i]) / span : 0.0;
      return kTrqMotor[i] + frac * (kTrqMotor[i + 1] - kTrqMotor[i]);
    }
  }

  return kTrqMotor[kNumBp - 1];
}

extern "C" {

// Exported signal-exchange instances (flat doubles), bound by name in engine_map.cmp.
ENGINE_MAP_API engine_map_in_t  map_in  = {};
ENGINE_MAP_API engine_map_out_t map_out = {};

// Initialize function. InitFnFlg=0 in the component file means ChassisSim does
// not call this; the table is static and needs no setup.
ENGINE_MAP_API void map_initialize(void)
{
}

// Step function: read map_in, evaluate the map, write map_out.
ENGINE_MAP_API void map_step(void)
{
  real_T throttle = map_in.throttle;
  if (throttle < 0.0) throttle = 0.0;
  if (throttle > 1.0) throttle = 1.0;

  const real_T rpm   = map_in.engine_rpm;
  const real_T omega = std::fabs(rpm) * (2.0 * M_PI / 60.0); // [rad/s] motor

  real_T trq_motor = lookup_motor_torque(rpm);
  if (omega > 1.0e-6 && trq_motor > kPowerMotor / omega) {
    trq_motor = kPowerMotor / omega;
  }

  real_T trq_total = kNumMotors * trq_motor;
  if (omega > 1.0e-6 && trq_total > kPowerTotal / omega) {
    trq_total = kPowerTotal / omega;
  }

  // Even split across the four wheels, order FL FR RL RR, at the wheel.
  const real_T trq_corner = throttle * trq_total * kGear / kNumMotors;

  map_out.trq_fl = trq_corner;
  map_out.trq_fr = trq_corner;
  map_out.trq_rl = trq_corner;
  map_out.trq_rr = trq_corner;
}

} // extern "C"
