#ifndef TESTSMALLCELLLUMEN_HPP_
#define TESTSMALLCELLLUMEN_HPP_

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

#include "CycleEpiDirectionSCL.hpp"
#include "CellEpi.hpp"
#include "PolarisationModifier.hpp"
#include "LumenGenerationModifierSCL.hpp"
#include "LumenDestructionModifierSCL.hpp"
#include "LumenModifierSCL.hpp"
#include "SimuInfoModifier.hpp"

#include "CellEndo.hpp"

#include "CellPolarityXWriter.hpp"
#include "CellPolarityYWriter.hpp"

#include "CellKillerSCL.hpp"

//#include "MyCycle.hpp"

#include "CellTypeWriter.hpp"

#include "SimulationParameters.hpp"
#include "SimulationParameters.hpp"

class TestSmallCellLumen : public AbstractCellBasedTestSuite
{
public:

    void TestSmallCellLumenX()
    {

      //Parameters::init();





      HoneycombVertexMeshGenerator generator(6, 3);
      MutableVertexMesh<2,2>* p_mesh = generator.GetMesh();

      std::vector<CellPtr> cells;
      MAKE_PTR(TransitCellProliferativeType, p_transit_type);
      //MyCycle* cycle = new MyCycle();
      //CellsGenerator<cycle->CreateCellCycleModel(), 2> cells_generator;
      CellsGenerator<CycleEpiDirectionSCL, 2> cells_generator;
      cells_generator.GenerateBasicRandom(cells, p_mesh->GetNumElements(), p_transit_type);

      VertexBasedCellPopulation<2> cell_population(*p_mesh, cells);




      //cell_population.AddPopulationWriter<VoronoiDataWriter>();


      OffLatticeSimulation<2> simulator(cell_population);

      simulator.SetOutputDirectory("TestSmallCellLumen");
      simulator.SetSamplingTimestepMultiple(SimulationParameters::SAMPLING_TIMESTEP);
      simulator.SetDt(SimulationParameters::TIMESTEP);
      //en heure, 24/48

      simulator.SetEndTime(SimulationParameters::TIME_OF_SIMULATION);
      //std::cout << "coucou" << std::endl;


      //Modifier

      MAKE_PTR(NagaiHondaForce<2>, p_force);
      simulator.AddForce(p_force);

      MAKE_PTR(SimpleTargetAreaModifier<2>, p_growth_modifier);
      simulator.AddSimulationModifier(p_growth_modifier);

      MAKE_PTR(PolarisationModifier<2>, p_direction_modifier);
      simulator.AddSimulationModifier(p_direction_modifier);

      MAKE_PTR(LumenDestructionModifierSCL<2>, p_lumen_destruction_modifier);
      simulator.AddSimulationModifier(p_lumen_destruction_modifier);

      MAKE_PTR(LumenGenerationModifierSCL<2>, p_lumen_generation_modifier);
      simulator.AddSimulationModifier(p_lumen_generation_modifier);

      MAKE_PTR(LumenModifierSCL<2>, p_lumen_modifier);
      simulator.AddSimulationModifier(p_lumen_modifier);


      MAKE_PTR(SimuInfoModifier<2>, p_info_simu_modifier);
      simulator.AddSimulationModifier(p_info_simu_modifier);

      //Writer
      cell_population.AddCellWriter<CellPolarityXWriter>();
      cell_population.AddCellWriter<CellPolarityYWriter>();
      cell_population.AddCellWriter<CellTypeWriter>();


      MAKE_PTR(CellEpi, p_epiDir);
      MAKE_PTR(CellEndo, p_endo);




      MAKE_PTR_ARGS(CellKillerSCL, p_killer, (&cell_population));

      //simulator.AddCellKiller(p_killer);

      //CycleEpiDirection* p_cell_cycle_model = new CycleEpiDirection();
      SimulationParameters::getNextIndex();

      int counter = 0;

      for (AbstractCellPopulation<2>::Iterator cell_iter = cell_population.Begin();
           cell_iter != cell_population.End();
           ++cell_iter)
      {

        //On definit le cycle
        //cell_iter->SetCellCycleModel();
        //cell_iter->GetCellData()->SetItem("target area", 10);


        cell_iter->GetCellData()->SetItem("cellIndex",SimulationParameters::getNextIndex());
        cell_iter->GetCellData()->SetItem("timeFromLastLumenGeneration",0);
        cell_iter->GetCellData()->SetItem("vecPolaX",0);
        cell_iter->GetCellData()->SetItem("vecPolaY",0);

        if(counter ==2){
          cell_iter->AddCellProperty(p_endo);
        }
        else{
          cell_iter->AddCellProperty(p_epiDir);




        }
        counter++;

      }

      simulator.Solve();


    }


};


#endif
