//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_PhotoatomicReaction.cpp
//! \author Alex Robinson
//! \brief  The photoatomic reaction base class instantiation
//!
//---------------------------------------------------------------------------//

// FRENSIE Includes
#include "MonteCarlo_PhotoatomicReaction.hpp"

namespace MonteCarlo{

EXPLICIT_TEMPLATE_CLASS_INST( StandardGenericAtomicReaction<PhotoatomicReaction,Utility::LinLin,false> );
EXPLICIT_TEMPLATE_CLASS_INST( StandardGenericAtomicReaction<PhotoatomicReaction,Utility::LinLin,true> );

EXPLICIT_TEMPLATE_CLASS_INST( StandardGenericAtomicReaction<PhotoatomicReaction,Utility::LinLog,false> );
EXPLICIT_TEMPLATE_CLASS_INST( StandardGenericAtomicReaction<PhotoatomicReaction,Utility::LinLog,true> );

EXPLICIT_TEMPLATE_CLASS_INST( StandardGenericAtomicReaction<PhotoatomicReaction,Utility::LogLin,false> );
EXPLICIT_TEMPLATE_CLASS_INST( StandardGenericAtomicReaction<PhotoatomicReaction,Utility::LogLin,true> );

EXPLICIT_TEMPLATE_CLASS_INST( StandardGenericAtomicReaction<PhotoatomicReaction,Utility::LogLog,false> );
EXPLICIT_TEMPLATE_CLASS_INST( StandardGenericAtomicReaction<PhotoatomicReaction,Utility::LogLog,true> );
  
} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_PhotoatomicReaction.cpp
//---------------------------------------------------------------------------//
