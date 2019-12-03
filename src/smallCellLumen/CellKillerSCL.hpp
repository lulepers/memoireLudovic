#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "AbstractCellKiller.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "OffLatticeSimulation.hpp"
#include "CellsGenerator.hpp"
#include "SmartPointers.hpp"
//This test is always run sequentially (never in parallel)
#include "FakePetscSetup.hpp"

#include "CellLumen.hpp"

class CellKillerSCL : public AbstractCellKiller<2>
{
private:

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellKiller<2> >(*this);
    }

public:

    CellKillerSCL(AbstractCellPopulation<2>* pCellPopulation)
        : AbstractCellKiller<2>(pCellPopulation)
    {}

    void CheckAndLabelCellsForApoptosisOrDeath()
    {
        for (AbstractCellPopulation<2>::Iterator cell_iter = this->mpCellPopulation->Begin();
            cell_iter != this->mpCellPopulation->End();
            ++cell_iter)
        {
          CellPtr pCell = *cell_iter;

          if (pCell->HasCellProperty<CellLumen>())
          {
            //std::cout << pCell->GetApoptosisTime() << std::endl;
            if(pCell->GetCellData()->GetItem("mustDie") == 1)
            {
              pCell->StartApoptosis();
            }
            //else
            //{
              //pCell->SetApoptosisTime(1);
              //pCell->StartApoptosis(0);
              //std::cout << pCell->GetCellData()->GetItem("mustDie") << std::endl;
              //pCell->GetCellData()->SetItem("target area", Parameters::CELLLUMEN_DIYNG_SIZE);//parfois cela crash avec assert(area !=0)

              //VertexElement<DIM, DIM>* p_element = p_cell_population->GetElementCorrespondingToCell(*cell_iter);
              //unsigned p_index = p_element->GetIndex();
              //double cell_perimeter = p_cell_population->rGetMesh().GetElongationShapeFactorOfElement(p_index);
              //double cote = cell_perimeter / 4;
              //double area = cote * cote;

              //if(area < Parameters::SIZE_MIN_FOR_KILL)
              //{
              //  pCell->Kill();
              //}
            //}

        }
            //cell_iter->Kill();

        }
    }

    void OutputCellKillerParameters(out_stream& rParamsFile)
    {
        AbstractCellKiller<2>::OutputCellKillerParameters(rParamsFile);
    }
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(CellKillerSCL)
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(CellKillerSCL)

namespace boost
{
    namespace serialization
    {
        template<class Archive>
        inline void save_construct_data(
            Archive & ar, const CellKillerSCL * t, const unsigned int file_version)
        {
            const AbstractCellPopulation<2>* const p_cell_population = t->GetCellPopulation();
            ar << p_cell_population;
        }

        template<class Archive>
        inline void load_construct_data(
            Archive & ar, CellKillerSCL * t, const unsigned int file_version)
        {
            AbstractCellPopulation<2>* p_cell_population;
            ar >> p_cell_population;

            // Invoke inplace constructor to initialise instance
            ::new(t)CellKillerSCL(p_cell_population);
        }
    }
}
