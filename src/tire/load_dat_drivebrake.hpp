#ifndef LOAD_DAT_DRIVEBRAKE_HPP
#define LOAD_DAT_DRIVEBRAKE_HPP

void load_dat_drivebrake(tire_data &td, const string file_path, double pres){
	static const double pres_range = 1.05; // Tire window factor
	static const double load_range = 1.10; // Load window factor
	static const double cam_range = 0.1; // Camber range (+-)

	// Open .dat file (run ASCII)
	ifstream dat_file(file_path);
	if(!dat_file.is_open()){
		cerr << "Cornering data didn't load properly, check filepath." << endl;
		return;
	}

	// Skip file header
	string so_no_head;
	getline(dat_file, so_no_head);
	getline(dat_file, so_no_head);
	getline(dat_file, so_no_head);

	// Clean mixed delimiters from data
	stringstream clean_data;
	string line;
	while(getline(dat_file, line)){
		for (char &ch : line) {
			if (ch == '\t') 
				ch = ' '; // Replaces tabs with spaces
		}
		if (!line.empty()){
			clean_data << line << '\n';
		}
	}

	// Trim data by columns [IA | P | FX | FZ | SL]
	mat body;
	body.load(clean_data, raw_ascii);
	uvec cols_extracted = {4, 7, 8, 10, 21};
	const mat body_small = body.cols(cols_extracted);

	// Trim data by rows (pressure, individual camber & load)
	for (int i = 0; i < td.load.n_elem; ++i){
		for (int j = 0; j < td.camber.n_elem; ++j){
			const uvec indices = find(
				(body_small.col(1) >= pres / pres_range) && 
				(body_small.col(1) <= pres * pres_range) &&
				(body_small.col(3) >= td.load(i) * load_range) &&
				(body_small.col(3) <= td.load(i) / load_range) && 
				(body_small.col(0) >= -td.camber(j) - cam_range) && 
				(body_small.col(0) <= -td.camber(j) + cam_range)
				);
			const mat long_data = body_small.rows(indices);
			td.db_ia(i, j) = long_data.col(0);
			td.db_p(i, j) = long_data.col(1);
			td.db_fx(i, j) = long_data.col(2);
			td.db_fz(i, j) = long_data.col(3);
			td.db_sr(i, j) = long_data.col(4);
		}
	} 
	dat_file.close();
}

#endif
