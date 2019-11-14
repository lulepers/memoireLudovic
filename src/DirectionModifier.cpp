#include "DirectionModifier.hpp"
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
DirectionModifier<DIM>::DirectionModifier()
    : AbstractCellBasedSimulationModifier<DIM>()
{
}

template<unsigned DIM>
DirectionModifier<DIM>::~DirectionModifier()
{
}

template<unsigned DIM>
void DirectionModifier<DIM>::UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void DirectionModifier<DIM>::SetupSolve(AbstractCellPopulation<DIM,DIM>& rCellPopulation, std::string outputDirectory)
{
    /*
     * We must update CellData in SetupSolve(), otherwise it will not have been
     * fully initialised by the time we enter the main time loop.
     */
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void DirectionModifier<DIM>::UpdateCellData(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
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
        double vecPolaX = pCell->GetCellData()->GetItem("vecPolaX");
        double vecPolaY = pCell->GetCellData()->GetItem("vecPolaY");


        //diminution naturelle
        //if(vecPolaX > 0){
        //  vecPolaX = vecPolaX - Parameters::VEC_POLARISATION_DECREASE * Parameters::TIMESTEP;
        //}
        //else if(vecPolaX < 0){
        //  vecPolaX = vecPolaX+ Parameters::VEC_POLARISATION_DECREASE * Parameters::TIMESTEP;
        //}

        //if(vecPolaY > 0){
        //  vecPolaY = vecPolaY - Parameters::VEC_POLARISATION_DECREASE * Parameters::TIMESTEP;
        //}
        //else if(vecPolaY < 0){
        //  vecPolaY = vecPolaY + Parameters::VEC_POLARISATION_DECREASE * Parameters::TIMESTEP;
        //}

        vecPolaX = vecPolaX - vecPolaX * Parameters::VEC_POLARISATION_DECREASE * Parameters::TIMESTEP;
        vecPolaY = vecPolaY - vecPolaY * Parameters::VEC_POLARISATION_DECREASE * Parameters::TIMESTEP;




        //impact des voisins

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


                //Cell endothéliale
                bool neighbour_is_endo = p_neighbour_cell->template HasCellProperty<CellEndo>();

                //Cell Lumen
                bool neighbour_is_lumen = p_neighbour_cell->template HasCellProperty<CellLumen>();




                if ( neighbour_is_epiDir == 1)
                {
                  //std::cout << "coucou" << std::endl;

                  double vecPolaXNeighbour = p_neighbour_cell->GetCellData()->GetItem("vecPolaX");
                  double vecPolaYNeighbour = p_neighbour_cell->GetCellData()->GetItem("vecPolaY");


                  if(vecPolaXNeighbour > 0){
                    vecPolaX = vecPolaX + Parameters::IMPACT_POLARISATION_EPI_ON_EPI * Parameters::TIMESTEP;
                  }
                  else if(vecPolaXNeighbour < 0){
                    vecPolaX = vecPolaX - Parameters::IMPACT_POLARISATION_EPI_ON_EPI * Parameters::TIMESTEP;
                  }

                  if(vecPolaYNeighbour > 0){
                    vecPolaY = vecPolaY + Parameters::IMPACT_POLARISATION_EPI_ON_EPI * Parameters::TIMESTEP;
                  }
                  else if(vecPolaYNeighbour < 0){
                    vecPolaY = vecPolaY - Parameters::IMPACT_POLARISATION_EPI_ON_EPI * Parameters::TIMESTEP;
                  }
                }

                if(neighbour_is_endo == 1){

                  c_vector<double, DIM> neighbour_location = p_cell_population->GetLocationOfCellCentre(p_neighbour_cell);
                  c_vector<double, DIM> cell_location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);

                  double dx = cell_location[0] - neighbour_location[0];
                  double dy = cell_location[1] - neighbour_location[1];

                  double normalisation = dx + dy;

                  vecPolaX = vecPolaX + dx / normalisation * Parameters::IMPACT_POLARISATION_ENDO_ON_EPI * Parameters::TIMESTEP;
                  vecPolaY = vecPolaY + dy / normalisation * Parameters::IMPACT_POLARISATION_ENDO_ON_EPI * Parameters::TIMESTEP;

                  //std::cout << vecPolaX << std::endl;

                }


                if(neighbour_is_lumen == 1){

                  c_vector<double, DIM> neighbour_location = p_cell_population->GetLocationOfCellCentre(p_neighbour_cell);
                  c_vector<double, DIM> cell_location = rCellPopulation.GetLocationOfCellCentre(*cell_iter);

                  double dx = cell_location[0] - neighbour_location[0];
                  double dy = cell_location[1] - neighbour_location[1];

                  double normalisation = dx + dy;

                  vecPolaX = vecPolaX + dx / normalisation * Parameters::IMPACT_POLARISATION_LUMEN_ON_EPI * Parameters::TIMESTEP;
                  vecPolaY = vecPolaY + dy / normalisation * Parameters::IMPACT_POLARISATION_LUMEN_ON_EPI * Parameters::TIMESTEP;

                  //std::cout << vecPolaX << std::endl;

                }

            }

        }
        pCell->GetCellData()->SetItem("vecPolaX",vecPolaX);
        pCell->GetCellData()->SetItem("vecPolaY",vecPolaY);
        //std::cout << vecPolaX << std::endl;
        //std::cout << vecPolaY << std::endl;
      }


    }

}



template<unsigned DIM>
void DirectionModifier<DIM>::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    // No parameters to output, so just call method on direct parent class
    AbstractCellBasedSimulationModifier<DIM>::OutputSimulationModifierParameters(rParamsFile);
}

// Explicit instantiation
template class DirectionModifier<1>;
template class DirectionModifier<2>;
template class DirectionModifier<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(DirectionModifier)
