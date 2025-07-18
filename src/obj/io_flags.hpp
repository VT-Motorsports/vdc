#ifndef IO_FLAGS_HPP
#define IO_FLAGS_HPP

class io_flags {
public:
	bool is_tc_loaded, is_tdb_loaded;
	int tire_plot_type;
  bool is_veh_loaded;
	
	io_flags(){
		is_tc_loaded = false;
		is_tdb_loaded = false;
		tire_plot_type = 3;
    is_veh_loaded = false;
	}
};

#endif
