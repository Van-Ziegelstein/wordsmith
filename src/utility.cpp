#include "utility.h"
#include <iostream>
#include <fstream>
#include <curses.h>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <csignal>
#include <string.h>

namespace sprint {

void exit_cleanup(int sig) {

  int status;

  if (has_colors())
      attroff(COLOR_PAIR(TIMER_COLOR));


  if (sig == SIGABRT) {

     printw("\n\nWhoops a problem occured.\n");
     status = EXIT_FAILURE;
  }   
  else {

     printw("\n\nAborted!\n");   
     status = EXIT_SUCCESS;
  }

  refresh();
  std::this_thread::sleep_for(std::chrono::seconds(1)); 
  endwin();

  exit(status);

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


docformat check_doctype(const std::string& fpath) {

  unsigned char filesig[4] = {0};
  const unsigned char zipmagic[4] = { 0x50, 0x4B, 0x03, 0x04 };
  docformat ftype = plain;


  std::ifstream raw_file(fpath, std::ios::binary);

  if (raw_file.fail()) {

     std::cout << "Couldn't open input file" << std::endl;
     exit(EXIT_FAILURE);

  }

  raw_file.read((char *)filesig, 4);

  if (memcmp(filesig, zipmagic, sizeof(filesig)) == 0)
     ftype = zip;

  return ftype;

}


void usage(void) {

   std::cout << "A small utility to track writing progress.\n"
                "Basic usage:\n"
		<< PROG_NAME
		<< " -f <path-to-manuscript> -t <sprint-time-in-minutes>\n\n";
		
   exit(EXIT_SUCCESS);

}

}
