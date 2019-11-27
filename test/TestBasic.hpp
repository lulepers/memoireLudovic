#ifndef TESTBASIC_HPP_
#define TESTBASIC_HPP_

#include <cxxtest/TestSuite.h>
#include "CheckpointArchiveTypes.hpp"
#include "AbstractCellBasedTestSuite.hpp"

#include "UniformCellCycleModel.hpp"
#include "FixedG1GenerationalCellCycleModel.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "CylindricalHoneycombMeshGenerator.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "CellsGenerator.hpp"
#include "MeshBasedCellPopulationWithGhostNodes.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "NagaiHondaForce.hpp"
#include "SimpleTargetAreaModifier.hpp"
#include "OffLatticeSimulation.hpp"
#include "TransitCellProliferativeType.hpp"
#include "SmartPointers.hpp"
#include "VoronoiDataWriter.hpp"

#include <cxxtest/TestSuite.h>
#include "CellBasedSimulationArchiver.hpp"
#include "SmartPointers.hpp"
#include "AbstractCellBasedTestSuite.hpp"
#include "AdhesionPottsUpdateRule.hpp"
#include "CellsGenerator.hpp"
#include "CylindricalHoneycombMeshGenerator.hpp"
#include "GeneralisedLinearSpringForce.hpp"
#include "HoneycombMeshGenerator.hpp"
#include "HoneycombVertexMeshGenerator.hpp"
#include "MeshBasedCellPopulationWithGhostNodes.hpp"
#include "NagaiHondaForce.hpp"
#include "SimpleTargetAreaModifier.hpp"
#include "NodeBasedCellPopulation.hpp"
#include "OffLatticeSimulation.hpp"
#include "OnLatticeSimulation.hpp"
#include "PlaneBoundaryCondition.hpp"
#include "PottsBasedCellPopulation.hpp"
#include "PottsMeshGenerator.hpp"
#include "RandomCellKiller.hpp"
#include "RepulsionForce.hpp"
#include "UniformG1GenerationalCellCycleModel.hpp"
#include "SurfaceAreaConstraintPottsUpdateRule.hpp"
#include "TysonNovakCellCycleModel.hpp"
#include "VertexBasedCellPopulation.hpp"
#include "VolumeConstraintPottsUpdateRule.hpp"
#include "VoronoiDataWriter.hpp"

#include "FakePetscSetup.hpp"

#include <complex>

#include "CycleEpiDirection.hpp"
#include "CellEpiDirection.hpp"
#include "DirectionModifier.hpp"
#include "LumenGenerationModifier.hpp"
#include "LumenDestructionModifier.hpp"
#include "LumenModifier.hpp"
#include "SimuInfoModifier.hpp"

#include "CellEndo.hpp"

#include "CellPolarityXWriter.hpp"
#include "CellPolarityYWriter.hpp"

//#include "MyCycle.hpp"

#include "CellTypeWriter.hpp"

#include "Parameters.hpp"

class TestBasic : public AbstractCellBasedTestSuite
{
public:

    void TestBasicX()
    {

      //Parameters::init();





      HoneycombVertexMeshGenerator generator(8, 3);
      MutableVertexMesh<2,2>* p_mesh = generator.GetMesh();

      std::vector<CellPtr> cells;
      MAKE_PTR(TransitCellProliferativeType, p_transit_type);
      //MyCycle* cycle = new MyCycle();
      //CellsGenerator<cycle->CreateCellCycleModel(), 2> cells_generator;
      CellsGenerator<CycleEpiDirection, 2> cells_generator;
      cells_generator.GenerateBasicRandom(cells, p_mesh->GetNumElements(), p_transit_type);

      VertexBasedCellPopulation<2> cell_population(*p_mesh, cells);




      //cell_population.AddPopulationWriter<VoronoiDataWriter>();


      OffLatticeSimulation<2> simulator(cell_population);

      simulator.SetOutputDirectory("Testbasique");
      simulator.SetSamplingTimestepMultiple(Parameters::SAMPLING_TIMESTEP);
      simulator.SetDt(Parameters::TIMESTEP);
      //en heure, 24/48

      simulator.SetEndTime(Parameters::TIME_OF_SIMULATION);
      //std::cout << "coucou" << std::endl;


      //Modifier

      MAKE_PTR(NagaiHondaForce<2>, p_force);
      simulator.AddForce(p_force);

      MAKE_PTR(SimpleTargetAreaModifier<2>, p_growth_modifier);
      simulator.AddSimulationModifier(p_growth_modifier);

      MAKE_PTR(DirectionModifier<2>, p_direction_modifier);
      simulator.AddSimulationModifier(p_direction_modifier);

      MAKE_PTR(LumenDestructionModifier<2>, p_lumen_destruction_modifier);
      simulator.AddSimulationModifier(p_lumen_destruction_modifier);

      MAKE_PTR(LumenGenerationModifier<2>, p_lumen_generation_modifier);
      simulator.AddSimulationModifier(p_lumen_generation_modifier);

      MAKE_PTR(LumenModifier<2>, p_lumen_modifier);
      simulator.AddSimulationModifier(p_lumen_modifier);


      MAKE_PTR(SimuInfoModifier<2>, p_info_simu_modifier);
      simulator.AddSimulationModifier(p_info_simu_modifier);

      //Writer
      cell_population.AddCellWriter<CellPolarityXWriter>();
      cell_population.AddCellWriter<CellPolarityYWriter>();
      cell_population.AddCellWriter<CellTypeWriter>();


      MAKE_PTR(CellEpiDirection, p_epiDir);
      MAKE_PTR(CellEndo, p_endo);


      //CycleEpiDirection* p_cell_cycle_model = new CycleEpiDirection();


      int counter = 0;

      for (AbstractCellPopulation<2>::Iterator cell_iter = cell_population.Begin();
           cell_iter != cell_population.End();
           ++cell_iter)
      {

        //On definit le cycle
        //cell_iter->SetCellCycleModel();


        if(counter < 8){
          cell_iter->AddCellProperty(p_endo);
        }
        else{


          cell_iter->GetCellData()->SetItem("cellIndex",Parameters::getNextIndex());
          cell_iter->GetCellData()->SetItem("timeFromLastLumenGeneration",0);
          cell_iter->GetCellData()->SetItem("vecPolaX",0);
          cell_iter->GetCellData()->SetItem("vecPolaY",0);
          cell_iter->AddCellProperty(p_epiDir);
        }
        counter++;

      }

      simulator.Solve();


    }


};


#endif
