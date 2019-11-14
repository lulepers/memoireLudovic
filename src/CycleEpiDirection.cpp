/*

 Copyright (c) 2005-2017, University of Oxford.
 All rights reserved.

 University of Oxford means the Chancellor, Masters and Scholars of the
 University of Oxford, having an administrative office at Wellington
 Square, Oxford OX1 2JD, UK.

 This file is part of Chaste.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 * Neither the name of the University of Oxford nor the names of its
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#include "CycleEpiDirection.hpp"
#include "CellEpiDirection.hpp"
#include "RandomNumberGenerator.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "Parameters.hpp"

CycleEpiDirection::CycleEpiDirection()
: AbstractSimplePhaseBasedCellCycleModel()
{
}

CycleEpiDirection::CycleEpiDirection(const CycleEpiDirection& rModel)
: AbstractSimplePhaseBasedCellCycleModel(rModel)
{
    /*
     * Initialize only those member variables defined in this class.
     *
     * The member variables mBirthTime, mReadyToDivide and mDimension
     * are initialized in the AbstractCellCycleModel constructor.
     */
}

bool CycleEpiDirection::ReadyToDivide()
{
  assert(mpCell != nullptr);
  if (!mReadyToDivide)
  {
    if (mpCell->HasCellProperty<CellEpiDirection>())
    {
      double timeFromLastGen = mpCell->GetCellData()->GetItem("timeFromLastLumenGeneration");

      double xl = mpCell->GetCellData()->GetItem("vecPolaX");
      double yl = mpCell->GetCellData()->GetItem("vecPolaY");

      double norme = sqrt(xl*xl + yl*yl);
      if(norme >Parameters::THRESHOLD_POLARISATION_EPI)
      {

        if(timeFromLastGen > Parameters::TIME_BEETWEN_TWO_LUMEN_GENERATION)
        {
          if (GetAge() > GetMinimumDivisionAge())
          {
            mReadyToDivide = true;
          }
        }
      }
    }
  }
  return mReadyToDivide;
}

AbstractSimplePhaseBasedCellCycleModel* CycleEpiDirection::CreateCellCycleModel()
{
    return new CycleEpiDirection(*this);
}


double CycleEpiDirection::GetMinimumDivisionAge()
{
    return Parameters::TIME_BEETWEN_TWO_LUMEN_GENERATION;
}

double CycleEpiDirection::GetAverageTransitCellCycleTime()
{
    return 1.0;
}

double CycleEpiDirection::GetAverageStemCellCycleTime()
{
    return 1.0;
}

void CycleEpiDirection::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
    //*rParamsFile << "\t\t\t<MaxArea>" << mMaxArea << "</MaxArea>\n";
    *rParamsFile << "\t\t\t<MinimumDivisionAge>" << Parameters::TIME_BEETWEN_TWO_LUMEN_GENERATION << "</MinimumDivisionAge>\n";

    // Call method on direct parent class
    AbstractSimplePhaseBasedCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(CycleEpiDirection)
