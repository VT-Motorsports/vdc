//
// File: tv_engine.h
//
// ChassisSim Engine component interface for the tv_io torque vectoring
// controller. Bound by symbol name from tv_engine.txt, so everything is
// exported with C linkage.
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

// Field order must match the InputVar lines in tv_engine.txt.
typedef struct {
  real_T throttle;       // idx 0   [0..1]
  real_T wheel_speed_fl; // idx 2
  real_T wheel_speed_fr; // idx 3
  real_T wheel_speed_rl; // idx 4
  real_T wheel_speed_rr; // idx 5
  real_T ax;             // idx 22
  real_T ay_g;           // idx 25  [g]
  real_T brake;          // idx 23  [0..1]
} tv_engine_in_t;

typedef struct {
  real_T trq_fl; // idx 3  [Nm]
  real_T trq_fr; // idx 4  [Nm]
  real_T trq_rl; // idx 5  [Nm]
  real_T trq_rr; // idx 6  [Nm]
} tv_engine_out_t;

extern TV_ENGINE_API tv_engine_in_t  tv_in;
extern TV_ENGINE_API tv_engine_out_t tv_out;

TV_ENGINE_API void tv_initialize(void);
TV_ENGINE_API void tv_step(void);

#ifdef __cplusplus
}
#endif

#endif // TV_ENGINE_H
