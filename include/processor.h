#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  float MemUsage();
  // TODO: Declare any necessary private members
 private:
    std::vector<long> prev_;
};

#endif