//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_CellPulseHeightEstimator.cpp
//! \author Alex Robinson
//! \brief  Cell pulse height estimator class template instantiations
//!
//---------------------------------------------------------------------------//

// FRENSIE Includes
#include "MonteCarlo_CellPulseHeightEstimator.hpp"

BOOST_CLASS_EXPORT_IMPLEMENT( MonteCarlo::WeightMultipliedCellPulseHeightEstimator );
EXPLICIT_TEMPLATE_CLASS_INST( MonteCarlo::CellPulseHeightEstimator<MonteCarlo::WeightMultiplier> );
EXPLICIT_CLASS_SAVE_LOAD_INST( MonteCarlo, CellPulseHeightEstimator<MonteCarlo::WeightMultiplier> );

BOOST_CLASS_EXPORT_IMPLEMENT( MonteCarlo::WeightAndEnergyMultipliedCellPulseHeightEstimator );
EXPLICIT_TEMPLATE_CLASS_INST( MonteCarlo::CellPulseHeightEstimator<MonteCarlo::WeightAndEnergyMultiplier> );
EXPLICIT_CLASS_SAVE_LOAD_INST( MonteCarlo, CellPulseHeightEstimator<MonteCarlo::WeightAndEnergyMultiplier> );
  
//---------------------------------------------------------------------------//
// end MonteCarlo_CellPulseHeightEstimator.cpp
//---------------------------------------------------------------------------//
