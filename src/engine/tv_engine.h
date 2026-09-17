//
// File: tv_engine.h
//
// ChassisSim Engine component interface for the standalone tv_io torque
// vectoring controller. Structured like the reference example's header
// (Double_Trial.h): flat input/output structs of real_T, plus an initialize
// and a step function. Bound by symbol name from tv_engine.cmp, so everything
// is exported with C linkage (stable Win64 C ABI across MinGW and the MSVC host).
//
#ifndef TV_ENGINE_H
#define TV_ENGINE_H

#include "rtwtypes.h" // real_T == double

#ifdef _WIN32
#define TV_ENGINE_API __declspec(dllexport)
#else
#define TV_ENGINE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// External inputs (ChassisSim Engine signals we consume).
// Field order MUST match the order of the InputVar lines in tv_engine.cmp.
// The trailing comment on each field is the ChassisSim Engine signal index it
// is wired to in that component file.
typedef struct {
  real_T throttle;       // idx 0   [0..1]
  real_T wheel_speed_fl; // idx 2   [rad/s]
  real_T wheel_speed_fr; // idx 3   [rad/s]
  real_T wheel_speed_rl; // idx 4   [rad/s]
  real_T wheel_speed_rr; // idx 5   [rad/s]
  real_T wheel_load_fl;  // idx 6   [N]
  real_T wheel_load_fr;  // idx 7   [N]
  real_T wheel_load_rl;  // idx 8   [N]
  real_T wheel_load_rr;  // idx 9   [N]
  real_T trq_dem_fl;     // idx 10  [Nm]  (interface signal; controller derives its own demand)
  real_T trq_dem_fr;     // idx 11  [Nm]
  real_T trq_dem_rl;     // idx 12  [Nm]
  real_T trq_dem_rr;     // idx 13  [Nm]
  real_T trq_max_fl;     // idx 14  [Nm]  (interface signal; controller derives its own caps)
  real_T trq_max_fr;     // idx 15  [Nm]
  real_T trq_max_rl;     // idx 16  [Nm]
  real_T trq_max_rr;     // idx 17  [Nm]
  real_T trq_min_fl;     // idx 18  [Nm]
  real_T trq_min_fr;     // idx 19  [Nm]
  real_T trq_min_rl;     // idx 20  [Nm]
  real_T trq_min_rr;     // idx 21  [Nm]
  real_T ax;             // idx 22  [m/s^2]
  real_T brake;          // idx 23  [0..1]

  // --- reserved: ChassisSim Engine indices to be confirmed ---
  // No InputVar line is emitted for these yet; they sit after every mapped
  // field so the positional InputVar ordering above is unaffected.
  real_T steer;     // UNRESOLVED INPUT INDEX -- steer angle at the tyre [rad]
  real_T yaw_rate;  // UNRESOLVED INPUT INDEX -- yaw rate [rad/s] (derived from wheel speeds until wired)
  real_T lat_accel; // UNRESOLVED INPUT INDEX -- lateral acceleration [m/s^2]
} tv_engine_in_t;

// External outputs (wheel torques published back to the Engine).
typedef struct {
  real_T trq_fl; // idx 3  [Nm]
  real_T trq_fr; // idx 4  [Nm]
  real_T trq_rl; // idx 5  [Nm]
  real_T trq_rr; // idx 6  [Nm]
} tv_engine_out_t;

// Signal-exchange instances bound by name in tv_engine.cmp.
extern TV_ENGINE_API tv_engine_in_t  tv_in;
extern TV_ENGINE_API tv_engine_out_t tv_out;

// Initialize function (InitFnFlg=0 -> not invoked, body empty).
TV_ENGINE_API void tv_initialize(void);

// Step function: reads tv_in, runs the controller, writes tv_out.
TV_ENGINE_API void tv_step(void);

#ifdef __cplusplus
}
#endif

#endif // TV_ENGINE_H
