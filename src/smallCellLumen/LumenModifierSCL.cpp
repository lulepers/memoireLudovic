#include "LumenModifierSCL.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "NodeBasedCellPopulation.hpp"
#include <stdlib.h>
#include <math.h>
//#include "ApoptoticCellProperty.hpp"
#include "CellEndo.hpp"
#include "CellLumen.hpp"
#include "SimulationParameters.hpp"

template<unsigned DIM>
LumenModifierSCL<DIM>::LumenModifierSCL()
    : AbstractCellBasedSimulationModifier<DIM>()
{
}

template<unsigned DIM>
LumenModifierSCL<DIM>::~LumenModifierSCL()
{
}

template<unsigned DIM>
void LumenModifierSCL<DIM>::UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void LumenModifierSCL<DIM>::SetupSolve(AbstractCellPopulation<DIM,DIM>& rCellPopulation, std::string outputDirectory)
{
    /*
     * We must update CellData in SetupSolve(), otherwise it will not have been
     * fully initialised by the time we enter the main time loop.
     */
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void LumenModifierSCL<DIM>::UpdateCellData(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    // Make sure the cell population is updated
    rCellPopulation.Update();
    //VertexBasedCellPopulation<DIM>* p_cell_population = static_cast<VertexBasedCellPopulation<DIM>*>(&rCellPopulation);

    /**
     * This hack is needed because in the case of a MeshBasedCellPopulation in which
     * multiple cell divisions have occurred over one time step, the Voronoi tessellation
     * (while existing) is out-of-date. Thus, if we did not regenerate the Voronoi
     * tessellation here, an assertion may trip as we try to access a Voronoi element
     * whose index exceeds the number of elements in the out-of-date tessellation.
     *
     * \todo work out how to properly fix this (#1986)
     */
    if (bool(dynamic_cast<MeshBasedCellPopulation<DIM>*>(&rCellPopulation)))
    {
        static_cast<MeshBasedCellPopulation<DIM>*>(&(rCellPopulation))->CreateVoronoiTessellation();
    }

    if (dynamic_cast<VertexBasedCellPopulation<DIM>*>(&rCellPopulation) == NULL)
    {
        EXCEPTION("Lumen Modifier is to be used with a VertexBasedCellPopulation only");
    }

    // MAKE_PTR(CellBoundary, p_border);

    //double num_cells = 0 ;
    // Iterate over cell population
    //VertexBasedCellPopulation<DIM>* p_cell_population = dynamic_cast<VertexBasedCellPopulation<DIM>*>(&rCellPopulation) ;
    for (typename AbstractCellPopulation<DIM, DIM>::Iterator cell_iter = rCellPopulation.Begin();
         cell_iter != rCellPopulation.End();
         ++cell_iter)
    {



      CellPtr pCell = *cell_iter;

      if (pCell->HasCellProperty<CellLumen>())
      {

        if(pCell->GetCellData()->GetItem("mustDie") == 0)
        {
          pCell->GetCellData()->SetItem("target area", SimulationParameters::SCL_CELLLUMEN_PREFFERED_SIZE);
        }
        else
        {
          //MAKE_PTR(ApoptoticCellProperty, p_apo_prop);
          //pCell->AddCellProperty(p_apo_prop);
          //pCell->GetCellData()->SetItem("target area", Parameters::CELLLUMEN_DIYNG_SIZE);//parfois cela crash avec assert(area !=0)

          //VertexElement<DIM, DIM>* p_element = p_cell_population->GetElementCorrespondingToCell(*cell_iter);
          //unsigned p_index = p_element->GetIndex();
          //double cell_perimeter = p_cell_population->rGetMesh().GetElongationShapeFactorOfElement(p_index);
          //double cote = cell_perimeter / 4;
          pCell->GetCellData()->SetItem("target area", SimulationParameters::SCL_CELLLUMEN_DIYNG_SIZE);
          //double area = rCellPopulation.GetVolumeOfCell(pCell);

          //std::cout << SimulationTime::Instance()->GetTime() << "  < " << pCell->GetCellData()->GetItem("timeOfStartDying")+ Parameters::TIME_BEFORE_KILL << std::endl;

          //if(area < Parameters::SIZE_MIN_FOR_KILL && SimulationTime::Instance()->GetTime() > pCell->GetCellData()->GetItem("timeOfStartDying")+ Parameters::TIME_BEFORE_KILL)
          if(SimulationTime::Instance()->GetTime() > pCell->GetCellData()->GetItem("timeOfStartDying")+ SimulationParameters::SCL_TIME_BEFORE_KILL)
          {
            pCell->Kill();
          }
        }


    }



  }
}



template<unsigned DIM>
void LumenModifierSCL<DIM>::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    // No parameters to output, so just call method on direct parent class
    AbstractCellBasedSimulationModifier<DIM>::OutputSimulationModifierParameters(rParamsFile);
}

// Explicit instantiation
template class LumenModifierSCL<1>;
template class LumenModifierSCL<2>;
template class LumenModifierSCL<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(LumenModifierSCL)
