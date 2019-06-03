#include "tracker.h"
#include <csignal>


namespace sprint {

void time_frags::breakup(const std::chrono::seconds& timestamp) {

        int remnant;
	auto num_seconds = timestamp.count();
    
        if (num_seconds >= 60) {
    
	    frags[0] = num_seconds % 60;
            remnant = num_seconds / 60;
	    total_minutes = remnant;
    
            if (remnant >= 60) {
    
               frags[1] = remnant % 60;
               frags[2] = remnant / 60;
        
            } 
            else  
                frags[1] = remnant;
          }
          else 
             frags[0] = num_seconds;

} 


int time_frags::is_finished() {

    if (total_minutes >= max_time)
       return 1;
    else
       return 0;

}


int plain_mon::word_count() {

    int word_total = std::distance(word_it, end);
    stream_src.clear();
    stream_src.seekg(beginning);
    return word_total;

}


int plain_mon::words_added () {

    int current_words = word_count();

    if (start_words > current_words)
       return -1*(start_words - current_words);
    else
       return current_words - start_words;

}


int plain_mon::speed_estimate(int duration) {

    int new_words = words_added();

    if (new_words <= 0 || duration <= 0)
       return 0;
    else 
       return (new_words / duration)*60;
       
}


void plain_mon::resync(std::string doc_name) {

     stream_src.close();
     stream_src.open(doc_name);

     if (stream_src.fail())
        raise(SIGABRT);

}

plain_mon::plain_mon(std::ifstream& file) : word_it(file), stream_src(file)  { 
      
     beginning = file.tellg(); 
     start_words = word_count(); 
     
}

}
