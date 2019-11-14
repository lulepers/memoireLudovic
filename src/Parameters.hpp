class Parameters{
public:
    static constexpr double IMPACT_POLARISATION_EPI_ON_EPI = 0.0;
    static constexpr double IMPACT_POLARISATION_ENDO_ON_EPI = 3;
    static constexpr double IMPACT_POLARISATION_LUMEN_ON_EPI = -1;

    static constexpr double VEC_POLARISATION_DECREASE = 0.5;

    static constexpr double THRESHOLD_POLARISATION_EPI = 3;
    static constexpr double TIME_BEETWEN_TWO_LUMEN_GENERATION = 2;

    static constexpr double TIMESTEP = 0.003;
    static constexpr double SAMPLING_TIMESTEP = 10;
    static constexpr double TIME_OF_SIMULATION = 15;


    static constexpr double AGE_DIV_MIN = 0.004;

    static constexpr double CELLLUMEN_PREFFERED_SIZE = 0.3;

};


//REMAQUES:-Voir si les vecteurs se calculent bien
//         -La diminution de la polarisation n'est peut être pas juste une soustraction? risque de négatif
//         -Comment savoir proprement si une cellule viens de se diviser
//         -vecteur définition de lumen ok?
//
