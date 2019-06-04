#include <iostream>
#include <thread>
#include <sstream>
#include <unistd.h>
#include <curses.h>
#include <csignal>
#include <cstdlib>
#include "utility.h"
#include "tracker.h"



int main(int argc, char *argv[]) {

  int cmd_opt, x_pos, y_pos, key, sprint_duration = 0;
  std::string file_path, file_name;
  std::chrono::seconds time_frame;
  sprint::plain_mon* text;
  sprint::docformat filetype;


  if (argc == 1) {
     
     sprint::usage();
     exit(EXIT_SUCCESS);

  }    

  opterr = 0;

  while ((cmd_opt = getopt(argc, argv, ":hf:t:")) != -1) {
        
        switch (cmd_opt) {

	       case 'h':
	       sprint::usage();
	       break;

               case 'f':
               file_path = optarg;
               break;

               case 't':
               std::istringstream(optarg) >> sprint_duration;    
               break;

               case '?':
               std::cout << "Invalid option." << std::endl;
               exit(EXIT_FAILURE);
               break;

               case ':':
               std::cout << "Missing option argument." << std::endl;
               exit(EXIT_FAILURE);
               break;

               default: 
               sprint::usage();
               exit(EXIT_SUCCESS);

        }

  }


  if (sprint_duration <= 0 || sprint_duration > 1440) {

     std::cout << "You must be kidding..." << std::endl;
     exit(EXIT_FAILURE);

  }


  sprint::time_frags timer(sprint_duration);
  file_name = file_path.substr(file_path.find_last_of("/\\") + 1);

  sprint::curses_init();
  std::signal(SIGINT, sprint::exit_cleanup);
  std::signal(SIGTERM, sprint::exit_cleanup);
  std::signal(SIGABRT, sprint::exit_cleanup);

  filetype = sprint::check_doctype(file_path);

  switch(filetype) {

     case sprint::plain:
     text = new sprint::plain_mon(file_path);
     break;

     case sprint::zip:
     text = new sprint::odf_mon(file_path);  
     break;

  }

  printw("File: %s\nCurrent word count: %d\n\n", file_name.c_str(), text->word_count());
  getyx(stdscr, y_pos, x_pos);

  if (has_colors())
     attron(COLOR_PAIR(TIMER_COLOR));

  auto start_time = std::chrono::steady_clock::now();
  auto current_time = start_time;


  while (! timer.is_finished()) { 
  
     current_time = std::chrono::steady_clock::now();
     time_frame = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
     timer.breakup(time_frame);

     move(y_pos, x_pos);
     printw("Time: %02d:%02d:%02d", timer.hours(), timer.minutes(), timer.seconds());
     refresh();

     std::this_thread::sleep_for(std::chrono::milliseconds(500));

  }


  if (has_colors())
     attroff(COLOR_PAIR(TIMER_COLOR));

  printw("\nTime's up!\nSave your document and then hit c to view your progress.\n\n");
  refresh();
  
  do {
   
     key = getch();
   
  } while (key != 'c');


  printw("New word count: %d\nWords added: %d\nAverage speed: %d words per hour\n", text->word_count(), text->words_added(), text->speed_estimate(sprint_duration)); 
  refresh();

  getch();
  endwin();  
  delete text;

}
