#ifndef WRITE_OR_DIE
#define WRITE_OR_DIE

#include <chrono>
#include <fstream>
#include <iterator>


namespace sprint {

class time_frags {
     
     int total_minutes;
     int max_time;
     int frags[3];

  public:
     int seconds() { return frags[0]; }
     int minutes() { return frags[1]; }
     int hours() { return frags[2]; }
     int is_finished();
     void breakup(const std::chrono::seconds& timestamp);
     time_frags(int max) : total_minutes(0), max_time(max), frags() { }

};


class content_watch {

   std::istream_iterator<std::string> word_it, end;
   std::streampos beginning;
   std::ifstream& stream_src;
   int start_words;

 public:
   int word_count();
   int words_added();
   int speed_estimate(int duration);
   void resync(std::string doc_name);
   content_watch(std::ifstream& file); 

};


}


#endif
