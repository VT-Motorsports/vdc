#ifndef LOAD_JSON_FILE_HPP
#define LOAD_JSON_FILE_HPP

json read_json_file(const string file_path) {
    json j;
    try {
		ifstream file(file_path);
		if (!file.is_open()){
			throw runtime_error(file_path);
		}
		file >> j;
	} catch (const exception& e){
		cerr << "Error reading JSON file: " << e.what() << endl;
		return json{}; // Return empty json
	}
    return j;
}

#endif