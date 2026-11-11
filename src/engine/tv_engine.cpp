//
// File: tv_engine.cpp
//
// ChassisSim Engine component wrapper around the tv_io torque vectoring
// controller. Converts the Engine signals into tv_io units, runs
// calculate_torques on a persistent instance and publishes the wheel torques.
//

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <windows.h>

// tv_io.hpp uses ImVec4 only for GUI colour fields.
struct ImVec4 {
  float x, y, z, w;
  ImVec4(float a = 0, float b = 0, float c = 0, float d = 0) : x(a), y(b), z(c), w(d) {}
};

#include "../tv/tv_io.hpp"
#include "tv_engine.h"

static const double kWheelSpeedToRadps = 1.0; // wheel speed input -> rad/s
static const double kAxToMps2          = 1.0; // ax input -> m/s^2
static const double kGravity           = 9.81; // ay_g input -> m/s^2

static const int kLogFlushRows = 100;

static double g_k_mz_ay   = 0.0;
static bool   g_debug_log = false;
static int    g_log_every = 1000;
static double g_ay_sign   = 1.0;
static double g_ax_scale  = 1.0;
static bool   g_have_lltd = false; // lltd_f stays at the tv_io default unless set
static double g_lltd_f    = 0.0;

static HMODULE   g_module     = NULL;
static bool      g_cfg_loaded = false;
static FILE     *g_log        = NULL;
static bool      g_log_tried  = false;
static long long g_calls      = 0;
static int       g_log_rows   = 0;

// Full path of a file in the DLL's folder.
static bool dll_dir_path(const char *name, char *path)
{
  DWORD n = GetModuleFileNameA(g_module, path, MAX_PATH);
  if (n == 0 || n >= MAX_PATH) return false;
  char *slash = strrchr(path, '\\');
  if (!slash) return false;
  size_t len = strlen(name) + 1;
  if ((size_t)(slash + 1 - path) + len > MAX_PATH) return false;
  memcpy(slash + 1, name, len);
  return true;
}

static char *trim(char *s)
{
  while (*s == ' ' || *s == '\t') ++s;
  char *e = s + strlen(s);
  while (e > s && (e[-1] == ' ' || e[-1] == '\t' || e[-1] == '\r' || e[-1] == '\n')) --e;
  *e = '\0';
  return s;
}

static void config_load(void)
{
  g_cfg_loaded = true;
  char path[MAX_PATH];
  if (!dll_dir_path("tv_config.txt", path)) return;
  FILE *f = fopen(path, "r");
  if (!f) return;
  char line[256];
  while (fgets(line, sizeof(line), f)) {
    char *c = strpbrk(line, ";#");
    if (c) *c = '\0';
    char *eq = strchr(line, '=');
    if (!eq) continue;
    *eq = '\0';
    const char *key = trim(line);
    char *val = trim(eq + 1);
    char *end;
    double x = strtod(val, &end);
    if (end == val) continue;
    if (strcmp(key, "k_mz_ay") == 0) g_k_mz_ay = x;
    else if (strcmp(key, "debug_log") == 0) g_debug_log = (x != 0.0);
    else if (strcmp(key, "log_every") == 0 && x >= 1.0) g_log_every = (int)x;
    else if (strcmp(key, "ay_sign") == 0) g_ay_sign = x;
    else if (strcmp(key, "ax_scale") == 0) g_ax_scale = x;
    else if (strcmp(key, "lltd_f") == 0 && x >= 0.0 && x <= 1.0) { g_lltd_f = x; g_have_lltd = true; }
  }
  fclose(f);
}

static void log_open(void)
{
  g_log_tried = true;
  char path[MAX_PATH];
  if (!dll_dir_path("tv_debug.csv", path)) return;
  g_log = fopen(path, "w");
  if (!g_log) return;
  fprintf(g_log, "call,throttle,ws_fl,ws_fr,ws_rl,ws_rr,ax,ay_g,brake,"
                 "v,yaw_rate_target,yaw_rate,mz_des,trq_fl,trq_fr,trq_rl,trq_rr\n");
}

extern "C" {

TV_ENGINE_API tv_engine_in_t  tv_in  = {};
TV_ENGINE_API tv_engine_out_t tv_out = {};

static tv_io tv;

BOOL WINAPI DllMain(HINSTANCE inst, DWORD reason, LPVOID)
{
  if (reason == DLL_PROCESS_ATTACH) {
    g_module = inst;
  } else if (reason == DLL_PROCESS_DETACH && g_log) {
    fclose(g_log);
    g_log = NULL;
  }
  return TRUE;
}

static void init_once(void)
{
  config_load();
  tv.k_mz_ay = (float)g_k_mz_ay;
  if (g_have_lltd) tv.lltd_f = (float)g_lltd_f;
}

TV_ENGINE_API void tv_initialize(void)
{
  if (!g_cfg_loaded) init_once();
}

TV_ENGINE_API void tv_step(void)
{
  if (!g_cfg_loaded) init_once();

  tv.throttle = (float)(tv_in.throttle * 100.0);
  tv.brake    = (float)(tv_in.brake    * 100.0);

  const float w_fl = (float)(tv_in.wheel_speed_fl * kWheelSpeedToRadps);
  const float w_fr = (float)(tv_in.wheel_speed_fr * kWheelSpeedToRadps);
  const float w_rl = (float)(tv_in.wheel_speed_rl * kWheelSpeedToRadps);
  const float w_rr = (float)(tv_in.wheel_speed_rr * kWheelSpeedToRadps);

  tv.velocity = 0.25f * (w_fl + w_fr + w_rl + w_rr) * tv.rad_wheel;

  // Yaw rate from the left-right wheel speed difference, deg/s for tv_io.
  const float v_left  = 0.5f * (w_fl + w_rl) * tv.rad_wheel;
  const float v_right = 0.5f * (w_fr + w_rr) * tv.rad_wheel;
  const float yaw_rate_radps = (tv.track > 1.0e-4f) ? (v_right - v_left) / tv.track : 0.0f;
  tv.yaw_rate = yaw_rate_radps * (180.0f / (float)M_PI);

  const float rpm_per_radps = tv.gear * 60.0f / (2.0f * (float)M_PI);
  tv.rpm[0] = w_fl * rpm_per_radps;
  tv.rpm[1] = w_fr * rpm_per_radps;
  tv.rpm[2] = w_rl * rpm_per_radps;
  tv.rpm[3] = w_rr * rpm_per_radps;

  tv.use_ext_ax = true;
  tv.ext_ax = (float)(tv_in.ax * g_ax_scale * kAxToMps2);
  tv.use_ext_ay = true;
  tv.ext_ay = (float)(tv_in.ay_g * g_ay_sign * kGravity);

  tv.steer = 0.0f;
  tv.yaw_target_mode = YAW_TGT_LAT_ACCEL;

  tv.calculate_torques();

  // No regen; ChassisSim applies the friction brakes itself.
  tv_out.trq_fl = (tv.trq[0] > 0.0f) ? tv.trq[0] : 0.0;
  tv_out.trq_fr = (tv.trq[1] > 0.0f) ? tv.trq[1] : 0.0;
  tv_out.trq_rl = (tv.trq[2] > 0.0f) ? tv.trq[2] : 0.0;
  tv_out.trq_rr = (tv.trq[3] > 0.0f) ? tv.trq[3] : 0.0;

  ++g_calls;
  if (g_debug_log && g_calls % g_log_every == 0) {
    if (!g_log_tried) log_open();
    if (g_log) {
      fprintf(g_log, "%lld,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g,"
                     "%.6g,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g,%.6g\n",
              g_calls, tv_in.throttle, tv_in.wheel_speed_fl, tv_in.wheel_speed_fr,
              tv_in.wheel_speed_rl, tv_in.wheel_speed_rr, tv_in.ax, tv_in.ay_g, tv_in.brake,
              (double)tv.velocity, (double)tv.yaw_rate_target, (double)tv.yaw_rate, (double)tv.mz_des,
              tv_out.trq_fl, tv_out.trq_fr, tv_out.trq_rl, tv_out.trq_rr);
      if (++g_log_rows % kLogFlushRows == 0) fflush(g_log);
    }
  }
}

} // extern "C"
