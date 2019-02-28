#ifndef TIME_MANAGEMENT
#define TIME_MANAGEMENT

#include <string>

#define TIMER_COLOR 1
#define DEFAULT_COLOR -1

namespace sprint {

void curses_init(void);

void exit_cleanup(int sig);

void usage(void);


}


#endif
