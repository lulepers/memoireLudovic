#ifndef TESTBIGCELLLUMEN_HPP_
#define TESTBIGCELLLUMEN_HPP_

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

#include "CycleBCL.hpp"

#include "CellEpi.hpp"
#include "CellEndo.hpp"

#include "PolarisationModifier.hpp"

#include "CellTypeWriter.hpp"
#include "CellPolarityXWriter.hpp"
#include "CellPolarityYWriter.hpp"

#include "SimuInfoModifier.hpp"
#include "LumenGenerationModifierBCL.hpp"
#include "LumenModifierBCL.hpp"


#include "SimulationParameters.hpp"




class TestBigCellLumen : public AbstractCellBasedTestSuite
{
public:

    //export CHASTE_TEST_OUTPUT=../Chaste_src/testoutput/
    //SCL = smallCellLumen
    //BCL = bigCellLumen


    void TestBigCellLumenX()
    {

      //Parameters::init();





      HoneycombVertexMeshGenerator generator(6, 3);
      MutableVertexMesh<2,2>* p_mesh = generator.GetMesh();

      std::vector<CellPtr> cells;
      MAKE_PTR(TransitCellProliferativeType, p_transit_type);
      //MyCycle* cycle = new MyCycle();
      //CellsGenerator<cycle->CreateCellCycleModel(), 2> cells_generator;
      CellsGenerator<CycleBCL, 2> cells_generator;
      cells_generator.GenerateBasicRandom(cells, p_mesh->GetNumElements(), p_transit_type);

      VertexBasedCellPopulation<2> cell_population(*p_mesh, cells);




      //cell_population.AddPopulationWriter<VoronoiDataWriter>();


      OffLatticeSimulation<2> simulator(cell_population);

      simulator.SetOutputDirectory("TestBigCellLumen");
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

      MAKE_PTR(PolarisationModifier<2>, p_polarisation_modifier);
      simulator.AddSimulationModifier(p_polarisation_modifier);

      MAKE_PTR(LumenGenerationModifierBCL<2>, p_lumen_generation_modifier);
      simulator.AddSimulationModifier(p_lumen_generation_modifier);

      MAKE_PTR(LumenModifierBCL<2>, p_lumen_modifier);
      simulator.AddSimulationModifier(p_lumen_modifier);

      MAKE_PTR(SimuInfoModifier<2>, p_info_simu_modifier);
      simulator.AddSimulationModifier(p_info_simu_modifier);

      //Writer
      cell_population.AddCellWriter<CellTypeWriter>();
      cell_population.AddCellWriter<CellPolarityXWriter>();
      cell_population.AddCellWriter<CellPolarityYWriter>();

      MAKE_PTR(CellEpi, p_epi);
      MAKE_PTR(CellEndo, p_endo);

      int counter = 0;

      for (AbstractCellPopulation<2>::Iterator cell_iter = cell_population.Begin();
           cell_iter != cell_population.End();
           ++cell_iter)
      {

        cell_iter->GetCellData()->SetItem("cellIndex",SimulationParameters::getNextIndex());
        cell_iter->GetCellData()->SetItem("timeFromLastLumenGeneration",0);
        cell_iter->GetCellData()->SetItem("lumenNearby",1);
        cell_iter->GetCellData()->SetItem("vecPolaX",0);
        cell_iter->GetCellData()->SetItem("vecPolaY",0);

        if(counter ==2){
          cell_iter->AddCellProperty(p_endo);
        }
        else{
          cell_iter->AddCellProperty(p_epi);

        }

          counter = counter +1;
      }

      simulator.Solve();


    }


};


#endif
