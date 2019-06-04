#ifndef WRITE_OR_DIE
#define WRITE_OR_DIE

#include <string>
#include <chrono>
#include <fstream>
#include <iterator>
#include <regex>
#include <ctime>
#include <vector>


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


class plain_mon {

 protected:  
   const std::string& doc;
   int start_words;

 public:
   virtual int word_count();
   int words_added();
   int speed_estimate(int duration);
   plain_mon(const std::string& doc_name); 
   virtual ~plain_mon();

};


class odf_mon : public plain_mon {
   
   std::regex wcount_reg;
   std::time_t old_atime;
   std::vector<char> meta_content; 
 
 public:
   odf_mon(const std::string& doc_name);  
   int word_count();

};


}

#endif
