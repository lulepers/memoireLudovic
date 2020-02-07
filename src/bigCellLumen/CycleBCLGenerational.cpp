#include "CycleBCLGenerational.hpp"
#include "StemCellProliferativeType.hpp"
#include "TransitCellProliferativeType.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "AbstractCellCycleModel.hpp"
#include "CellEpi.hpp"
#include "SimulationParameters.hpp"
#include <math.h>

CycleBCLGenerational::CycleBCLGenerational()
: mCycleDuration(11)
{
}

CycleBCLGenerational::CycleBCLGenerational(const CycleBCLGenerational& rModel)
   : AbstractSimpleGenerationalCellCycleModel(rModel),
     mCycleDuration(rModel.mCycleDuration)
{
    /*
     * The member variables mGeneration and mMaxTransitGeneration are
     * initialized in the AbstractSimpleGenerationalCellCycleModel
     * constructor.
     *
     * The member variables mCurrentCellCyclePhase, mG1Duration,
     * mMinimumGapDuration, mStemCellG1Duration, mTransitCellG1Duration,
     * mSDuration, mG2Duration and mMDuration are initialized in the
     * AbstractPhaseBasedCellCycleModel constructor.
     *
     * The member variables mBirthTime, mReadyToDivide and mDimension
     * are initialized in the AbstractCellCycleModel constructor.
     *
     * Note that mG1Duration is (re)set as soon as InitialiseDaughterCell()
     * is called on the new cell-cycle model.
     */
}


bool CycleBCLGenerational::ReadyToDivide()
{
   assert(mpCell != NULL);
   //si Normal
   if (!mReadyToDivide)
   {
     if (mpCell->HasCellProperty<CellEpi>())
     {
       UpdateCellCyclePhase();
       if ( (GetCurrentCellCyclePhase() != G_ZERO_PHASE) &&
            (GetAge() >=  GetMDuration() +  GetG1Duration() +  GetSDuration() + GetG2Duration()) )
       {
           mReadyToDivide = true;
       }
     }
   }


   //Si pour lumen
   if (!mReadyToDivide)
   {
     if (mpCell->HasCellProperty<CellEpi>())
     {
       double timeFromLastGen = mpCell->GetCellData()->GetItem("timeFromLastLumenGeneration");

       double xl = mpCell->GetCellData()->GetItem("vecPolaX");
       double yl = mpCell->GetCellData()->GetItem("vecPolaY");

       double norme = sqrt(xl*xl + yl*yl);
       if(norme >SimulationParameters::THRESHOLD_POLARISATION_EPI)
       {

         if(timeFromLastGen > SimulationParameters::BCL_TIME_BEETWEN_TWO_LUMEN_GENERATION)
         {
           if (GetAge() > SimulationParameters::AGE_DIV_LUMEN_MIN)
           {
             if(mpCell->GetCellData()->GetItem("lumenNearby") == 0)
             {
               //on ajoute un tag pour savoir que c'est pas une vraie division
               mpCell->GetCellData()->SetItem("hadALumenDivision",1);

               mReadyToDivide = true;
             }

           }
         }
       }
     }
   }


   return mReadyToDivide;
}


AbstractCellCycleModel* CycleBCLGenerational::CreateCellCycleModel()
{
    return new CycleBCLGenerational(*this);
}

void CycleBCLGenerational::SetCycleDuration(double cycleduration)
{
    mCycleDuration = cycleduration;
}

double CycleBCLGenerational::GetCycleDuration() const
{
    return mCycleDuration;
}


void CycleBCLGenerational::SetG1Duration()
{

    //PARTIE CONCERNANT LA VERITALBE DIVISION CELLULAIRE
    //RandomNumberGenerator* p_gen = RandomNumberGenerator::Instance();
    double a_thyr = 1.0035 ;
    double b_thyr = -0.747 ;
    double simulation_time = 48.0 ;
    double x_thyr = SimulationTime::Instance()->GetTime()/simulation_time ;

    assert(mpCell != nullptr);


        // double CoreCycleDuration = 32;
        mG1Duration =  13/(a_thyr*exp(b_thyr*x_thyr)) ; // ICI : MODIFIER DUREE PHASE G1 POUR CELLULES DE COEUR, tu peux remplacer GetTransitCellG1Duration() par un nombre
        mSDuration =  13/(a_thyr*exp(b_thyr*x_thyr)) ;
        mG2Duration = 4/(a_thyr*exp(b_thyr*x_thyr)) ;
        mMDuration =  2/(a_thyr*exp(b_thyr*x_thyr)) ;
        mMaxTransitGenerations  = 6 ;

      //!!!!!!!!!!!!! LES CELLULES NE SONT PAS EN PROLIFERATIVE!!!!!!!!!!!!!!!!!!!!!!!!!
    //if (mpCell->GetCellProliferativeType()->IsType<StemCellProliferativeType>())
    //{
    //    mG1Duration = GetStemCellG1Duration() + 4*p_gen->ranf(); // U[14,18] for default parameters (mStemCellG1Duration) according to Meineke
    //}
    //else if (mpCell->GetCellProliferativeType()->IsType<TransitCellProliferativeType>())
    //{
      //std::cout << "test" << '\n';
    //  if (mpCell->HasCellProperty<CellEpi>())
    //  {
          // double CoreCycleDuration = 32;
    //      mG1Duration =  13/(a_thyr*exp(b_thyr*x_thyr)) ; // ICI : MODIFIER DUREE PHASE G1 POUR CELLULES DE COEUR, tu peux remplacer GetTransitCellG1Duration() par un nombre
    //      mSDuration =  13/(a_thyr*exp(b_thyr*x_thyr)) ;
    //      mG2Duration = 4/(a_thyr*exp(b_thyr*x_thyr)) ;
    //      mMDuration =  2/(a_thyr*exp(b_thyr*x_thyr)) ;
    //      mMaxTransitGenerations  = 6 ;
    //    }

    //}
    //else if (mpCell->GetCellProliferativeType()->IsType<DifferentiatedCellProliferativeType>())
    //{
    //    mG1Duration = DBL_MAX;
    //}
    //else
    //{
    //    NEVER_REACHED;
    //}


}


void CycleBCLGenerational::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
    // No new parameters to output, so just call method on direct parent class
    AbstractSimpleGenerationalCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(CycleBCLGenerational)
