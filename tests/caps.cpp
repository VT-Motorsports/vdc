// Compiles tv_engine.cpp in directly to read tv_io internals (trq_cap, fz, mz_des, trq)
// for the run2 fixed case. Build with -I<src/engine>. Reads tv_config.txt from the
// folder this exe sits in. Build with -DCAPS_HEX for %a hex output (bit-exact compare).
#include "tv_engine.cpp"
#ifdef CAPS_HEX
#define CAPS_FMT "cap %a %a %a %a  fz %a %a %a %a  mz %a  trq %a %a %a %a\n"
#else
#define CAPS_FMT "cap %.4f %.4f %.4f %.4f  fz %.1f %.1f %.1f %.1f  mz %.2f  trq %.4f %.4f %.4f %.4f\n"
#endif
int main()
{
  double v = 15.0, r = 0.2032, om = 1.4 * 9.81 / v, ht = 1.194 * 0.5;
  double wl = (v - om * ht) / r, wr = (v + om * ht) / r;
  tv_in = { 0.5, wl, wr, wl, wr, 0.0, 1.4, 0.0 };
  for (int i = 0; i < 5; ++i) tv_step();
  printf(CAPS_FMT,
         tv.trq_cap[0], tv.trq_cap[1], tv.trq_cap[2], tv.trq_cap[3], tv.fz[0], tv.fz[1], tv.fz[2], tv.fz[3],
         tv.mz_des, tv.trq[0], tv.trq[1], tv.trq[2], tv.trq[3]);
  return 0;
}
