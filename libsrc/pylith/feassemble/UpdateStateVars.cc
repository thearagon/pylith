// -*- C++ -*-
//
// ----------------------------------------------------------------------
//
// Brad T. Aagaard, U.S. Geological Survey
// Charles A. Williams, GNS Science
// Matthew G. Knepley, University of Chicago
//
// This code was developed as part of the Computational Infrastructure
// for Geodynamics (http://geodynamics.org).
//
// Copyright (c) 2010-2016 University of California, Davis
//
// See COPYING for license information.
//
// ----------------------------------------------------------------------
//

#include <portinfo>

#include "UpdateStateVars.hh" // implementation of object methods

#include "pylith/topology/Field.hh" // USES Field

#include "pylith/utils/error.hh" // USES PYLITH_METHOD_*

#include <cassert> // USES assert()

// ---------------------------------------------------------------------------------------------------------------------
// Default constructor.
pylith::feassemble::UpdateStateVars::UpdateStateVars(void) :
    _stateVarsIS(NULL),
    _stateVarsDM(NULL),
    _stateVarsVecLocal(NULL),
    _stateVarsVecGlobal(NULL),
    _auxiliaryFieldVecGlobal(NULL) {}


// ---------------------------------------------------------------------------------------------------------------------
// Destructor.
pylith::feassemble::UpdateStateVars::~UpdateStateVars(void) {
    deallocate();
} // destructor


// ---------------------------------------------------------------------------------------------------------------------
// Deallocate PETSc and local data structures.
void
pylith::feassemble::UpdateStateVars::deallocate(void) {
    PYLITH_METHOD_BEGIN;

    PetscErrorCode err = 0;
    err = ISDestroy(&_stateVarsIS);PYLITH_CHECK_ERROR(err);
    err = DMDestroy(&_stateVarsDM);PYLITH_CHECK_ERROR(err);
    err = VecDestroy(&_stateVarsVecLocal);PYLITH_CHECK_ERROR(err);
    err = VecDestroy(&_stateVarsVecGlobal);PYLITH_CHECK_ERROR(err);
    err = VecDestroy(&_auxiliaryFieldVecGlobal);PYLITH_CHECK_ERROR(err);

    PYLITH_METHOD_END;
} // deallocate


// ---------------------------------------------------------------------------------------------------------------------
// Get PETSc DM associated with state variables.
PetscDM
pylith::feassemble::UpdateStateVars::stateVarsDM(void) {
    return _stateVarsDM;
} // stateVarsDM


// ---------------------------------------------------------------------------------------------------------------------
// Get PETSc local vector associated with state variables.
PetscVec
pylith::feassemble::UpdateStateVars::stateVarsLocalVector(void) {
    return _stateVarsVecLocal;
} // stateVarsLocalVector

// ---------------------------------------------------------------------------------------------------------------------
// Initialize layout for updating state variables.
void
pylith::feassemble::UpdateStateVars::initialize(const pylith::topology::Field& auxiliaryField) {
    PYLITH_METHOD_BEGIN;

    PetscErrorCode err = 0;
    PetscDM auxiliaryDM = auxiliaryField.dmMesh();

    const pylith::string_vector& subfieldNames = auxiliaryField.subfieldNames();
    const size_t numAuxiliarySubfields = subfieldNames.size();
    pylith::int_array stateSubfieldIndices(numAuxiliarySubfields);

    size_t numStateSubfields = 0;
    for (size_t iSubfield = 0; iSubfield < numAuxiliarySubfields; ++iSubfield) {
        const pylith::topology::Field::SubfieldInfo& info = auxiliaryField.subfieldInfo(subfieldNames[iSubfield].c_str());
        if (info.description.hasHistory) {
            stateSubfieldIndices[numStateSubfields++] = info.index;
        } // if
    } // for
    std::sort(&stateSubfieldIndices[0], &stateSubfieldIndices[numStateSubfields]);

    // Create subDM holding only the state vars, which we want to update.
    err = DMCreateSubDM(auxiliaryDM, numStateSubfields, &stateSubfieldIndices[0], &_stateVarsIS,
                        &_stateVarsDM);PYLITH_CHECK_ERROR(err);
    err = DMCreateGlobalVector(_stateVarsDM, &_stateVarsVecGlobal);PYLITH_CHECK_ERROR(err);
    err = DMCreateLocalVector(_stateVarsDM, &_stateVarsVecLocal);PYLITH_CHECK_ERROR(err);

    err = DMCreateGlobalVector(auxiliaryDM, &_auxiliaryFieldVecGlobal);

    PYLITH_METHOD_END;
} // initialize


// ---------------------------------------------------------------------------------------------------------------------
// Setup values for updating state variables.
void
pylith::feassemble::UpdateStateVars::prepare(pylith::topology::Field* auxiliaryField) {
    PYLITH_METHOD_BEGIN;

    // :TODO: Verify that we need the global vectors and can't get by with just using VecISCopy() with the local vector.

    PetscErrorCode err = 0;
    err = VecSet(_stateVarsVecLocal, 0.0);PYLITH_CHECK_ERROR(err);

    // Move auxiliaryDM data to global vector.
    assert(auxiliaryField);
    PetscDM auxiliaryDM = auxiliaryField->dmMesh();
    err = DMLocalToGlobalBegin(auxiliaryDM, auxiliaryField->localVector(), INSERT_VALUES, _auxiliaryFieldVecGlobal);PYLITH_CHECK_ERROR(err);
    err = DMLocalToGlobalEnd(auxiliaryDM, auxiliaryField->localVector(), INSERT_VALUES, _auxiliaryFieldVecGlobal);PYLITH_CHECK_ERROR(err);

    PYLITH_METHOD_END;
} // prepare


// ---------------------------------------------------------------------------------------------------------------------
// Setup values for updating state variables.
void
pylith::feassemble::UpdateStateVars::restore(pylith::topology::Field* auxiliaryField) {
    PYLITH_METHOD_BEGIN;

    PetscErrorCode err = 0;
    assert(auxiliaryField);
    PetscDM auxiliaryDM = auxiliaryField->dmMesh();

    // Move statevarDM data to global vector.
    err = DMLocalToGlobalBegin(_stateVarsDM, _stateVarsVecLocal, INSERT_VALUES, _stateVarsVecGlobal);PYLITH_CHECK_ERROR(err);
    err = DMLocalToGlobalEnd(_stateVarsDM, _stateVarsVecLocal, INSERT_VALUES, _stateVarsVecGlobal);PYLITH_CHECK_ERROR(err);

    // Copy global data from stateVars to auxiliaryField
    err = VecISCopy(_auxiliaryFieldVecGlobal, _stateVarsIS, SCATTER_FORWARD, _stateVarsVecGlobal);PYLITH_CHECK_ERROR(err);

    // Move auxiliaryDM data to local vector
    err = DMGlobalToLocalBegin(auxiliaryDM, _auxiliaryFieldVecGlobal, INSERT_VALUES, auxiliaryField->localVector());PYLITH_CHECK_ERROR(err);
    err = DMGlobalToLocalEnd(auxiliaryDM, _auxiliaryFieldVecGlobal, INSERT_VALUES, auxiliaryField->localVector());PYLITH_CHECK_ERROR(err);

    PYLITH_METHOD_END;
} // restore


// End of file
