#ifndef VEHICLE_HPP
#define VEHICLE_HPP

class vehicle {
public:
	// Chassis 
	double m; // mass [kg]
	double i_zz; // inertia [kg.m²]
	double fw; // front weight [%] ***
	double h_s; // static center of gravity height [mm] ***
  
  // Aerodynamics
	double cxa; // drag coefficient * unit area [m²]
	double cza; // drag coefficient * unit area [m²]
  double fa; // front aero [%]
             
  // Powertrain
	double ft; // front torque [-]

  // Tires
	double dia; // full tire diameter [m]
	vec p94_s; // pacejka94 scaling coefficients [%]
	vec p94_a; // pacejka94 lateral coefficients [~]
	vec p94_b; // pacejka94 longitudinal coefficients [~]
	vec p94_c; // pacejka94 alignment coefficients [~]
	double kp_f, kp_r; // tire stiffness [N/mm] **

  // Suspension (Design)
	double l; // wheelbase [mm] ***
  double t_f, t_r; // track width [mm] ***
	double asq; // anti-squat [%] ***
	double adi; // anti-dive [%] ***
	double ack; // ackermann rate [°/°²]
	double rat; // steering ratio [-]
	double rc_f, rc_r; // roll centers [mm] ***
	double cag_f, cag_r; // camber gain [°/mm] ***
	double tog_f, tog_r; // toe gain [°/mm] ***
	double mrs_f, mrs_r; // motion ratio [-]

  // Suspension (Tuning)
	double cam_f, cam_r; // static camber [°]
	double toe_f, toe_r; // static toe [°]
	double rh_f, rh_r; // static ride heights [mm] ***
	double fb; // front braking [-]
	double ks_f, ks_r; // spring stiffness [N/mm] ***
	double ka_f, ka_r; // arb stiffness [N/mm] ***
  
  // Calculated Parameters
	double a; // front axle offset from cg [mm] ***
	double b; // rear axle offset from cg [mm] ***
	double kw_f, kw_r; // combined wheel rates (heave/pitch mode) [N/mm] ***
	double kr, kr_f, kr_r; // roll rates [N.mm/°] ***
	double krs_f, krs_r; // sprung roll rates [N.mm/°] ***
	double krp_f, krp_r; // pneumatic roll rates [N.mm/°] ***

  // Constructor
	vehicle(){
	}

  // Recalculate parameters before running ymd
  void recalculate() {
    
    // Footprint
    a = (1 - fw/100) * l;
    b = l - a;

    // Wheel rates [N/mm]
    kw_f = (pow(mrs_f, -2) * ks_f * kp_f) / (pow(mrs_f, -2) * ks_f + kp_f);
    kw_r = (pow(mrs_r, -2) * ks_r * kp_r) / (pow(mrs_r, -2) * ks_r + kp_r);
    
    // Individual roll rates [N.mm/°]
    krs_f = (pow(mrs_f, -2) * (ks_f + ka_f)) * pow(t_f, 2) / 2 / 57.3;
    krs_r = (pow(mrs_r, -2) * (ks_r + ka_r)) * pow(t_r, 2) / 2 / 57.3;
    krp_f = kp_f * pow(t_f, 2) / 2 / 57.3;
    krp_r = kp_r * pow(t_r, 2) / 2 / 57.3;

    // Combined roll rates [N.mm/°] 
    kr_f = (krs_f * krp_f) / (krs_f + krp_f);
    kr_r = (krs_r * krp_r) / (krs_r + krp_r);
    kr = kr_f + kr_r;
    
  }

  // Get aerodynamic drag force on the chassis in body coordinates
  double get_drag(const double &v) const {
    return -0.5 * 1.225 * pow(v, 2) * cxa;
  }

// Get static corner loads from vehicle mass and weight distribution 
vec get_fz_static() const {
	vec z_s = {fw/100 * m * -4.9, fw/100 * m * -4.9, (1 - fw/100) * m * -4.9, (1 - fw/100) * m * -4.9};
	return z_s;
}

// Get aerodynamic cornering loads from CzA and aero distribution
vec get_fz_aero(const double &v) const {
	const double c = -0.5 * 1.225 * pow(v, 2);
  const double cza_f = cza * fa/100;
  const double cza_r = cza * (1 - fa/100);
	vec z_a = {cza_f * c / 2, cza_f * c / 2, cza_r * c / 2, cza_r * c / 2};
	return z_a;
}

// Get lateral load transfers from body-coordinate lateral forces
vec get_dz_lateral(const vec &y, const double &h) const {
	const double y1 = y(0) + y(1);
	const double y2 = y(2) + y(3);
	const double qb = rc_f * (1 - fw/100) + rc_r * fw/100;
	const double mx1 = (kr_f / kr) * (h - qb) * (y1 + y2) + (y1 * rc_f) + (kr_f * kr_r / kr) * ((y2 * rc_r / krp_r) - (y1 * rc_f / krp_f));
	const double mx2 = (kr_r / kr) * (h - qb) * (y1 + y2) + (y2 * rc_r) + (kr_f * kr_r / kr) * ((y1 * rc_f / krp_f) - (y2 * rc_r / krp_r));
	vec dz_l = {mx1 / t_f, -mx1 / t_f, mx2 / t_r, -mx2 / t_r};
	return dz_l;
}

// Get longitudinal load transfers from body-coordinate longitudinal forces
vec get_dz_longitudinal(const vec &x, const double &h) const {
	const double dz = sum(x) * h / l / 2;
	vec dz_n = {dz, dz, -dz, -dz};
	return dz_n;
}

// Get corner torques from a combination of factors (torque vectoring-capable)
vec get_torque_drive(const double &t_req, const double &sip, const double &str_in, const double &v) const {
	const double dx_sip_f = 0 * sip;
	const double dx_str_f = 0 * str_in;
	const double dx_v_f = 0 * v;
	const double dx_sip_r = 0 * sip;
	const double dx_str_r = 0 * str_in;
	const double dx_v_r = 0 * v;
	const double dx_f = dx_sip_f + dx_str_f + dx_v_f;
	const double dx_r = dx_sip_r + dx_str_r + dx_v_r;
	const double x_req = t_req / (0.49 * dia);
	const vec x = {ft/100 * x_req / 2 - dx_f, ft/100 * x_req / 2 + dx_f, (1 - ft/100) * x_req / 2 - dx_r, (1 - ft/100) * x_req / 2 + dx_r};
	vec trq = x * (0.49 * dia);
	return trq;
}

// Get braking torques from required total torque, brake bias (evenly distributed left/right)
vec get_torque_brake(const double &t_req) const {
	const double t_req_f = fb/100 * t_req;
	const double t_req_r = (1 - fb/100) * t_req;
	vec trq = {t_req_f / 2, t_req_f / 2, t_req_r / 2, t_req_r / 2};
	return trq;
}

// Get slip angles from dynamic states (& add tractrix effect for low-speed maneuvers)
vec get_slip_angles(const double &sip, const vec &str, const double &r) const {
	vec wsp(4); // Wheel sideslip
	const mat c2w = {{a/1000, 0.5 * t_f/1000}, {a/1000, -0.5 * t_f/1000}, {-b/1000, 0.5 * t_r/1000}, {-b/1000, -0.5 * t_r/1000}}; // CG to W vectors
	const vec r2c = {r * -sin(sip / 57.3), r * cos(sip / 57.3)}; // Turn center to CG
	for (int i = 0; i < 4; ++i) { // Generate wheel sideslip values
		const vec r2w = r2c - c2w.row(i).t();
		wsp(i) = 57.3 * (atan2(r2w(1), r2w(0)) - atan2(1, 0));
		if (wsp(i) < -90) {
			wsp(i) += 180;
		}
	}
	const vec trx = 57.3 * vec({0, 0, b/1000 / r, b/1000 / r}); // Tractrix effect for low-speed maneuvers
	vec slp = wsp + trx - str;
	return slp;
}

// Get tire (possible) longitudinal forces (maximum, not based on slip ratio)
vec get_fx_tire(const vec &trq, const vec &z, const vec &inc) const {
	const vec fzn = z;
	const vec fzkn = fzn / 1000;
	const vec D = p94_s(0)/100 * fzn % (p94_b(1) * fzkn + p94_b(2)) % (1 - p94_b(14) * pow(inc, 2));
	vec x_t(4);
	for (int i = 0; i < 4; ++i) {
		if (abs(trq(i) / (0.98 * dia / 2)) > abs(D(i))) {
			// If torque applied to wheel exceeds longitudinal grip limits
			x_t(i) = 0.7 * D(i) * -sign(trq(i));
		}
		else {
			// If torque applied to wheel does not exceed longitudinal grip limits
			x_t(i) = trq(i) / (dia / 2);
		}
	}
  for (int i = 0; i < 4; ++i){
    if (z(i) >= 0){
      x_t(i) = 0;
    }
  }
	return x_t;
}

// Get tire (possible) lateral forces (based on slip angle and longitudinal forces)
vec get_fy_tire(const vec &trq, const vec &x_p, const vec &slp, const vec &z, const vec &inc) const {
	const vec fzn = z;
	const vec fzkn = fzn / 1000;
	const double C = p94_a(0);
	const vec D = p94_s(1)/100 * fzn % (p94_a(1) * fzkn + p94_a(2)) % (1 - p94_a(15) * pow(inc, 2));
	const vec BCD = p94_a(3) * sin(atan(fzkn / p94_a(4))) % (1 - p94_a(5) * abs(inc));
	const vec B = BCD / (C * D);
	const vec H = (p94_a(8) * fzkn + p94_a(9) + p94_a(10) * inc);
	const vec V = (p94_a(11) * fzn + p94_a(12) + (p94_a(13) * fzkn + p94_a(14)) % fzkn % inc);
	const vec E = (p94_a(6) * fzkn + p94_a(7)) % (1 - (p94_a(16) * inc + p94_a(17)));
	const vec Bx1 = B % (slp + H);
	const vec y_e = D % sin(C * atan(Bx1 - E % (Bx1 - atan(Bx1)))) + V;
	const vec y_0 = D % sin(C * atan(B%H - E % (B%H - atan(B%H)))) + V;
	vec y_t = y_e % sqrt(1 - pow((trq / (0.98 * dia / 2)) / x_p, 2)) + y_0;
  for (int i = 0; i < 4; ++i){
    if (z(i) >= 0){
      y_t(i) = 0;
    }
  }
	return y_t;
}

// WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP  
vec get_mz_tire(const vec &trq, const vec &x_p, const vec &slp, const vec &z, const vec &inc) const {
	const vec fzn = z;
	const vec fzkn = fzn / 1000;
	const double C = p94_a(0);
	const vec D = p94_s(2)/100 * fzn % (p94_c(1) * fzkn + p94_c(2)) % (1 - p94_c(15) * pow(inc, 2));
	const vec BCD = p94_c(3) * sin(atan(fzkn / p94_c(4))) % (1 - p94_c(5) * abs(inc));
	const vec B = BCD / (C * D);
	const vec H = (p94_c(8) * fzkn + p94_c(9) + p94_c(10) * inc);
	const vec V = (p94_c(11) * fzn + p94_c(12) + (p94_c(13) * fzkn + p94_c(14)) % fzkn % inc);
	const vec E = (p94_c(6) * fzkn + p94_c(7)) % (1 - (p94_c(16) * inc + p94_c(17)));
	const vec Bx1 = B % (slp + H);
	const vec mz_e = D % sin(C * atan(Bx1 - E % (Bx1 - atan(Bx1)))) + V;
	const vec mz_0 = D % sin(C * atan(B%H - E % (B%H - atan(B%H)))) + V;
	vec mz_t = mz_e % sqrt(1 - pow((trq / (0.98 * dia / 2)) / x_p, 2)) + mz_0;
  for (int i = 0; i < 4; ++i){
    if (z(i) >= 0){
      mz_t(i) = 0;
    }
  }
	return mz_t;
}

// Get tire (maximum) longitudinal forces 
vec get_fx_potential(const vec &z, const vec &inc) const {
	const vec fzn = z;
	const vec fzkn = fzn / 1000;
	const vec D = p94_s(0)/100 * fzn % (p94_b(1) * fzkn + p94_b(2)) % (1 - p94_b(14) * pow(inc, 2));
	vec x_p = D;
	return x_p;
}

// Get tire (maximum) lateral forces (based on slip angle)
vec get_fy_potential(const vec &slp, const vec &z, const vec &inc) const {
	const vec fzn = z;
	const vec fzkn = fzn / 1000;
	const vec D = p94_s(1)/100 * fzn % (p94_a(1) * fzkn + p94_a(2)) % (1 - p94_a(15) * pow(inc, 2));
	const vec V = (p94_a(11) * fzn + p94_a(12) + (p94_a(13) * fzkn + p94_a(14)) % fzkn % inc);
	vec y_p = D + V % -sign(slp);
	return y_p;
}

// WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP  
vec get_mz_potential(const vec &z, const vec &inc) const {
	vec mz_p = {0, 0, 0, 0};
	return mz_p;
}

// Get static wheel bump COMPONENTS from ride height offsets
vec get_bump_static() const {
	vec bmp_s = {-rh_f, -rh_f, -rh_r, -rh_r};
	return bmp_s;
}

// Get dynamic wheel bump COMPONENTS from dynamic loads
vec get_bump_dynamic(const vec &z_d) const {
	const vec kw = {kw_f, kw_f, kw_r, kw_r};
	vec bmp_d = z_d / -kw;
	return bmp_d;
}

// Get jacking wheel bump COMPONENTS from longitudinal & lateral jacking effects
vec get_bump_jacking(const vec &x, const vec &y, const double &h) const {
	const double bmp_adi = (x(0) + x(1)) * (-adi/100 * h / a) / (2 * kw_f);
	const double bmp_asq = (x(2) + x(3)) * (+asq/100 * h / b) / (2 * kw_r);
	const double bmp_arf = (y(0) - y(1)) * rc_f / (t_f / 2) / (2 * kw_f);
	const double bmp_arr = (y(2) - y(3)) * rc_r / (t_r / 2) / (2 * kw_r);
	vec bmp_j = {bmp_adi + bmp_arf, bmp_adi + bmp_arf, bmp_asq + bmp_arr, bmp_asq + bmp_arr};
	return bmp_j;
}

// Get static inclination (absolute) COMPONENT from set camber (relative) values
vec get_inclination_static() const {
	vec inc_s = {-cam_f, +cam_f, -cam_r, +cam_r};
	return inc_s;
}

// WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP WIP  
vec get_inclination_steered(const vec &str) const {
	vec inc_e = {0, 0, 0, 0};
	return inc_e;
}

// Get dynamic inclination COMPONENT from bump curves (linear simplification)
vec get_inclination_dynamic(const bmp_whl &bmp) const {
	const vec cag = {-cag_f, +cag_f, -cag_r, +cag_r};
	const double rll = get_rll(bmp);
	vec inc_d = bmp.d % cag + rll;
	return inc_d;
}

// Get static steer (absolute) COMPONENT from set toe (relative) values
vec get_steer_static() const {
	vec str_s = {-toe_f, +toe_f, -toe_r, +toe_r};
	return str_s;
}

// Get steered steer (redundant I know) COMPONENT from steer angle and ackermann
vec get_steer_steered(const double &str_in) const {
	const double rck = str_in / rat;
	const vec akr = pow(rck, 2) * ack / 2 * vec({1, -1, 0, 0});
	vec str_e = rck * vec({1, 1, 0, 0}) + akr;
	return str_e;
}

// Get dynamic steer COMPONENT from bump curves (linear simplification) 
vec get_steer_dynamic(const vec &bmp_d) const {
	const vec tog = {-tog_f, +tog_f, -tog_r, +tog_r};
	vec str_d = bmp_d % tog;
	return str_d;
}

// Get static center of gravity COMPONENT from static relative bump
double get_cg_static(const vec &bmp_s) const {
	const double dh_f = -(bmp_s(0) + bmp_s(1)) / 2;
	const double dh_r = -(bmp_s(2) + bmp_s(3)) / 2;
	double h_st = h_s + dh_f * (1 - fw/100) + dh_r * fw/100;
	return h_st;
}

// Get dynamic center of gravity COMPONENT from dynamic bump
double get_cg_dynamic(const vec &bmp_d) const {
	const double dh_f = -(bmp_d(0) + bmp_d(1)) / 2;
	const double dh_r = -(bmp_d(2) + bmp_d(3)) / 2;
	double h_d = dh_f * (1 - fw/100) + dh_r * fw/100;
	return h_d;
}

// Get jacked center of gravity COMPONENT from jacking bump
double get_cg_jacking(const vec &bmp_j) const {
	const double dh_f = -(bmp_j(0) + bmp_j(1)) / 2;
	const double dh_r = -(bmp_j(2) + bmp_j(3)) / 2;
	double h_j = dh_f * (1 - fw/100) + dh_r * fw/100;
	return h_j;
}

// Get TOTAL heave from TOTAL bump
double get_hve(const bmp_whl &bmp) const {
	const double hve_f = -(bmp()(0) + bmp()(1)) / 2;
	const double hve_r = -(bmp()(2) + bmp()(3)) / 2;
	return hve_f * (1 - fw/100) + hve_r * fw/100;
}

// Get TOTAL roll from TOTAL bump
double get_rll(const bmp_whl &bmp) const {
	const double rll_f = (bmp()(1) - bmp()(0)) / t_f * 57.3;
	const double rll_r = (bmp()(3) - bmp()(2)) / t_r * 57.3;
	return rll_f * (1 - fw/100) + rll_r * fw/100;
}

// Get TOTAL pitch from TOTAL bump
double get_pch(const bmp_whl &bmp) const {
	const double pch_l = (bmp()(0) - bmp()(2)) / l * 57.3;
	const double pch_r = (bmp()(1) - bmp()(3)) / l * 57.3;
	return (pch_l + pch_r) / 2;
}

// Get TOTAL ax in SIDESLIP frame
double get_ax_resultant(const fx_whl &x, const fy_whl &y, const str_whl &str, const double &yaw, const double &v) const {
	const double fx_b = sum(x.reframe(y.t, str() - yaw));
	return (fx_b + get_drag(v)) / m;
}

// Get torque required to hit AX target
double get_trq_required(const double &ax, const double &ax_res, const double &yaw) const {
	const double fx_b = (ax / cos(yaw / 57.3) - ax_res) * m;
	return fx_b * (0.49 * dia);
}

// Get TOTAL ay in SIDESLIP frame
double get_ay_resultant(const fx_whl &x, const fy_whl &y, const str_whl &str, const double &yaw) const {
	const double fy_b = sum(y.reframe(x.t, str() - yaw));
	return fy_b / m;
}

// Get TOTAL angular acceleration (aa)
double get_aa_resultant(const fx_whl &x, const fy_whl &y, const mz_whl &mz, const str_whl &str) const {
	const vec fx_w = x.reframe(y.t, str());
	const vec fy_w = y.reframe(x.t, str());
	const double mz_x = (fx_w(1) - fx_w(0)) * t_f/1000 + (fx_w(3) - fx_w(2)) * t_r/1000;
	const double mz_y = (fy_w(0) + fy_w(1)) * a/1000 - (fy_w(2) + fy_w(3)) * b/1000;
  const double mz_z = sum(mz.t);
	return (mz_x + mz_y + mz_z) / i_zz * 57.3;
}

// Get cornering radius in SIDESLIP frame 
double get_radius(const double &ay_res, const double &v) const {
	double r = pow(v, 2) / ay_res;
	if (abs(r) > 10000.0) { // Avoid failure to settle at origin
		r = 9999.99;
	}
	return r;
}

// Total computation for single point on YMD
void get_instance_const_v(ymd_v_io &io, const int &i, const int &j, const int &k) const {
	cg h(h_s);
	fx_whl x;
	fy_whl y;
	fz_whl z;
	mz_whl mz;
	inc_whl inc;
	str_whl str;
	bmp_whl bmp;
	vec trq;
	vec slp;
	double ax_res = 0;
	double ay_res = 0;
	double aa_res = 0;
	double ax_old, ay_old, aa_old, r_old;
	double trq_req = 0;
	double r_res = 100;

	// SETTLING THE PLOT
	static const int max_iterations = 40;
	const double P = 0.7;
	for (int i_ = 0; i_ < max_iterations; ++i_) {
		ax_old = ax_res;
		ay_old = ay_res;
		aa_old = aa_res;
		r_old = r_res;
		z.s = get_fz_static();
		z.a = get_fz_aero(io.v(i, j, k));
		z.l = get_dz_lateral(y.reframe(x.t, str()), h());
		z.n = get_dz_longitudinal(x.reframe(y.t, str()), h());
		bmp.s = get_bump_static();
		bmp.d = get_bump_dynamic(z() - z.s);
		bmp.j = get_bump_jacking(x.reframe(y.t, str()), y.reframe(x.t, str()), h());
		inc.s = get_inclination_static();
		inc.e = get_inclination_steered(str());
		inc.d = get_inclination_dynamic(bmp);
		str.s = get_steer_static();
		str.e = get_steer_steered(io.steer(i, j, k));
		str.d = get_steer_dynamic(bmp.d);
		h.s = get_cg_static(bmp.s);
		h.d = get_cg_dynamic(bmp.d);
		h.j = get_cg_jacking(bmp.j);
		if (trq_req > 0)
			trq = get_torque_drive(trq_req, io.yaw(i, j, k), io.steer(i, j, k), io.v(i, j, k));
		else
			trq = get_torque_brake(trq_req);
		slp = get_slip_angles(io.yaw(i, j, k), str(), r_res);
		if (pow(io.yaw(i, j, k), 2) < 0.001 && pow(io.steer(i, j, k), 2) < 0.25) { // Handles small values
			slp = vec({0, 0, 0, 0});
		}
		x.t = get_fx_tire(trq, z(), inc());
		x.p = get_fx_potential(z(), inc());
		y.t = get_fy_tire(trq, x.p, slp, z(), inc());
		y.p = get_fy_potential(slp, z(), inc());
		mz.t = get_mz_tire(trq, x.p, slp, z(), inc());
		mz.p = get_mz_potential(z(), inc());
		ax_res = get_ax_resultant(x, y, str, io.yaw(i, j, k), io.v(i, j, k));
		trq_req = trq_req + get_trq_required(io.ax(i, j, k), ax_res, io.yaw(i, j, k));
		ay_res = get_ay_resultant(x, y, str, io.yaw(i, j, k));
		aa_res = get_aa_resultant(x, y, mz, str);
		r_res = get_radius(ay_res, io.v(i, j, k));

		if (pow(ay_res - ay_old, 2) < 0.0001 && pow(ax_res - ax_old, 2) < 0.0001 && pow(aa_res - aa_old, 2) < 0.0001) {
			static int count = 0;
			if (count) {
				// Cube outputs
				io.ay(i, j, k) = P * ay_res + (1 - P) * ay_old;
				io.aa(i, j, k) = P * aa_res + (1 - P) * aa_old;
				if (i > 0) {
					const double d_aa = io.aa(i, j, k) - io.aa(i - 1, j, k);
					io.stb(i, j, k) = d_aa / io.yaw_dlt;
				}
				if (i == 1) {
					io.stb(0, j, k) = io.stb(1, j, k);
				}
				if (j > 0) {
					const double d_aa = io.aa(i, j, k) - io.aa(i, j - 1, k);
					io.cnt(i, j, k) = d_aa / io.steer_dlt;
				}
				if (j == 1) {
					io.cnt(i, 0, k) = io.cnt(i, 1, k);
				}
				io.hve(i, j, k) = get_hve(bmp);
				io.rll(i, j, k) = get_rll(bmp);
				io.pch(i, j, k) = get_pch(bmp);
				io.r(i, j, k) = P * r_res + (1 - P) * r_old;

				// Object outputs
				io.x(i, j, k) = x;
				io.y(i, j, k) = y;
				io.z(i, j, k) = z;
				io.mz(i, j, k) = mz;
				io.inc(i, j, k) = inc;
				io.str(i, j, k) = str;
				io.bmp(i, j, k) = bmp;
				io.h(i, j, k) = h;
				io.trq(i, j, k) = trq;
				io.slp(i, j, k) = slp;

				// AX convergence error
				if (pow(io.ax(i, j, k) - ax_res, 2) > 0.0001) {
					io.v(i, j, k) = -1;
					io.ay(i, j, k) = nan("");
					io.aa(i, j, k) = nan("");
				}
				return;
			}
			count++;
		}
	}
	// Handle failure to iterate here
	io.ay(i, j, k) = nan("");
	io.aa(i, j, k) = nan("");
  // cout << "Failed to iterate at (" << i << ", " << j << ", " << k << ")." << endl;
}

// Compute a cube of yaw moment diagram instances into a ymd_v_io object
void get_ymd_const_v(ymd_v_io &io) {
	// Resize io matrix
	io.resize();

	// Recalculate all variables
	recalculate();
  
	// Populate using instances
	for (int k = 0; k < io.n_slices; ++k) {
		for (int j = 0; j < io.n_cols; ++j) {
			for (int i = 0; i < io.n_rows; ++i) {
				get_instance_const_v(io, i, j, k);
			}
		}
	}

  cout << "Generated YMD of size [" << io.n_rows << ", " << io.n_cols << ", " << io.n_slices << "]."<< endl;

	// Update maxes for colormap
	io.update_maxes();

}
};

#endif
