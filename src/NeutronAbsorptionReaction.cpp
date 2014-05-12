//---------------------------------------------------------------------------//
//!
//! \file   NeutronAbsorptionReaction.cpp
//! \author Alex Robinson
//! \brief  The neutron absorption reaction class definition
//!
//---------------------------------------------------------------------------//

// FACEMC Includes
#include "NeutronAbsorptionReaction.hpp"
#include "ContractException.hpp"

namespace FACEMC{

// Constructor
NeutronAbsorptionReaction::NeutronAbsorptionReaction(
		   const NuclearReactionType reaction_type,
		   const double temperature,
		   const double q_value,
		   const unsigned threshold_energy_index,
	           const Teuchos::ArrayRCP<const double>& incoming_energy_grid,
		   const Teuchos::ArrayRCP<const double>& cross_section )
  : NuclearReaction( reaction_type,
		     temperature,
		     q_value,
		     threshold_energy_index,
		     incoming_energy_grid,
		     cross_section )
{ /* ... */ }

// Return the number of neutrons emitted from the rxn at the given energy
unsigned NeutronAbsorptionReaction::getNumberOfEmittedNeutrons(
						    const double energy ) const
{
  return 0u;
}

// Simulate the reaction
void NeutronAbsorptionReaction::react( NeutronState& neutron, 
				       ParticleBank& bank ) const
{
  neutron.setAsGone();
}

} // end FACEMC namespace

//---------------------------------------------------------------------------//
// end NeutronAbsorptionReaction.cpp
//---------------------------------------------------------------------------//
