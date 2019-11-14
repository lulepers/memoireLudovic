#include "CellPolarityYWriter.hpp"
#include "AbstractCellPopulation.hpp"
#include "CellEpiDirection.hpp"


template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
CellPolarityYWriter<ELEMENT_DIM, SPACE_DIM>::CellPolarityYWriter()
    : AbstractCellWriter<ELEMENT_DIM, SPACE_DIM>("results.viztypes")
{
    this->mVtkCellDataName = "Cellular polarisastion Y";
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
double CellPolarityYWriter<ELEMENT_DIM, SPACE_DIM>::GetCellDataForVtkOutput(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    double vecPolaY = 0;
    if (pCell->HasCellProperty<CellEpiDirection>())
    {
          vecPolaY = pCell->GetCellData()->GetItem("vecPolaY");
    }


    return vecPolaY;
}

template<unsigned ELEMENT_DIM, unsigned SPACE_DIM>
void CellPolarityYWriter<ELEMENT_DIM, SPACE_DIM>::VisitCell(CellPtr pCell, AbstractCellPopulation<ELEMENT_DIM, SPACE_DIM>* pCellPopulation)
{
    double vecPolaY = 0;
    if (pCell->HasCellProperty<CellEpiDirection>())
    {
        //CellPropertyCollection collection = pCell->rGetCellPropertyCollection().GetProperties<CellEpiDirection>();
        //boost::shared_ptr<CellEpiDirection> p_celltype = boost::static_pointer_cast<CellEpiDirection>(collection.GetProperty());
        vecPolaY = pCell->GetCellData()->GetItem("vecPolaY");
    }


    *this->mpOutStream << " " << vecPolaY;

    unsigned location_index = pCellPopulation->GetLocationIndexUsingCell(pCell);
    *this->mpOutStream << " " << location_index;

    c_vector<double, SPACE_DIM> coords = pCellPopulation->GetLocationOfCellCentre(pCell);
    for (unsigned i=0; i<SPACE_DIM; i++)
    {
        *this->mpOutStream << " " << coords[i];
    }
}

// Explicit instantiation
template class CellPolarityYWriter<1,1>;
template class CellPolarityYWriter<1,2>;
template class CellPolarityYWriter<2,2>;
template class CellPolarityYWriter<1,3>;
template class CellPolarityYWriter<2,3>;
template class CellPolarityYWriter<3,3>;

#include "SerializationExportWrapperForCpp.hpp"
// Declare identifier for the serializer
EXPORT_TEMPLATE_CLASS_ALL_DIMS(CellPolarityYWriter)
