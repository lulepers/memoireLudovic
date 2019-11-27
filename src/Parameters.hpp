#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

class Parameters{
public:
    static constexpr double IMPACT_POLARISATION_EPI_ON_EPI = 0.2;
    static constexpr double IMPACT_POLARISATION_ENDO_ON_EPI = 2;
    static constexpr double IMPACT_POLARISATION_LUMEN_ON_EPI = -1;

    static constexpr double VEC_POLARISATION_DECREASE = 0.8;

    static constexpr double THRESHOLD_POLARISATION_EPI = 3;
    static constexpr double TIME_BEETWEN_TWO_LUMEN_GENERATION = 2;

    static constexpr double TIMESTEP = 0.01;
    static constexpr double SAMPLING_TIMESTEP = 10;
    static constexpr double TIME_OF_SIMULATION = 20;


    static constexpr double AGE_DIV_MIN = 0.05;

    static constexpr double CELLLUMEN_PREFFERED_SIZE = 0.3;
    static constexpr double CELLLUMEN_DIYNG_SIZE = 0.06;
    static constexpr double SIZE_MIN_FOR_KILL = 0.07;
    static constexpr double TIME_BEFORE_KILL = 3;

    static int index_cell;

    static int getNextIndex();

};


#endif

//REMAQUES:-Voir si les vecteurs se calculent bien
//         -La diminution de la polarisation n'est peut être pas juste une soustraction? risque de négatif
//         -Comment savoir proprement si une cellule viens de se diviser (dt marche pas)
//         -vecteur définition de lumen ok?
//         -parfois 1 division donne 2 cellules fille sans lumen?
