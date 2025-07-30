#ifndef LOAD_VEHICLE_HPP
#define LOAD_VEHICLE_HPP

void load_vehicle(vehicle &car, const string file_path) {
	json car_json = read_json_file(file_path);
	if (!car_json.empty()){
		// Chassis
    car.m = car_json["Mass [kg]"].get<double>();
		car.i_zz = car_json["Inertia [kg.m²]"].get<double>();
		car.h_s = car_json["Center of Gravity Height [mm]"].get<double>();
		car.fw = car_json["Front Weight [%]"].get<double>();
    cout << "Loaded chassis!" << endl;

    // Aerodynamics
		car.cxa = car_json["Total CxA [m²]"].get<double>();
		car.cza = car_json["Total CzA [m²]"].get<double>();
		car.fa = car_json["Front Aero [%]"].get<double>();
    // cout << "Loaded aerodynamics!" << endl;

    // Powertrain
		car.ft = car_json["Front Torque [%]"].get<double>();
    // cout << "Loaded powertrain!" << endl;

    // Tires
		car.dia = car_json["Tire Diameter [mm]"].get<double>();
		car.p94_s = vec(car_json["Scaling Factors [%]"].get<vector<double>>());
		car.p94_a = vec(car_json["Lateral Pacejka94 Coefficients (a)"].get<vector<double>>());
		car.p94_b = vec(car_json["Longitudinal Pacejka94 Coefficients (b)"].get<vector<double>>());
		car.p94_c = vec(car_json["Alignment Pacejka94 Coefficients (c)"].get<vector<double>>());
		car.kp_f = car_json["Front Tire Stiffness [N/mm]"].get<double>();
		car.kp_r = car_json["Rear Tire Stiffness [N/mm]"].get<double>();
    // cout << "Loaded tires!" << endl;

    // Suspension (Design)
		car.l = car_json["Wheelbase [mm]"].get<double>();
		car.t_f = car_json["Front Track Width [mm]"].get<double>();
		car.t_r = car_json["Rear Track Width [mm]"].get<double>();
		car.adi = car_json["Anti-Dive [%]"].get<double>();
		car.asq = car_json["Anti-Squat [%]"].get<double>();
		car.ack = car_json["Ackermann Rate [°/°²]"].get<double>();
		car.rat = car_json["Steering Ratio [-]"].get<double>();
		car.rc_f = car_json["Front RC Height [mm]"].get<double>();
		car.rc_r = car_json["Rear RC Height [mm]"].get<double>();
		car.cag_f = car_json["Front Camber Gain [°/mm]"].get<double>();
		car.cag_r = car_json["Rear Camber Gain [°/mm]"].get<double>();
		car.tog_f = car_json["Front Toe Gain [°/mm]"].get<double>();
		car.tog_r = car_json["Rear Toe Gain [°/mm]"].get<double>();
		car.mrs_f = car_json["Front Motion Ratio [-]"].get<double>();
		car.mrs_r = car_json["Rear Motion Ratio [-]"].get<double>();
    // cout << "Loaded suspension (design)!" << endl;

    // Suspension (Tuning)
		car.cam_f = car_json["Front Static Camber [°]"].get<double>();
		car.cam_r = car_json["Rear Static Camber [°]"].get<double>();
		car.toe_f = car_json["Front Static Toe [°]"].get<double>();
		car.toe_r = car_json["Rear Static Toe [°]"].get<double>();
		car.rh_f = car_json["Front Ride Height Offset [mm]"].get<double>();
		car.rh_r = car_json["Rear Ride Height Offset [mm]"].get<double>();
		car.fb = car_json["Front Braking [%]"].get<double>();
		car.ks_f = car_json["Front Spring Stiffness [N/mm]"].get<double>();
		car.ks_r = car_json["Rear Spring Stiffness [N/mm]"].get<double>();
		car.ka_f = car_json["Front ARB Stiffness [N/mm]"].get<double>();
		car.ka_r = car_json["Rear ARB Stiffness [N/mm]"].get<double>();
    // cout << "Loaded suspension (tuning)!" << endl;
	}
}

#endif
