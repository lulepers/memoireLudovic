#include "LumenGenerationModifier.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "NodeBasedCellPopulation.hpp"
#include <stdlib.h>
#include <math.h>
#include "CellEpiDirection.hpp"
#include "CellEndo.hpp"
#include "CellLumen.hpp"
#include "Parameters.hpp"

template<unsigned DIM>
LumenGenerationModifier<DIM>::LumenGenerationModifier()
    : AbstractCellBasedSimulationModifier<DIM>()
{
}

template<unsigned DIM>
LumenGenerationModifier<DIM>::~LumenGenerationModifier()
{
}

template<unsigned DIM>
void LumenGenerationModifier<DIM>::UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void LumenGenerationModifier<DIM>::SetupSolve(AbstractCellPopulation<DIM,DIM>& rCellPopulation, std::string outputDirectory)
{
    /*
     * We must update CellData in SetupSolve(), otherwise it will not have been
     * fully initialised by the time we enter the main time loop.
     */
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void LumenGenerationModifier<DIM>::UpdateCellData(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    // Make sure the cell population is updated
    rCellPopulation.Update();
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
        EXCEPTION("Direction Modifier is to be used with a VertexBasedCellPopulation only");
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
      //IF Epithéliale
      if (pCell->HasCellProperty<CellEpiDirection>())
      {
        double timeFromLastGen = pCell->GetCellData()->GetItem("timeFromLastLumenGeneration");

        double xl = pCell->GetCellData()->GetItem("vecPolaX");
        double yl = pCell->GetCellData()->GetItem("vecPolaY");

        double norme = sqrt(xl*xl + yl*yl);
        //pCell->GetCellData()->SetItem("timeFromLastLumenGeneration", 0);
      if(norme >Parameters::THRESHOLD_POLARISATION_EPI)
      {
        pCell->GetCellData()->SetItem("timeFromLastLumenGeneration", timeFromLastGen + Parameters::TIMESTEP);
      }
      else
      {
        //Si le vecteur est en dessous du trheshold on le mets à 0
        pCell->GetCellData()->SetItem("timeFromLastLumenGeneration", 0);
      }
      //Si la cellule viens de se diviser et la simulation a durée assez longtemps
      if(pCell->GetAge()<Parameters::AGE_DIV_MIN && SimulationTime::Instance()->GetTime() > 2 * Parameters::AGE_DIV_MIN)
      {


        //on cherche la cellule fille
        VertexBasedCellPopulation<DIM>* p_cell_population = dynamic_cast<VertexBasedCellPopulation<DIM>*>(&rCellPopulation) ;
        std::set<unsigned> neighbour_indices = p_cell_population->GetNeighbouringLocationIndices(*cell_iter);

        // If this cell has any neighbours (as defined by mesh/population/interaction distance)...
        if (!neighbour_indices.empty() )
        {

            for (std::set<unsigned>::iterator neighbour_iter = neighbour_indices.begin();
                 neighbour_iter != neighbour_indices.end();
                 ++neighbour_iter)
            {
              CellPtr p_neighbour_cell = p_cell_population->GetCellUsingLocationIndex(*neighbour_iter);

              //Cell Epithéliale
              bool neighbour_is_epiDir = p_neighbour_cell->template HasCellProperty<CellEpiDirection>();

              if ( neighbour_is_epiDir == 1)
              {
                if(pCell->GetCellData()->GetItem("cellIndex")==p_neighbour_cell->GetCellData()->GetItem("cellIndex"))
                {


                  //on regarde si elle est pointée par le vecteur

                  c_vector<double, DIM> neighbour_location = p_cell_population->GetLocationOfCellCentre(p_neighbour_cell);
                  c_vector<double, DIM> cell_location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);

                  double dx = neighbour_location[0] - cell_location[0];
                  double dy = neighbour_location[1] - cell_location[1];

                  double deltaXMere = dx - pCell->GetCellData()->GetItem("vecPolaX");
                  double deltaYMere = dy - pCell->GetCellData()->GetItem("vecPolaY");

                  double deltaXFille = -dx - pCell->GetCellData()->GetItem("vecPolaX");
                  double deltaYFille = -dy - pCell->GetCellData()->GetItem("vecPolaY");


                  //regarde si ce vecteur est plus proche de celui polarisé que l'autre (pas de racine, inutile)
                  double VecMere = deltaXMere * deltaXMere + deltaYMere * deltaYMere;
                  double VecFille = deltaXFille * deltaXFille + deltaYFille * deltaYFille;

                  if(VecMere < VecFille){
                    p_neighbour_cell->RemoveCellProperty<CellEpiDirection>();
                    MAKE_PTR(CellLumen, p_lumen);
                    p_neighbour_cell->AddCellProperty(p_lumen);
                    p_neighbour_cell->GetCellData()->SetItem("mustDie", 0);
                  }

                  else{
                    pCell->RemoveCellProperty<CellEpiDirection>();
                    MAKE_PTR(CellLumen, p_lumen);
                    pCell->AddCellProperty(p_lumen);
                    pCell->GetCellData()->SetItem("mustDie", 0);
                  }

                  pCell->GetCellData()->SetItem("cellIndex",Parameters::getNextIndex());

                  pCell->GetCellData()->SetItem("timeFromLastLumenGeneration", 0);
                  pCell->GetCellData()->SetItem("vecPolaX", 0);
                  pCell->GetCellData()->SetItem("vecPolaY", 0);

                  p_neighbour_cell->GetCellData()->SetItem("cellIndex",Parameters::getNextIndex());

                  p_neighbour_cell->GetCellData()->SetItem("timeFromLastLumenGeneration", 0);
                  p_neighbour_cell->GetCellData()->SetItem("vecPolaX", 0);
                  p_neighbour_cell->GetCellData()->SetItem("vecPolaY", 0);



                }

              }



            }

          }
      }
    }
  }
}



template<unsigned DIM>
void LumenGenerationModifier<DIM>::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    // No parameters to output, so just call method on direct parent class
    AbstractCellBasedSimulationModifier<DIM>::OutputSimulationModifierParameters(rParamsFile);
}

// Explicit instantiation
template class LumenGenerationModifier<1>;
template class LumenGenerationModifier<2>;
template class LumenGenerationModifier<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(LumenGenerationModifier)
