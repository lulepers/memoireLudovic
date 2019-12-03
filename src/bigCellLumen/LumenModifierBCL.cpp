#include "LumenModifierBCL.hpp"
#include "MeshBasedCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "NodeBasedCellPopulation.hpp"
#include <stdlib.h>
#include <math.h>
//#include "ApoptoticCellProperty.hpp"
#include "CellEpi.hpp"
#include "CellLumen.hpp"
#include "SimulationParameters.hpp"

template<unsigned DIM>
LumenModifierBCL<DIM>::LumenModifierBCL()
    : AbstractCellBasedSimulationModifier<DIM>()
{
}

template<unsigned DIM>
LumenModifierBCL<DIM>::~LumenModifierBCL()
{
}

template<unsigned DIM>
void LumenModifierBCL<DIM>::UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void LumenModifierBCL<DIM>::SetupSolve(AbstractCellPopulation<DIM,DIM>& rCellPopulation, std::string outputDirectory)
{
    /*
     * We must update CellData in SetupSolve(), otherwise it will not have been
     * fully initialised by the time we enter the main time loop.
     */
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void LumenModifierBCL<DIM>::UpdateCellData(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
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
    double targetSize = 0;

    for (typename AbstractCellPopulation<DIM, DIM>::Iterator cell_iter = rCellPopulation.Begin();
         cell_iter != rCellPopulation.End();
         ++cell_iter)
    {



      CellPtr pCell = *cell_iter;

      if (pCell->HasCellProperty<CellLumen>())
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

              CellPtr pnCell = p_cell_population->GetCellUsingLocationIndex(*neighbour_iter);

              bool neighbour_is_epi = pnCell->template HasCellProperty<CellEpi>();
              if(neighbour_is_epi)
              {

                c_vector<double, DIM> neighbour_location = p_cell_population->GetLocationOfCellCentre(pnCell);
                c_vector<double, DIM> cell_location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);

                double dx = neighbour_location[0] - cell_location[0];
                double dy = neighbour_location[1] - cell_location[1];

                double deltaX1 = dx - pnCell->GetCellData()->GetItem("vecPolaX");
                double deltaY1 = dy - pnCell->GetCellData()->GetItem("vecPolaY");

                double deltaX2 = -dx - pnCell->GetCellData()->GetItem("vecPolaX");
                double deltaY2 = -dy - pnCell->GetCellData()->GetItem("vecPolaY");


                //regarde si ce vecteur est plus proche de celui polarisé que l'autre (pas de racine, inutile)
                double Vec1 = deltaX1 * deltaX1 + deltaY1 * deltaY1;
                double Vec2 = deltaX2 * deltaX2 + deltaY2 * deltaY2;

                if(Vec1 > Vec2){

                  double xl = pnCell->GetCellData()->GetItem("vecPolaX");
                  double yl = pnCell->GetCellData()->GetItem("vecPolaY");

                  double norme = sqrt(xl*xl + yl*yl);

                  targetSize = targetSize + norme * SimulationParameters::BCL_LUMEN_SIZE_FACTOR;
                }

              }


            }

          }
          pCell->GetCellData()->SetItem("target area",targetSize);
    }




  }


}



template<unsigned DIM>
void LumenModifierBCL<DIM>::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    // No parameters to output, so just call method on direct parent class
    AbstractCellBasedSimulationModifier<DIM>::OutputSimulationModifierParameters(rParamsFile);
}

// Explicit instantiation
template class LumenModifierBCL<1>;
template class LumenModifierBCL<2>;
template class LumenModifierBCL<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(LumenModifierBCL)
