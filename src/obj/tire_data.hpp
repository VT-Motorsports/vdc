#ifndef TIRE_DATA_HPP
#define TIRE_DATA_HPP

class tire_data {
public:
	// Global
	vec load; // Load cases [N]
	vec camber; // Camber [deg]
	// c - Cornering
	field<vec> c_sa; // Slip angle [deg]
	field<vec> c_ia; // Inclination angle [deg]
	field<vec> c_p; // Pressure [kPa]
	field<vec> c_fy; // Lateral force [N]
	field<vec> c_fz; // Vertical force [N]
	field<vec> c_mz; // Aligning moment [N.m]

	// db - Drive/brake
	field<vec> db_ia; // Inclination angle [deg]
	field<vec> db_p; // Pressure [kPa]
	field<vec> db_fx; // Longitudinal force [N]
	field<vec> db_fz; // Vertical force [N]
	field<vec> db_sr; // Slip ratio [-]

	// td.c_sa(i_load, j_cam) -- stored instead of trimming every frame

	tire_data(){
		load = vec({-222, -445, -667, -889, -1112});
		camber = vec({0, -2, -4});
		c_sa.set_size(load.n_elem, camber.n_elem);
		c_ia.set_size(load.n_elem, camber.n_elem);
		c_p.set_size(load.n_elem, camber.n_elem);
		c_fy.set_size(load.n_elem, camber.n_elem);
		c_fz.set_size(load.n_elem, camber.n_elem);
		c_mz.set_size(load.n_elem, camber.n_elem);
		db_ia.set_size(load.n_elem, camber.n_elem);
		db_p.set_size(load.n_elem, camber.n_elem);
		db_fx.set_size(load.n_elem, camber.n_elem);
		db_fz.set_size(load.n_elem, camber.n_elem);
		db_sr.set_size(load.n_elem, camber.n_elem);
	}
};

#endif
