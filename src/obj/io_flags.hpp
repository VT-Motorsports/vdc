#ifndef IO_FLAGS_HPP
#define IO_FLAGS_HPP

class io_flags {
public:
	bool is_tc_loaded;
	bool is_tdb_loaded;
	int tire_plot_type;
	
	io_flags(){
		is_tc_loaded = false;
		is_tdb_loaded = false;
		tire_plot_type = 3;
	}
};

#endif