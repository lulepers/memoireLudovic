#include "Parameters.hpp"


int Parameters::index_cell_epi = 0;

int Parameters::getNextIndex(){
  index_cell_epi = index_cell_epi + 1;

  return index_cell_epi;
}
