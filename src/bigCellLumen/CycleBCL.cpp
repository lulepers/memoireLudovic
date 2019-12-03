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

#include "CycleBCL.hpp"
#include "CellEpi.hpp"
#include "RandomNumberGenerator.hpp"
#include "DifferentiatedCellProliferativeType.hpp"
#include "SimulationParameters.hpp"

CycleBCL::CycleBCL()
: AbstractSimplePhaseBasedCellCycleModel()
{
}

CycleBCL::CycleBCL(const CycleBCL& rModel)
: AbstractSimplePhaseBasedCellCycleModel(rModel)
{
    /*
     * Initialize only those member variables defined in this class.
     *
     * The member variables mBirthTime, mReadyToDivide and mDimension
     * are initialized in the AbstractCellCycleModel constructor.
     */
}

bool CycleBCL::ReadyToDivide()
{
  assert(mpCell != nullptr);
  if (!mReadyToDivide)
  {
    if (mpCell->HasCellProperty<CellEpi>())
    {
      double timeFromLastGen = mpCell->GetCellData()->GetItem("timeFromLastLumenGeneration");

      double xl = mpCell->GetCellData()->GetItem("vecPolaX");
      double yl = mpCell->GetCellData()->GetItem("vecPolaY");

      double norme = sqrt(xl*xl + yl*yl);
      if(norme >SimulationParameters::THRESHOLD_POLARISATION_EPI)
      {

        if(timeFromLastGen > SimulationParameters::BCL_TIME_BEETWEN_TWO_LUMEN_GENERATION)
        {
          if (GetAge() > GetMinimumDivisionAge())
          {
            if(mpCell->GetCellData()->GetItem("lumenNearby") == 0)
            {
              mReadyToDivide = true;
            }

          }
        }
      }
    }
  }
  return mReadyToDivide;
}

AbstractSimplePhaseBasedCellCycleModel* CycleBCL::CreateCellCycleModel()
{
    return new CycleBCL(*this);
}


double CycleBCL::GetMinimumDivisionAge()
{
    return 1.0;
}

double CycleBCL::GetAverageTransitCellCycleTime()
{
    return 1.0;
}

double CycleBCL::GetAverageStemCellCycleTime()
{
    return 1.0;
}

void CycleBCL::OutputCellCycleModelParameters(out_stream& rParamsFile)
{
    //*rParamsFile << "\t\t\t<MaxArea>" << mMaxArea << "</MaxArea>\n";
    *rParamsFile << "\t\t\t<MinimumDivisionAge>" << GetMinimumDivisionAge() << "</MinimumDivisionAge>\n";

    // Call method on direct parent class
    AbstractSimplePhaseBasedCellCycleModel::OutputCellCycleModelParameters(rParamsFile);
}

// Serialization for Boost >= 1.36
#include "SerializationExportWrapperForCpp.hpp"
CHASTE_CLASS_EXPORT(CycleBCL)
