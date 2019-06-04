#include "tracker.h"
#include <csignal>
#include <cstdlib>

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

    std::ifstream doc_stream(doc);

    if (doc_stream.fail())
       std::raise(SIGABRT);

    std::istream_iterator<std::string> word_it(doc_stream), end;
    int word_total = std::distance(word_it, end);
    doc_stream.close();
    return word_total;

}


int plain_mon::words_added() {

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


plain_mon::plain_mon(const std::string& doc_name) : doc(doc_name) { 

     start_words = word_count(); 
     
}


odf_mon::odf_mon(const std::string& doc_name) : wcount_reg("meta:word-count=\"([[:digit:]]+)\"") {

     archive = zip_open(doc_name.c_str(), ZIP_RDONLY, NULL);

     if (!archive)
        throw "Error while opening ODF archive";

     if (zip_name_locate(archive, "meta.xml", 0) == -1)
        throw "No meta.xml in ODF archive";

     zip_stat(archive, "meta.xml", 0 , &meta_fparams);
     old_atime = meta_fparams.mtime;
     metafile = zip_fopen(archive, "meta.xml", 0);

     if (!metafile)
        throw "Error while opening meta.xml";

     extract_wcount_xml();

     if (wcount_m.empty() || wcount_m.size() < 2)
        throw "No word count in meta.xml";

     std::istringstream(wcount_m.str(1)) >> start_words; 

}


odf_mon::~odf_mon() {

     zip_close(archive);

     if (metafile)
        zip_fclose(metafile);

}


void odf_mon::extract_wcount_xml() {

     meta_content.resize(meta_fparams.size);
     zip_fread(metafile, meta_content.data(), meta_fparams.size);
     std::string xml_seq(meta_content.data(), meta_content.size()); 
          
     std::regex_search(xml_seq, wcount_m, wcount_reg);

}


int odf_mon::word_count() {

     zip_stat(archive, "meta.xml", 0 , &meta_fparams);

     if (meta_fparams.mtime == old_atime)
        return start_words;

     extract_wcount_xml();          
     
     if (wcount_m.empty() || wcount_m.size() < 2)
        std::raise(SIGABRT);

     return std::atoi(wcount_m.str(1).c_str());

}


}
