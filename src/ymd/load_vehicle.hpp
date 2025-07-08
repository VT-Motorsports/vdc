#ifndef LOAD_VEHICLE_HPP
#define LOAD_VEHICLE_HPP

void load_vehicle(vehicle &car, const string file_path) {
	json car_json = read_json_file(file_path);
	if (!car_json.empty()){
		car.fw = car_json["Front Weight [%]"].get<double>() / 100;
		car.ft = car_json["Front Torque [%]"].get<double>() / 100;
		car.fb = car_json["Front Braking [%]"].get<double>() / 100;
		car.cxa = car_json["Total CxA [m²]"].get<double>();
		car.cza_f = car_json["Front CzA [m²]"].get<double>();
		car.cza_r = car_json["Rear CzA [m²]"].get<double>();
		car.m = car_json["Mass [kg]"].get<double>();
		car.i_zz = car_json["Inertia [kg.m²]"].get<double>();
		car.h_s = car_json["Center of Gravity Height [mm]"].get<double>() / 1000;
		car.l = car_json["Wheelbase [mm]"].get<double>() / 1000;
		car.dia = car_json["Tire Diameter [mm]"].get<double>() / 1000;
		car.rat = car_json["Steering Ratio [-]"].get<double>();
		car.ack = car_json["Ackermann Rate [°/°²]"].get<double>();
		car.adi = car_json["Anti-Dive [%]"].get<double>() / 100;
		car.asq = car_json["Anti-Squat [%]"].get<double>() / 100;
		car.rc_f = car_json["Front RC Height [mm]"].get<double>() / 1000;
		car.rc_r = car_json["Rear RC Height [mm]"].get<double>() / 1000;
		car.rh_f = car_json["Front Ride Height Offset [mm]"].get<double>() / 1000;
		car.rh_r = car_json["Rear Ride Height Offset [mm]"].get<double>() / 1000;
		car.cam_f = car_json["Front Static Camber [°]"].get<double>();
		car.cam_r = car_json["Rear Static Camber [°]"].get<double>();
		car.toe_f = car_json["Front Static Toe [°]"].get<double>();
		car.toe_r = car_json["Rear Static Toe [°]"].get<double>();
		car.cag_f = car_json["Front Camber Gain [°/m]"].get<double>();
		car.cag_r = car_json["Rear Camber Gain [°/m]"].get<double>();
		car.tog_f = car_json["Front Toe Gain [°/m]"].get<double>();
		car.tog_r = car_json["Rear Toe Gain [°/m]"].get<double>();
		car.ks_f = car_json["Front Spring Stiffness [N/mm]"].get<double>() * 1000;
		car.ks_r = car_json["Rear Spring Stiffness [N/mm]"].get<double>() * 1000;
		car.kp_f = car_json["Front Tire Stiffness [N/mm]"].get<double>() * 1000;
		car.kp_r = car_json["Rear Tire Stiffness [N/mm]"].get<double>() * 1000;
		car.ka_f = car_json["Front ARB Stiffness [N/mm]"].get<double>() * 1000;
		car.ka_r = car_json["Rear ARB Stiffness [N/mm]"].get<double>() * 1000;
		car.mrs_f = car_json["Front Spring Motion Ratio [-]"].get<double>();
		car.mrs_r = car_json["Rear Spring Motion Ratio [-]"].get<double>();
		car.p94_a = vec(car_json["Lateral Pacejka94 Coefficients (a)"].get<vector<double>>());
		car.p94_b = vec(car_json["Longitudinal Pacejka94 Coefficients (b)"].get<vector<double>>());
		car.p94_c = vec(car_json["Alignment Pacejka94 Coefficients (c)"].get<vector<double>>());
		car.p94_s = vec(car_json["Scaling Factors"].get<vector<double>>());
	}
}

#endif