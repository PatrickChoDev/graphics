#ifndef __PROGRESS_HPP__
#define __PROGRESS_HPP__

#include <iostream>

class progress_bar {
private:
  int total;
  int current;
  int update_interval;
    
public:
  progress_bar(int total_items) : total(total_items), current(0) {
    update_interval = total / 100;
    if(update_interval < 1) {
      update_interval = 1;
    }
  }
  
  void update() {
    current++;
    if(current % update_interval == 0 || current == total) {
      int progress = (current * 100) / total;
      char spinner[] = {'|', '/', '-', '\\'};
      char spin_char = spinner[(current / update_interval) % 4];
      std::cout << "\rRendering " << spin_char << " " << progress << "% (" << current << "/" << total << " pixels) " << std::flush;
    }
  }
  
  void finish() {
    std::cout << "\rRendering - 100% (" << total << "/" << total << " pixels)" << std::endl;
  }
};

#endif