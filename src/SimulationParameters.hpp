#ifndef SIMULATIONPARAMETERS_HPP_
#define SIMULATIONPARAMETERS_HPP_

class SimulationParameters{
public:

  //
  static constexpr double TIMESTEP = 0.01;
  static constexpr double SAMPLING_TIMESTEP = 10;
  static constexpr double TIME_OF_SIMULATION = 20;

  //Commun aux simulations

  static constexpr double IMPACT_POLARISATION_EPI_ON_EPI = 0.2;
  static constexpr double IMPACT_POLARISATION_ENDO_ON_EPI = 2;
  static constexpr double IMPACT_POLARISATION_LUMEN_ON_EPI = -1;

  static constexpr double VEC_POLARISATION_DECREASE = 0.8;

  static constexpr double THRESHOLD_POLARISATION_EPI = 3;
  static constexpr double AGE_DIV_MIN = 0.05;

  //BCL
  static constexpr double BCL_TIME_BEETWEN_TWO_LUMEN_GENERATION = 0;
  static constexpr double BCL_LUMEN_SIZE_FACTOR = 0.125;
  static constexpr double BCL_NBR_CELL_BETWEEN_TWO_LUMEN = 3;
  static constexpr double BCL_AGE_TO_LUMEN_MATURITY = 15;

  //SCL

  static constexpr double SCL_TIME_BEETWEN_TWO_LUMEN_GENERATION = 2;



  static constexpr double SCL_CELLLUMEN_PREFFERED_SIZE = 0.3;
  static constexpr double SCL_CELLLUMEN_DIYNG_SIZE = 0.06;
  static constexpr double SCL_SIZE_MIN_FOR_KILL = 0.07;
  static constexpr double SCL_TIME_BEFORE_KILL = 3;



  //Function

  static int index_cell;

  static int getNextIndex();


};


#endif
