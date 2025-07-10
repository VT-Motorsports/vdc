#ifndef LOAD_DAT_CORNERING_HPP
#define LOAD_DAT_CORNERING_HPP

void load_dat_cornering(tire_data &td, const string file_path, double pres){
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

	// Trim data by columns [SA | IA | P | FY | FZ | MZ]
	mat body;
	body.load(clean_data, raw_ascii);
	uvec cols_extracted = {3, 4, 7, 9, 10, 12};
	const mat body_small = body.cols(cols_extracted);

	// Trim data by rows (pressure, individual camber & load)
	for (int i = 0; i < td.load.n_elem; ++i){
		for (int j = 0; j < td.camber.n_elem; ++j){
			const uvec indices = find(
				(body_small.col(2) >= pres / pres_range) && 
				(body_small.col(2) <= pres * pres_range) &&
				(body_small.col(4) >= td.load(i) * load_range) &&
				(body_small.col(4) <= td.load(i) / load_range) && 
				(body_small.col(1) >= -td.camber(j) - cam_range) && 
				(body_small.col(1) <= -td.camber(j) + cam_range)
				);
			const mat lat_data = body_small.rows(indices);
			td.c_sa(i, j) = lat_data.col(0);
			td.c_ia(i, j) = lat_data.col(1);
			td.c_p(i, j) = lat_data.col(2);
			td.c_fy(i, j) = lat_data.col(3);
			td.c_fz(i, j) = lat_data.col(4);
			td.c_mz(i, j) = lat_data.col(5);
		}
	} 
	dat_file.close();
}

#endif