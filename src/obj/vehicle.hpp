#ifndef VEHICLE_HPP
#define VEHICLE_HPP

class vehicle {
public:
	//
	// Loaded Variables
	//
	double fw; // front weight [-]
	double ft; // front torque [-]
	double fb; // front braking [-]
	double cxa; // drag coefficient * unit area [m²]
	double cza_f, cza_r; // drag coefficient * unit area [m²]
	double m; // mass [kg]
	double i_zz; // inertia [kg.m²]
	double h_s; // static center of gravity height [m]
	double l; // wheelbase [m]
	double dia; // full tire diameter [m]
	double rat; // steering ratio [-]
	double ack; // ackermann rate [°/°²]
	double adi; // anti-dive [-]
	double asq; // anti-squat [-]
	double rc_f, rc_r; // roll centers [m]
	double rh_f, rh_r; // static ride heights [m]
	double cam_f, cam_r; // static camber [°]
	double toe_f, toe_r; // static toe [°]
	double cag_f, cag_r; // camber gain [°/m]
	double tog_f, tog_r; // toe gain [°/m]
	double kp_f, kp_r; // tire stiffness [N/m]
	double ks_f, ks_r; // spring stiffness [N/m]
	double ka_f, ka_r; // arb stiffness [N/m]
	double mrs_f, mrs_r; // motion ratio [-]
	vec p94_a; // pacejka94 lateral coefficients [~]
	vec p94_b; // pacejka94 longitudinal coefficients [~]
	vec p94_c; // pacejka94 alignment coefficients [~]
	vec p94_s; // pacejka94 scaling coefficients [~]
	//
	// Calculated Variables
	//
	double a; // front axle offset from cg [m]
	double b; // rear axle offset from cg [m]
	double kw_f, kw_r; // combined wheel rates (heave/pitch mode) [N/m]
	double kr, kr_f, kr_r; // roll rates [N.m/°]
	double krs_f, krs_r; // sprung roll rates [N.m/°]
	double krp_f, krp_r; // pneumatic roll rates [N.m/°]
	//
	// Constructor
	//
	vehicle(){
	  p94_s.resize(3);	
	}
};

#endif
