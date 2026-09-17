//
// File: engine_map.h
//
// Minimal ChassisSim Engine component used to debug the DLL pipeline itself:
// throttle and engine rpm in, four wheel torques out, nothing else. It shares
// the export pattern of tv_engine (flat input/output structs of real_T plus a
// step and an initialize function, all extern "C" and dllexported so the
// component is bound by unmangled symbol name from engine_map.cmp), but is a
// completely separate component -- it does not include or affect tv_engine or
// tv_io in any way.
//
#ifndef ENGINE_MAP_H
#define ENGINE_MAP_H

#include "rtwtypes.h" // real_T == double

#ifdef _WIN32
#define ENGINE_MAP_API __declspec(dllexport)
#else
#define ENGINE_MAP_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// External inputs. Field order MUST match the order of the InputVarN lines in
// engine_map.cmp; the trailing comment is the Engine signal index each field is
// wired to in that component file.
typedef struct {
  real_T throttle;   // idx 0  [0..1]
  real_T engine_rpm; // idx 1  [rev/min]
} engine_map_in_t;

// External outputs (wheel torques published back to the Engine).
typedef struct {
  real_T trq_fl; // idx 3  [Nm]
  real_T trq_fr; // idx 4  [Nm]
  real_T trq_rl; // idx 5  [Nm]
  real_T trq_rr; // idx 6  [Nm]
} engine_map_out_t;

// Signal-exchange instances bound by name in engine_map.cmp.
extern ENGINE_MAP_API engine_map_in_t  map_in;
extern ENGINE_MAP_API engine_map_out_t map_out;

// Initialize function (InitFnFlg=0 -> not invoked, body empty).
ENGINE_MAP_API void map_initialize(void);

// Step function: reads map_in, evaluates the torque map, writes map_out.
ENGINE_MAP_API void map_step(void);

#ifdef __cplusplus
}
#endif

#endif // ENGINE_MAP_H
