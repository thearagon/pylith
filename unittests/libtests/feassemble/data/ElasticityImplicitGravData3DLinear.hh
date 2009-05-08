// -*- C++ -*-
//
// ======================================================================
//
//                           Brad T. Aagaard
//                        U.S. Geological Survey
//
// {LicenseText}
//
// ======================================================================
//

// DO NOT EDIT THIS FILE
// This file was generated from python application elasticityimplicitgrav.

#if !defined(pylith_feassemble_elasticityimplicitgravdata3dlinear_hh)
#define pylith_feassemble_elasticityimplicitgravdata3dlinear_hh

#include "IntegratorData.hh"

namespace pylith {
  namespace feassemble {
     class ElasticityImplicitGravData3DLinear;
  } // pylith
} // feassemble

class pylith::feassemble::ElasticityImplicitGravData3DLinear : public IntegratorData
{

public: 

  /// Constructor
  ElasticityImplicitGravData3DLinear(void);

  /// Destructor
  ~ElasticityImplicitGravData3DLinear(void);

private:

  static const int _spaceDim;

  static const int _cellDim;

  static const int _numVertices;

  static const int _numCells;

  static const int _numBasis;

  static const int _numQuadPts;

  static const char* _matType;

  static const char* _matDBFilename;

  static const int _matId;

  static const char* _matLabel;

  static const double _dt;

  static const double _gravityVec[];

  static const double _vertices[];

  static const int _cells[];

  static const double _verticesRef[];

  static const double _quadPts[];

  static const double _quadWts[];

  static const double _basis[];

  static const double _basisDerivRef[];

  static const double _fieldTIncr[];

  static const double _fieldT[];

  static const double _fieldTmdt[];

  static const double _valsResidual[];

  static const double _valsJacobian[];

};

#endif // pylith_feassemble_elasticityimplicitgravdata3dlinear_hh

// End of file
