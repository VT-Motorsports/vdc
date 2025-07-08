#ifndef WHICH_GUI_HPP
#define WHICH_GUI_HPP

enum which_gui {
	base,
	tire,
	ymd,
	lts
};

inline which_gui current_gui = base;

#endif