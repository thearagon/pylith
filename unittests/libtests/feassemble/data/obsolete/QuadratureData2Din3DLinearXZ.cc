// -*- C++ -*-
//
// ======================================================================
//
// Brad T. Aagaard, U.S. Geological Survey
// Charles A. Williams, GNS Science
// Matthew G. Knepley, University of Chicago
//
// This code was developed as part of the Computational Infrastructure
// for Geodynamics (http://geodynamics.org).
//
// Copyright (c) 2010-2017 University of California, Davis
//
// See COPYING for license information.
//
// ======================================================================
//

// DO NOT EDIT THIS FILE
// This file was generated from python application quadratureapp.

#include "QuadratureData2Din3DLinearXZ.hh"

const int pylith::feassemble::QuadratureData2Din3DLinearXZ::_numVertices = 3;

const int pylith::feassemble::QuadratureData2Din3DLinearXZ::_spaceDim = 3;

const int pylith::feassemble::QuadratureData2Din3DLinearXZ::_numCells = 1;

const int pylith::feassemble::QuadratureData2Din3DLinearXZ::_cellDim = 2;

const int pylith::feassemble::QuadratureData2Din3DLinearXZ::_numBasis = 3;

const int pylith::feassemble::QuadratureData2Din3DLinearXZ::_numQuadPts = 1;

const PylithScalar pylith::feassemble::QuadratureData2Din3DLinearXZ::_vertices[] = {
  0.00000000e+00,  0.00000000e+00,  0.00000000e+00,
 -1.00000000e+00,  0.00000000e+00,  0.00000000e+00,
  0.00000000e+00,  0.00000000e+00,  1.00000000e+00,
};

const int pylith::feassemble::QuadratureData2Din3DLinearXZ::_cells[] = {
       0,       1,       2,
};

const PylithScalar pylith::feassemble::QuadratureData2Din3DLinearXZ::_verticesRef[] = {
 -1.00000000e+00, -1.00000000e+00,
  1.00000000e+00, -1.00000000e+00,
 -1.00000000e+00,  1.00000000e+00,
};

const PylithScalar pylith::feassemble::QuadratureData2Din3DLinearXZ::_quadPtsRef[] = {
 -3.33333333e-01, -3.33333333e-01,
};

const PylithScalar pylith::feassemble::QuadratureData2Din3DLinearXZ::_quadWts[] = {
  2.00000000e+00,
};

const PylithScalar pylith::feassemble::QuadratureData2Din3DLinearXZ::_quadPts[] = {
 -3.33333333e-01,  0.00000000e+00,  3.33333333e-01,
};

const PylithScalar pylith::feassemble::QuadratureData2Din3DLinearXZ::_basis[] = {
  3.33333333e-01,  3.33333333e-01,
  3.33333333e-01,};

const PylithScalar pylith::feassemble::QuadratureData2Din3DLinearXZ::_basisDerivRef[] = {
 -5.00000000e-01, -5.00000000e-01,
  5.00000000e-01,  0.00000000e+00,
  0.00000000e+00,  5.00000000e-01,
};

const PylithScalar pylith::feassemble::QuadratureData2Din3DLinearXZ::_basisDeriv[] = {
  1.00000000e+00,  0.00000000e+00, -1.00000000e+00,
 -1.00000000e+00,  0.00000000e+00,  0.00000000e+00,
  0.00000000e+00,  0.00000000e+00,  1.00000000e+00,
};

const PylithScalar pylith::feassemble::QuadratureData2Din3DLinearXZ::_jacobian[] = {
 -5.00000000e-01,  0.00000000e+00,
  0.00000000e+00,  0.00000000e+00,
  0.00000000e+00,  5.00000000e-01,
};

const PylithScalar pylith::feassemble::QuadratureData2Din3DLinearXZ::_jacobianDet[] = {
  2.50000000e-01,
};

const PylithScalar pylith::feassemble::QuadratureData2Din3DLinearXZ::_jacobianInv[] = {
 -2.00000000e+00,  0.00000000e+00, -0.00000000e+00,
  0.00000000e+00,  0.00000000e+00,  2.00000000e+00,
};

pylith::feassemble::QuadratureData2Din3DLinearXZ::QuadratureData2Din3DLinearXZ(void)
{ // constructor
  numVertices = _numVertices;
  spaceDim = _spaceDim;
  numCells = _numCells;
  cellDim = _cellDim;
  numBasis = _numBasis;
  numQuadPts = _numQuadPts;
  vertices = const_cast<PylithScalar*>(_vertices);
  cells = const_cast<int*>(_cells);
  verticesRef = const_cast<PylithScalar*>(_verticesRef);
  quadPtsRef = const_cast<PylithScalar*>(_quadPtsRef);
  quadWts = const_cast<PylithScalar*>(_quadWts);
  quadPts = const_cast<PylithScalar*>(_quadPts);
  basis = const_cast<PylithScalar*>(_basis);
  basisDerivRef = const_cast<PylithScalar*>(_basisDerivRef);
  basisDeriv = const_cast<PylithScalar*>(_basisDeriv);
  jacobian = const_cast<PylithScalar*>(_jacobian);
  jacobianDet = const_cast<PylithScalar*>(_jacobianDet);
  jacobianInv = const_cast<PylithScalar*>(_jacobianInv);
} // constructor

pylith::feassemble::QuadratureData2Din3DLinearXZ::~QuadratureData2Din3DLinearXZ(void)
{}


// End of file