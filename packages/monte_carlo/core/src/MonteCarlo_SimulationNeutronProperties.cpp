//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_SimulationNeutronProperties.hpp
//! \author Alex Robinson, Luke Kersting
//! \brief  Simulation neutron properties class def.
//!
//---------------------------------------------------------------------------//

// FRENSIE Includes
#include "MonteCarlo_SimulationNeutronProperties.hpp"
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

// Initialize the static member data
const double SimulationNeutronProperties::s_absolute_min_neutron_energy = 1e-11;

// The absolute max neutron energy (MeV)
const double SimulationNeutronProperties::s_absolute_max_neutron_energy = 20.0;

// Constructor
SimulationNeutronProperties::SimulationNeutronProperties()
  : d_free_gas_threshold( 400.0 ),
    d_min_neutron_energy( s_absolute_min_neutron_energy ),
    d_max_neutron_energy( s_absolute_max_neutron_energy )
{ /* ... */ }

// Set the free gas thermal treatment temperature threshold
/*! \details The value given is the number of times above the material
 * temperature that the energy of a neutron can be before the free gas
 * thermal treatment is not used anymore.
 */
void SimulationNeutronProperties::setFreeGasThreshold( const double threshold )
{
  // Make sure the threshold is valid
  testPrecondition( threshold > 0.0 );

  d_free_gas_threshold = threshold;
}

// Return the free gas thermal treatment temperature threshold
double SimulationNeutronProperties::getFreeGasThreshold() const
{
  return d_free_gas_threshold;
}

// Set the minimum neutron energy (MeV)
void SimulationNeutronProperties::setMinNeutronEnergy( const double energy )
{
  // Make sure the energy is valid
  testPrecondition(energy >= s_absolute_min_neutron_energy);
  testPrecondition( energy < d_max_neutron_energy );

  d_min_neutron_energy = energy;
}

// Return the minimum neutron energy (MeV)
double SimulationNeutronProperties::getMinNeutronEnergy() const
{
  return d_min_neutron_energy;
}

// Return the absolute min neutron energy (MeV)
double SimulationNeutronProperties::getAbsoluteMinNeutronEnergy()
{
  return s_absolute_min_neutron_energy;
}

// Set the max neutron energy (MeV)
void SimulationNeutronProperties::setMaxNeutronEnergy( const double energy )
{
  // Make sure the energy is valid
  testPrecondition(energy > d_min_neutron_energy );
  testPrecondition(energy <= s_absolute_max_neutron_energy);

  d_max_neutron_energy = energy;
}

// Return the maximum neutron energy (MeV) - cannot be set at runtime
double SimulationNeutronProperties::getMaxNeutronEnergy() const
{
  return d_max_neutron_energy;
}

// Return the absolute maximum neutron energy (MeV)
double SimulationNeutronProperties::getAbsoluteMaxNeutronEnergy()
{
  return s_absolute_max_neutron_energy;
}

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_SimulationNeutronProperties.cpp
//---------------------------------------------------------------------------//
