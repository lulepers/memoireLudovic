#include "Parameters.hpp"


int Parameters::index_cell = 0;

int Parameters::getNextIndex(){
  index_cell = index_cell + 1;

  return index_cell;
}
