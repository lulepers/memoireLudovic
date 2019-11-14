#include "CellEpiDirection.hpp"
#include <math.h>

CellEpiDirection::CellEpiDirection(unsigned colour)
    : AbstractCellProperty(),
      mColour(colour)
{

}

CellEpiDirection::~CellEpiDirection()
{
}

double* CellEpiDirection::GetVecPolarisation()
{
    double* pointerVecPol = &vecPolarisation[0];
    return pointerVecPol;
}

double CellEpiDirection::GetNormeVecPolarisation()
{
    double norme = sqrt(vecPolarisation[0] * vecPolarisation[0] + vecPolarisation[1] * vecPolarisation[1]);
    return norme;
}



unsigned CellEpiDirection::GetColour() const
{
    return mColour;
}

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(CellEpiDirection)
