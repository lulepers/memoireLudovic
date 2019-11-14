#ifndef CELLEPIDIRECTION_HPP_
#define CELLEPIDIRECTION_HPP_

#include <boost/shared_ptr.hpp>
#include "AbstractCellProperty.hpp"
#include "ChasteSerialization.hpp"
#include <boost/serialization/base_object.hpp>

class CellEpiDirection : public AbstractCellProperty
{
protected:

    double vecPolarisation[2];
    unsigned mColour;

private:

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractCellProperty>(*this);
        archive & mColour;
        archive & vecPolarisation;
    }

public:

    CellEpiDirection(unsigned colour=1);

    virtual ~CellEpiDirection();



    double* GetVecPolarisation();
    double GetNormeVecPolarisation();
    unsigned GetColour() const;
};

#include "SerializationExportWrapper.hpp"
CHASTE_CLASS_EXPORT(CellEpiDirection)

#endif
