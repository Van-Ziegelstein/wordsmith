#include "tracker.h"
#include <fstream>
#include <iterator>
#include <csignal>
#include <zip.h>
#include <algorithm>

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


bool time_frags::is_finished() {

    if (total_minutes >= max_time)
       return true;
    else
       return false;

}


int plain_mon::word_filter(std::istream_iterator<std::string>& first_w, std::istream_iterator<std::string>& last_w) {
    return std::distance(first_w, last_w);
}


int plain_mon::word_count() {

    std::ifstream doc_stream(doc);

    if (doc_stream.fail())
       std::raise(SIGABRT);

    std::istream_iterator<std::string> word_it(doc_stream), end;
    int word_total = word_filter(word_it, end);

    if (start_words == -1)
       start_words = word_total;

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


plain_mon::plain_mon(const std::string& doc_name) : doc(doc_name), start_words(-1) { }

plain_mon::~plain_mon() { }


odf_mon::odf_mon(const std::string& doc_name) : plain_mon(doc_name), wcount_reg("meta:word-count=\"([[:digit:]]+)\""), old_atime(0) { }

odf_mon::odf_mon(const std::string& doc_name, const char *wmeta_reg) : plain_mon(doc_name), wcount_reg(wmeta_reg), old_atime(0) { }

int odf_mon::get_wordmeta(const char *metafile) {

    int wcount;
    std::smatch wcount_m;
    zip_stat_t meta_fparams;
    zip_t *archive = zip_open(doc.c_str(), ZIP_RDONLY, NULL);
     
    if (!archive)
       raise(SIGABRT);

    if (zip_name_locate(archive, metafile, 0) == -1)
       raise(SIGABRT);

    zip_stat(archive, metafile, 0 , &meta_fparams);

    if (old_atime == 0)
       old_atime = meta_fparams.mtime;

    if (meta_fparams.mtime == old_atime && start_words != -1)
       return start_words;
     
    zip_file_t *meta = zip_fopen(archive, metafile, 0);

    if (!meta)
       raise(SIGABRT);

    meta_content.resize(meta_fparams.size);
    zip_fread(meta, meta_content.data(), meta_fparams.size);
    std::string xml_seq(meta_content.data(), meta_content.size()); 
          
    std::regex_search(xml_seq, wcount_m, wcount_reg);

    if (wcount_m.empty() || wcount_m.size() < 2)
       std::raise(SIGABRT);

    std::istringstream(wcount_m.str(1)) >> wcount;
    zip_fclose(meta);
    zip_close(archive);

    if (start_words == -1)
       start_words = wcount;

    return wcount;
}


int odf_mon::word_count() {
    return get_wordmeta("meta.xml");
}


docx_mon::docx_mon(const std::string& doc_name) : odf_mon(doc_name, "<Words>([[:digit:]]+)<") { }

int docx_mon::word_count() {
    return get_wordmeta("docProps/app.xml");
}


tex_mon::tex_mon(const std::string& doc_name) : plain_mon(doc_name) { }


int tex_mon::word_filter(std::istream_iterator<std::string>& first_w, std::istream_iterator<std::string>& last_w) {

    int word_num = 0;
    bool meat_start = false, meat_end = false;

    std::for_each(first_w, last_w, [&] (const std::string word) {

          if (word.compare("\\begin{document}") == 0)
              meat_start = true;

          if (word.compare("\\end{document}") == 0)
              meat_end = true;
        
          if (meat_start && !meat_end &&
              !std::regex_match(word, std::regex(R"(\\[[:alpha:]]+\{?.*|^%.*)"))) {
       
             word_num++;

          }

    });

    return word_num;
}


}
