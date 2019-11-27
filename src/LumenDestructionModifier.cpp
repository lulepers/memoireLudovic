#include "LumenDestructionModifier.hpp"
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
LumenDestructionModifier<DIM>::LumenDestructionModifier()
    : AbstractCellBasedSimulationModifier<DIM>()
{
}

template<unsigned DIM>
LumenDestructionModifier<DIM>::~LumenDestructionModifier()
{
}

template<unsigned DIM>
void LumenDestructionModifier<DIM>::UpdateAtEndOfTimeStep(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
{
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void LumenDestructionModifier<DIM>::SetupSolve(AbstractCellPopulation<DIM,DIM>& rCellPopulation, std::string outputDirectory)
{
    /*
     * We must update CellData in SetupSolve(), otherwise it will not have been
     * fully initialised by the time we enter the main time loop.
     */
    UpdateCellData(rCellPopulation);
}

template<unsigned DIM>
void LumenDestructionModifier<DIM>::UpdateCellData(AbstractCellPopulation<DIM,DIM>& rCellPopulation)
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
        EXCEPTION("LumenDestructionModifier is to be used with a VertexBasedCellPopulation only");
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


      //if lumen
      if (pCell->HasCellProperty<CellLumen>())
      {


        //La "cellule" ne devrai pas exister
        bool notStable = false;




        //cellule voisine endo

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

                //Cell endothéliale
                bool neighbour_is_endo = p_neighbour_cell->template HasCellProperty<CellEndo>();

                if ( neighbour_is_endo == 1)
                {
                  notStable = true;
                }

            }

        }


        //touche la matrice extra cellulaire
        double n_boundary_nodes = 0 ;
        VertexElement<DIM,DIM>* p_element = p_cell_population->GetElementCorrespondingToCell(*cell_iter);
        unsigned num_nodes_in_element = p_element->GetNumNodes();
        for (unsigned local_index=0; local_index<num_nodes_in_element; local_index++)
        {
          unsigned node_index = p_element->GetNodeGlobalIndex(local_index);

          if (rCellPopulation.GetNode(node_index)->IsBoundaryNode())
          {

              n_boundary_nodes++ ;

          }
      }

      if (n_boundary_nodes > 0)
      {
        notStable = true;
      }




      //Tuer la lumière
      if(notStable == 1)
      {

        if(pCell->GetCellData()->GetItem("mustDie") == 0)
        {
          pCell->GetCellData()->SetItem("timeOfStartDying", SimulationTime::Instance()->GetTime());
        }
        pCell->GetCellData()->SetItem("mustDie", 1);


      }
      else{
        pCell->GetCellData()->SetItem("mustDie", 0);
      }



      }


    }

}



template<unsigned DIM>
void LumenDestructionModifier<DIM>::OutputSimulationModifierParameters(out_stream& rParamsFile)
{
    // No parameters to output, so just call method on direct parent class
    AbstractCellBasedSimulationModifier<DIM>::OutputSimulationModifierParameters(rParamsFile);
}

// Explicit instantiation
template class LumenDestructionModifier<1>;
template class LumenDestructionModifier<2>;
template class LumenDestructionModifier<3>;

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
EXPORT_TEMPLATE_CLASS_SAME_DIMS(LumenDestructionModifier)
