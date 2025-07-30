#ifndef MISC_CLASSES_HPP
#define MISC_CLASSES_HPP

/**
 * Wheel Forces: Longitudinal (x)
 *
 * t: Tire forces
 * p: Possible (maximum) forces for given conditions
 *
 * Forces are given in a tire coordinate system.
 * To get forces in body coordinates, reframe() is used.
 */
class fx_whl {
public:
	vec t, p;

	fx_whl() {
		t = {0, 0, 0, 0};
		p = {0, 0, 0, 0};
	}

	vec reframe(const vec &fy_whl_t, const vec &str) const {
		return t % cos(str / 57.3) - fy_whl_t % sin(str / 57.3);
	}
};
/**
 * Wheel Forces: Lateral (y)
 *
 * t: Tire forces
 * p: Possible (maximum) forces for given conditions
 *
 * Forces are given in a tire coordinate system.
 * To get forces in body coordinates, reframe() is used.
 */
class fy_whl {
public:
	vec t, p;

	fy_whl() {
		t = {0, 0, 0, 0};
		p = {0, 0, 0, 0};
	}

	vec reframe(const vec &fx_whl_t, const vec &str) const {
		return t % cos(str / 57.3) + fx_whl_t % sin(str / 57.3);
	}
};
/**
 * Wheel Forces: Vertical (z)
 *
 * Components:
 * s: Static loading
 * a: Aerodynamic loading
 * l: Longitudinal load transfer
 * n: Lateral load transfer
 * (): Sum of vertical components
 *
 * Forces are given in a tire/wheel coordinate system.
 * Forces are represented z-up (ISO), which gives negative contact forces.
 */
class fz_whl {
public:
	vec s, a, l, n;

	fz_whl() {
		s = {0, 0, 0, 0};
		a = {0, 0, 0, 0};
		l = {0, 0, 0, 0};
		n = {0, 0, 0, 0};
	}

	vec operator()() const {
		return s + a + l + n;
	}
};
/**
 * Aligning Moments
 *
 * t: Tire forces
 * p: Possible (maximum) forces for given conditions
 *
 */
class mz_whl {
public:
	vec t, p;

	mz_whl() {
		t = {0, 0, 0, 0};
		p = {0, 0, 0, 0};
	}
};
/**
 * Wheel Inclination
 *
 * Components:
 * s: Static inclination (alignment)
 * e: Steered inclination (from kingpin)
 * d: Dynamic inclination (from kinematic camber gain)
 * (): Sum of component angles
 *
 * Inclination is the counter-clockwise (ISO) rotation of the wheel around the
 * wheel's longitudinal axis. Camber is inclination * [-1, +1, -1, +1].
 * Angles are linearized as an approximation.
 */
class inc_whl {
public:
	vec s, e, d;

	inc_whl() {
		s = {0, 0, 0, 0};
		e = {0, 0, 0, 0};
		d = {0, 0, 0, 0};
	}

	vec operator()() const {
		return s + e + d;
	}
};
/**
 * Wheel Steer
 *
 * Components:
 * s: Static steer (alignment)
 * e: Steered steer (from steering)
 * d: Dynamic steer (from kinematic toe gain)
 * (): Sum of component angles
 *
 * Steer is the counter-clockwise (ISO) rotation of the wheel around the
 * wheel's vertical axis. Toe is inclination * [-1, +1, -1, +1].
 * Angles are linearized as an approximation.
 */
class str_whl {
public:
	vec s, e, d;

	str_whl() {
		s = {0, 0, 0, 0};
		e = {0, 0, 0, 0};
		d = {0, 0, 0, 0};
	}

	vec operator()() const {
		return s + e + d;
	}
};
/**
 * Wheel Bump
 *
 * Components:
 * s: Static bump (ride height)
 * d: Dynamic bump (from dynamic forces)
 * j: Jacking bump (from longitudinal/lateral jacking forces)
 * (): Sum of component displacements
 *
 * Bump is the vertical displacement (Z-up) of the wheel relative to the CG.
 * Negative bump = "droop".
 * Bump is linearized as an approximation.
 */
class bmp_whl {
public:
	vec s, d, j;

	bmp_whl() {
		s = {0, 0, 0, 0};
		d = {0, 0, 0, 0};
		j = {0, 0, 0, 0};
	}

	vec operator()() const {
		return s + d + j;
	}
};
/**
 * Center of Gravity
 *
 * Components:
 * s: Static CG height
 * d: Dynamic CG displacement
 * j: Jacking CG displacement
 * (): Static height + displacements
 *
 * CG height is given as vertical (Z-up) displacement relative to the ground.
 */
class cg {
public:
	double s, d, j;

	cg() {
		s = 0;
		d = 0;
		j = 0;
	}

	cg(const double &h_s) {
		s = h_s;
		d = 0;
		j = 0;
	}

	double operator()() const {
		return s + d + j;
	}
};

/**
 * YMD Constant Velocity Inputs
 *
 * Input Variables:
 * v: Velocity
 * ax_lo: Low-end longitudinal acceleration (max braking case)
 * ax_hi: High-end longitudinal acceleration (max throttle case)
 * ax_ct: Acceleration plot count
 * sip_rng: Sideslip/yaw range
 * sip_ivl: Sideslip/yaw interval
 * str_rng: Steer range (at steering wheel)
 * str_ivl: Steer interval
 *
 * Odd acceleration plot counts always have a median ax=0 plot.
 * Default values are set for code brevity.
 */
class ymd_v_in {
public:
	double v; // velocity (const)
	double ax_lo, ax_hi; // ax low, high
	int ax_ct; // ax count
	double sip_rng, sip_ivl; // sideslip range, interval
	double str_rng, str_ivl; // steer range, interval

	ymd_v_in() {
		v = 12;
		ax_lo = -12;
		ax_hi = 9;
		ax_ct = 3;
		sip_rng = 10;
		sip_ivl = 0.40;
		str_rng = 80;
		str_ivl = 4;
	}
};

/**
 * YMD Constant Velocity Output (Single)
 *
 * Output (in sideslip-frame):
 * v: Velocity (input)
 * yaw: Sideslip (input)
 * steer: Steer (input)
 * ax: Longitudinal acceleration
 * ay: Lateral acceleration
 * aa: Yaw acceleration
 * r: Turn radius
 * h: CG height
 * x: Longitudinal wheel forces
 * y: Lateral wheel forces
 * z: Vertical wheel forces
 * mz: Alignment moments
 * inc: Inclinations
 * str: Steers
 * bmp: Bumps
 * trq: Torques
 * slp: Slip angles
 * hve: Heave
 * rll: Roll
 * pch: Pitch
 * stb: Stability
 * cnt: Control
 */
class ymd_v_out {
public:
	double v;
	double yaw;
	double steer;
	double ax;
	double ay;
	double aa;
	double r;
	cg h;
	fx_whl x;
	fy_whl y;
	fz_whl z;
	mz_whl mz;
	inc_whl inc;
	str_whl str;
	bmp_whl bmp;
	vec trq;
	vec slp;
	double hve, rll, pch;
	double stb, cnt;
};

class ymd_v_io {
public:
	// Inputs
	bool update;
	int ax_plot, plot_type, i_tt, j_tt;
	int ax_ct;
	double speed, yaw_rng, yaw_dlt, steer_rng, steer_dlt, ax_lo, ax_hi;

	// Element-wise Inputs
	cube v, yaw, steer, ax;

	// Sizing
	int n_rows, n_cols, n_slices;

	// Cube Outputs
	cube ay, aa, stb, cnt, hve, rll, pch, r;

	// Colormap Outputs
	vec stb_min, stb_max, cnt_min, cnt_max;
	cube stb_norm, cnt_norm;

	// Field Outputs
	field<fx_whl> x; // io.x(i, j, k).t
	field<fy_whl> y;
	field<fz_whl> z;
	field<mz_whl> mz;
	field<inc_whl> inc;
	field<str_whl> str;
	field<bmp_whl> bmp;
	field<cg> h;
	field<vec> trq, slp;

	// Initialization
	ymd_v_io() {
		// Set update condition
		update = false;
		// Set displayed plot
		ax_plot = 0;
		plot_type = 1;
		i_tt = 0;
		j_tt = 0;
		// Zero elements
		n_rows = 0;
		n_cols = 0;
		n_slices = 0;
		// Set default inputs
		speed = 12.0;
		yaw_rng = 10.0;
		yaw_dlt = 1;
		steer_rng = 80.0;
		steer_dlt = 8;
		ax_ct = 3;
		ax_lo = -12.0;
		ax_hi = 9.0;
	}
	// Set up after any values change
	void resize() {
		// Reset update condition when called
		update = false;

		// Setup yaw_vals as range from -range:0:range
		vec yaw_vals;
		yaw_vals = join_cols(regspace(-yaw_rng, yaw_dlt, 0),
							regspace(yaw_dlt, yaw_dlt, yaw_rng));
		int i = yaw_vals.n_elem;

		// Setup steer_vals as range from -range:0:range
		vec steer_vals;
		steer_vals = join_cols(regspace(-steer_rng, steer_dlt, 0),
							regspace(steer_dlt, steer_dlt, steer_rng));
		int j = steer_vals.n_elem;

		// Setup ax_vals as range from min:0:max
		vec ax_vals;
		if (ax_ct > 1) {
			ax_vals = join_cols(linspace<vec>(ax_lo, 0, ax_ct / 2 + 1),
			                        linspace<vec>(0, ax_hi, ax_ct / 2 + 1));
			ax_vals.shed_row(ax_ct / 2); // Odd or even
			if (!(ax_ct % 2)){ax_vals.shed_row(ax_ct / 2);} // Even
		}
		else {
			ax_vals = vec({0});
		}
		int k = ax_vals.n_elem;

		// Setup input matrices
		v.set_size(i, j, k);
		v.fill(speed);
		yaw.set_size(i, j, k);
		for (int i_ = 0; i_ < i; ++i_) {
			yaw.row(i_).fill(yaw_vals(i_));
		}
		steer.set_size(i, j, k);
		for (int i_ = 0; i_ < j; ++i_) {
			steer.col(i_).fill(steer_vals(i_));
		}
		ax.set_size(i, j, k);
		for (int i_ = 0; i_ < k; ++i_) {
			ax.slice(i_).fill(ax_vals(i_));
		}

		// Size attributes
		n_rows = i;
		n_cols = j;
		n_slices = k;

		// Size cubes
		ay.zeros(i, j, k);
		aa.zeros(i, j, k);
		stb.zeros(i, j, k);
		cnt.zeros(i, j, k);
		hve.zeros(i, j, k);
		rll.zeros(i, j, k);
		pch.zeros(i, j, k);
		r.zeros(i, j, k);

		// Size fields
		x.set_size(i, j, k);
		y.set_size(i, j, k);
		z.set_size(i, j, k);
		mz.set_size(i, j, k);
		inc.set_size(i, j, k);
		str.set_size(i, j, k);
		bmp.set_size(i, j, k);
		h.set_size(i, j, k);
		trq.set_size(i, j, k);
		slp.set_size(i, j, k);
	}
	void update_maxes() {
		stb_min.set_size(n_slices);
		stb_max.set_size(n_slices);
		cnt_min.set_size(n_slices);
		cnt_max.set_size(n_slices);
		stb_norm = zeros(n_rows, n_cols, n_slices);
		cnt_norm = zeros(n_rows, n_cols, n_slices);
		for (int k = 0; k < n_slices; ++k) {
			stb_min(k) = stb.slice(k).min();
			stb_max(k) = stb.slice(k).max();
			cnt_min(k) = cnt.slice(k).min();
			cnt_max(k) = cnt.slice(k).max();
			stb_norm.slice(k) = (stb.slice(k) - stb_min(k)) / (stb_max(k) - stb_min(k));
			cnt_norm.slice(k) = (cnt.slice(k) - cnt_min(k)) / (cnt_max(k) - cnt_min(k));
		}
	}
};



#endif
