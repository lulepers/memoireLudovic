#ifndef CYCLEBCLGENERATIONAL_HPP_
#define CYCLEBCLGENERATIONAL_HPP_

#include "AbstractSimpleGenerationalCellCycleModel.hpp"
#include "RandomNumberGenerator.hpp"
#include "AbstractCellCycleModel.hpp"
/**
 * A stochastic cell-cycle model employed by Meineke et al (2001) in their off-lattice
 * model of the intestinal crypt (doi:10.1046/j.0960-7722.2001.00216.x). Cells that
 * are proliferating are assigned G1 phase durations drawn from a uniform distribution,
 * which differs between stem and transit amplifying cells. All other cell-cycle phases
 * are held constant.
 */
class CycleBCLGenerational : public AbstractSimpleGenerationalCellCycleModel
{
    friend class TestSimpleCellCycleModels;

private:

    /** Needed for serialization. */
    friend class boost::serialization::access;
    /**
     * Archive the cell-cycle model and random number generator, never used directly - boost uses this.
     *
     * @param archive the archive
     * @param version the current version of this class
     */
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & boost::serialization::base_object<AbstractSimpleGenerationalCellCycleModel>(*this);

        // Make sure the RandomNumberGenerator singleton gets saved too
        SerializableSingleton<RandomNumberGenerator>* p_wrapper = RandomNumberGenerator::Instance()->GetSerializationWrapper();
        archive & p_wrapper;
        archive & mCycleDuration;
    }

protected:
    /**
    Duration of total cell cycle from experiment
    */
    double mCycleDuration ;
    /**
     * Set the duration of G1 phase. This method is called on each cell at the
     * start of a simulation, and for both daughter cells immediately following
     * cell division.
     *
     * If the cell associated with this cell-cycle model has stem proliferative
     * type, then the G1 phase duration is drawn from the uniform distribution
     * U[14,18]. If the cell has transit proliferative type (semi-differentiated),
     * then the G1 phase duration is drawn from the uniform distribution U[4,6].
     * These two distributions, proposed by Meineke et al (doi:10.1046/j.0960-7722.2001.00216.x),
     * reflect indirect biological observations that stem cells cycle more
     * slowly than their progeny.
     *
     * If the cell is differentiated, then the G1 phase duration is set to DBL_MAX,
     * so that the cell will never reach the end of G1 phase.
     */
    void SetG1Duration();

    /**
     * Protected copy-constructor for use by CreateCellCycleModel().
     *
     * The only way for external code to create a copy of a cell-cycle model
     * is by calling that method, to ensure that a model of the correct subclass is created.
     * This copy-constructor helps subclasses to ensure that all member variables are correctly copied when this happens.
     *
     * This method is called by child classes to set member variables for a daughter cell upon cell division.
     * Note that the parent cell cycle model will have had ResetForDivision() called just before CreateCellCycleModel() is called,
     * so performing an exact copy of the parent is suitable behaviour. Any daughter-cell-specific initialisation
     * can be done in InitialiseDaughterCell().
     *
     * @param rModel the cell cycle model to copy.
     */
    CycleBCLGenerational(const CycleBCLGenerational& rModel);

public:

    /**
     * Constructor - just a default, mBirthTime is set in the AbstractCellCycleModel class.
     * mG1Duration is set very high, it is set for the individual cells when InitialiseDaughterCell is called
     */
    CycleBCLGenerational();


    virtual bool ReadyToDivide();

    /**
     * Overridden builder method to create new copies of
     * this cell-cycle model.
     *
     * @return new cell-cycle model
     */
    AbstractCellCycleModel* CreateCellCycleModel();

    void SetCycleDuration(double cycleduration) ;

    double GetCycleDuration() const ;

    /**
     * Overridden OutputCellCycleModelParameters() method.
     *
     * @param rParamsFile the file stream to which the parameters are output
     */
    virtual void OutputCellCycleModelParameters(out_stream& rParamsFile);
};

#include "SerializationExportWrapper.hpp"
// Declare identifier for the serializer
CHASTE_CLASS_EXPORT(CycleBCLGenerational)

#endif /*CycleBCLGenerational_HPP_*/
