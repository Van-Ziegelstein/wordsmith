#ifndef TIME_MANAGEMENT
#define TIME_MANAGEMENT

#include <string>

#define TIMER_COLOR 1
#define DEFAULT_COLOR -1

namespace sprint {

enum docformat { plain, zip };

void curses_init(void);

void exit_cleanup(int sig);

docformat check_doctype(const std::string& fpath);

void usage(void);


}


#endif
