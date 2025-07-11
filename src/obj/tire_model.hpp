#ifndef TIRE_MODEL_HPP
#define TIRE_MODEL_HPP

class tire_model {
public:
	// Pacejka Coefficients
	vec a, b, c;
	double sa, sb, sc;
	
	tire_model(){
		a = {1.20, 0, 2.0, 400, 1, 0, 1, -2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		b = {1.5, 0.9, 3.0, 0, -60, -0.300, 0, 0, -2, 0, 0, 0, 0, 0, 0.010, 0.25};
		c = {2.20, -0.025, 0.025, -220, 20.0, 0.050, -0.60, -2.00, 0.20, 0, 0.20, 0, 0, -12.0, -20.0, 0.007, -0.10, 0};
		sa = 0.6;
		sb = 0.7;
		sc = 0.6;
	}
	
	vector<double> get_fx(const double &load, const double &cam, const vec &sr){
		vector<double> fx(sr.size());
		const double fzn = load; // Fz in Newtons
		const double fzkn = fzn / 1000; // Fz in Kilonewtons
		for (int i = 0; i < sr.size(); i++) {
			const double C = b[0];
			const double D = fzn * (b[1] * fzkn + b[2]) * (1 - b[14] * pow(cam, 2));
			const double BCD = (b[3] * fzkn * fzkn + b[4] * fzn) * exp(-b[5] * fzkn);
			const double B = BCD / (C * D);
			const double H = b[9] * fzkn + b[10];
			const double V = b[11] * fzn + b[12];
			const double E = (b[6] * fzkn * fzkn + b[7] * fzkn + b[8]) * (1 - (b[15] * cam) - b[13] * sign(sr[i] + H));
			const double Cs = C;
			const double Ds = D * sb;
			const double Bs = B / sb;
			const double Hs = H * sb;
			const double Vs = V * sb;
			const double Es = E;
			const double Bx1s = Bs * (sr(i) + Hs);
			fx[i] = Ds * sin(Cs * atan(Bx1s - Es * (Bx1s - atan(Bx1s)))) + Vs;
		}
		return fx;
	}
	
	vector<double> get_fy(const double &load, const double &cam, const vec &sa){
		vector<double> fy(sa.size());
		const double fzn = load; // Fz in Newtons
		const double fzkn = fzn / 1000; // Fz in Kilonewtons
		for (int i = 0; i < sa.size(); i++) {
			const double C = a[0];
			const double D = fzn * (a[1] * fzkn + a[2]) * (1 - a[15] * pow(cam, 2));
			const double BCD = a[3] * sin(atan(fzkn / a[4]) * 2) * (1 - a[5] * abs(cam));
			const double B = BCD / (C * D);
			const double H = a[8] * fzkn + a[9] + a[10] * cam;
			const double V = a[11] * fzn + a[12] + (a[13] * fzkn + a[14]) * cam * fzkn;
			const double E = (a[6] * fzkn + a[7]) * (1 - (a[16] * cam + a[17]));
			const double Cs = C;
			const double Ds = D * sa;
			const double Bs = B / sa;
			const double Hs = H * sa;
			const double Vs = V * sa;
			const double Es = E;
			const double Bx1s = Bs * (sa(i) + Hs);
			fy[i] = Ds * sin(Cs * atan(Bx1s - Es * (Bx1s - atan(Bx1s)))) + Vs;
		}
		return fy;
	}
	
	vector<double> get_fy(const double &load, const double &cam, const vec &sa){
		vector<double> fy(sa.size());
		const double fzn = load; // Fz in Newtons
		const double fzkn = fzn / 1000; // Fz in Kilonewtons
		for (int i = 0; i < sa.size(); i++) {
			const double C = c[0];
			const double D = fzn * (c[1] * fzkn + c[2]) * (1 - c[15] * pow(cam, 2));
			const double BCD = c[3] * sin(atan(fzkn / c[4]) * 2) * (1 - c[5] * abs(cam));
			const double B = BCD / (C * D);
			const double H = c[8] * fzkn + c[9] + c[10] * cam;
			const double V = c[11] * fzn + c[12] + (c[13] * fzkn + c[14]) * cam * fzkn;
			const double E = (c[6] * fzkn + c[7]) * (1 - (c[16] * cam + c[17]));
			const double Cs = C;
			const double Ds = D * sc;
			const double Bs = B / sc;
			const double Hs = H * sc;
			const double Vs = V * sc;
			const double Es = E;
			const double Bx1s = Bs * (sa(i) + Hs);
			mz[i] = Ds * sin(Cs * atan(Bx1s - Es * (Bx1s - atan(Bx1s)))) + Vs;
		}
		return mz;
	}
};

#endif