#include "utility.h"
#include <iostream>
#include <curses.h>
#include <chrono>
#include <thread>
#include <cstdlib>


namespace sprint {

void exit_cleanup(int sig) {

  if (has_colors())
      attroff(COLOR_PAIR(TIMER_COLOR));

  printw("\n\nAborted!\n");
  refresh();
  std::this_thread::sleep_for(std::chrono::seconds(1)); 
  endwin();
  exit(EXIT_SUCCESS);

}


void curses_init(void) {

  initscr();
  noecho();
  cbreak();

  if (has_colors()) {

      start_color();
      use_default_colors();
      init_pair(TIMER_COLOR, COLOR_CYAN, DEFAULT_COLOR);

      if (can_change_color())
          init_color(COLOR_CYAN, 60, 920, 920);

  }

}


void usage(void) {

   std::cout << "This is a small utility to track writing progress.\n"
                "Its main purpose is to help with writing sprints.\n"
                "The basic usage is:\n"
		<< PROG_NAME
		<< " -f <path-to-manuscript> -t <sprint-time-in-minutes>\n\n";
		
   exit(EXIT_SUCCESS);

}

}
