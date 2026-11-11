// Drives tv_engine.dll through its exported tv_in / tv_step / tv_out.
//   step <dll>                  fixed case: 15 m/s, 1.4 g, 50% throttle, 5 steps
//   step <dll> slip <thr> <k>   fixed case with throttle <thr> and FR wheel speed * <k>
//   step <dll> rand             2000 seeded random inputs, one line each, %a hex
// Build with -DSTEP_HEX to print the fixed cases in %a hex too (bit-exact compare).
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef STEP_HEX
#define FIXED_FMT "%a %a %a %a\n"
#else
#define FIXED_FMT "%.4f %.4f %.4f %.4f\n"
#endif
struct In { double thr, fl, fr, rl, rr, ax, ay_g, brk; };
struct Out { double fl, fr, rl, rr; };
int main(int argc, char **argv)
{
  HMODULE h = LoadLibraryA(argv[1]);
  if (!h) { printf("load failed %lu\n", GetLastError()); return 1; }
  In *in = (In *)GetProcAddress(h, "tv_in");
  Out *out = (Out *)GetProcAddress(h, "tv_out");
  void (*step)(void) = (void (*)(void))GetProcAddress(h, "tv_step");
  if (argc > 2 && strcmp(argv[2], "rand") == 0) {
    srand(1);
    for (int n = 0; n < 2000; ++n) {
      double v = 1.0 + 30.0 * rand() / RAND_MAX, r = 0.2032;
      double ay = (rand() / (double)RAND_MAX - 0.5) * 3.0;
      double om = ay * 9.81 / v, ht = 0.597;
      double sl = (rand() / (double)RAND_MAX - 0.5) * 0.2 * (n % 3 != 0);
      *in = { rand() / (double)RAND_MAX, (v - om * ht) / r * (1 + sl), (v + om * ht) / r * (1 - sl),
              (v - om * ht) / r, (v + om * ht) / r * (1 + sl * 0.5),
              (rand() / (double)RAND_MAX - 0.5) * 10.0, ay, (n % 7 == 0) ? 0.3 : 0.0 };
      step();
      printf("%a %a %a %a\n", out->fl, out->fr, out->rl, out->rr);
    }
    return 0;
  }
  double v = 15.0, r = 0.2032, om = 1.4 * 9.81 / v, ht = 1.194 * 0.5;
  double wl = (v - om * ht) / r, wr = (v + om * ht) / r;
  *in = { 0.5, wl, wr, wl, wr, 0.0, 1.4, 0.0 };
  if (argc > 3 && strcmp(argv[2], "slip") == 0) {
    in->thr = atof(argv[3]);
    in->fr = wr * atof(argv[4]);
  }
  for (int i = 0; i < 5; ++i) step();
  printf(FIXED_FMT, out->fl, out->fr, out->rl, out->rr);
  return 0;
}
