#include "utility.h"
#include <iostream>
#include <fstream>
#include <curses.h>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <csignal>
#include <string.h>
#include <magic.h>

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

  docformat ftype;
  magic_t cookie = magic_open(MAGIC_MIME_TYPE | MAGIC_ERROR);

  if (!cookie) {

     std::cout << "Could not allocate magic cookie." << std::endl;
     exit(EXIT_FAILURE);

  }

  if (magic_load(cookie, NULL) != 0) {

     std::cout << "Error: " << magic_error(cookie) << std::endl;
     exit(EXIT_FAILURE);

  }

  const char *mimetype = magic_file(cookie, fpath.c_str());

  if (!mimetype) {
     
     std::cout << "Error: " << magic_error(cookie) << std::endl;
     exit(EXIT_FAILURE);

  }


  if (strncmp(mimetype, "application/vnd.oasis.opendocument.text", strlen(mimetype)) == 0)
     ftype = odt;
  else if (strncmp(mimetype, "application/vnd.openxmlformats-officedocument.wordprocessingml.document", strlen(mimetype)) == 0)
     ftype = docx;
  else
     ftype = plain;

  magic_close(cookie);
  
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
