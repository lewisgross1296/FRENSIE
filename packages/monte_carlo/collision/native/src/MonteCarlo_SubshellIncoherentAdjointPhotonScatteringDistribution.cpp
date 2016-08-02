//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_SubshellIncoherentAdjointPhotonScatteringDistribution.cpp
//! \author Alex Robinson
//! \brief  The subshell incoherent adjoint photon scattering dist. decl.
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <functional>

// FRENSIE Includes
#include "MonteCarlo_SubshellIncoherentAdjointPhotonScatteringDistribution.hpp"
#include "Utility_GaussKronrodIntegrator.hpp"
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

// Constructor
SubshellIncoherentAdjointPhotonScatteringDistribution::SubshellIncoherentAdjointPhotonScatteringDistribution(
      const double max_energy,
      const Data::SubshellType interaction_subshell,
      const double num_electrons_in_subshell,
      const double binding_energy,
      const Teuchos::RCP<const Utility::OneDDistribution>& occupation_number )
  : IncoherentAdjointPhotonScatteringDistribution( max_energy ),
    d_subshell( interaction_subshell ),
    d_num_electrons_in_subshell( num_electrons_in_subshell ),
    d_binding_energy( binding_energy ),
    d_occupation_number( occupation_number )
{
  // Make sure the binding energy is valid
  testPrecondition( binding_energy > 0.0 );
  // Make sure the max energy is valid
  testPrecondition( max_energy > binding_energy );
  // Make sure the interaction subshell is valid
  testPrecondition( interaction_subshell != INVALID_SUBSHELL );
  testPrecondition( interaction_subshell != UNKNOWN_SUBSHELL );
  // Make sure the subshell occupancy is valid
  testPrecondition( num_electrons_in_subshell > 0.0 );
  // Make sure the occupation number is valid
  testPrecondition( !occupation_number.is_null() );
  testPrecondition( occupation_number->getLowerBoundOfIndepVar() == -1.0 );
}

// Return the subshell
Data::SubshellType SubshellIncoherentAdjointPhotonScatteringDistribution::getSubshell() const
{
  return d_subshell;
}

// Return the number of electrons in the subshell
double SubshellIncoherentAdjointPhotonScatteringDistribution::getSubshellOccupancy() const
{
  return d_num_electrons_in_subshell;
}

// Return the binding energy
double SubshellIncoherentAdjointPhotonScatteringDistribution::getSubshellBindingEnergy() const
{
  return d_binding_energy;
}

// Evaluate the distribution
double SubshellIncoherentAdjointPhotonScatteringDistribution::evaluate(
                                   const double incoming_energy,
                                   const double max_energy,
                                   const double scattering_angle_cosine ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy > 0.0 );
  testPrecondition( incoming_energy <= max_energy - d_binding_energy );
  // Make sure the scattering angle cosine is valid
  testPrecondition( scattering_angle_cosine >=
                    calculateMinScatteringAngleCosine( incoming_energy, max_energy ) );
  testPrecondition( scattering_angle_cosine <= 1.0 );

  const double adjoint_occupation_number =
    this->evaluateAdjointOccupationNumber(
                        incoming_energy, max_energy, scattering_angle_cosine );

  const double diff_kn_cross_section =
    this->evaluateAdjointKleinNishinaDist( incoming_energy,
                                           max_energy,
                                           scattering_angle_cosine );

  return d_num_electrons_in_subshell*adjoint_occupation_number*diff_kn_cross_section;
}

// Evaluate the integrated cross section (b)
double SubshellIncoherentAdjointPhotonScatteringDistribution::evaluateIntegratedCrossSection(
                                                 const double incoming_energy,
                                                 const double max_energy,
                                                 const double precision ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy > 0.0 );
  testPrecondition( incoming_energy <= max_energy - d_binding_energy );

  // Evaluate the integrated cross section
  std::function<double (double x)> diff_cs_wrapper =
    boost::bind<double>( &ThisType::evaluate,
                         boost::cref( *this ),
                         incoming_energy,
                         max_energy,
                         _1 );

  double abs_error, integrated_cs;

  Utility::GaussKronrodIntegrator quadrature_gkq_int( precision );

  const double min_scattering_angle_cosine =
    calculateMinScatteringAngleCosine( incoming_energy, max_energy );

  quadrature_gkq_int.integrateAdaptively<15>( diff_cs_wrapper,
                                              min_scattering_angle_cosine,
                                              1.0,
                                              integrated_cs,
                                              abs_error );

  // Make sure the integrated cross section is valid
  testProstcondition( integrated_cs >= 0.0 );

  return integrated_cs;
}

// Sample an outgoing energy and direction from the distribution
void SubshellIncoherentAdjointPhotonScatteringDistribution::sample(
                                        const double incoming_energy,
                                        double& outgoing_energy,
                                        double& scattering_angle_cosine ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy > 0.0 );
  testPrecondition( incoming_energy <= this->getMaxEnergy()-d_binding_energy );

  unsigned trial_dummy;

  return this->sampleAndRecordTrials( incoming_energy,
                                      outgoing_energy,
                                      scattering_angle_cosine,
                                      trial_dummy );
}

// Sample an outgoing energy and direction and record the number of trials
void SubshellIncoherentAdjointPhotonScatteringDistribution::sampleAndRecordTrials(
                                               const double incoming_energy,
                                               double& outgoing_energy,
                                               double& scattering_angle_cosine,
                                               unsigned& trials ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy > 0.0 );
  testPrecondition( incoming_energy <= this->getMaxEnergy()-d_binding_energy );

  // Evaluate the maximum adjoint occupation number
  const double min_scattering_angle_cosine =
    calculateMinScatteringAngleCosine( incoming_energy,
                                       this->getMaxEnergy() );

  const double max_pz_max =
    this->calculateOccupationNumberUpperArgument( incoming_energy,
                                                  min_scattering_angle_cosine);

  const double max_occupation_number =
    d_occupation_number->evaluate( max_pz_max );

  while( true )
  {
    this->sampleAndRecordTrialsAdjointKleinNishina( incoming_energy,
                                                    outgoing_energy,
                                                    scattering_angle_cosine,
                                                    trials );

    const double adjoint_occupation_number =
      this->evaluateAdjointOccupationNumber( incoming_energy,
                                             this->getMaxEnergy(),
                                             scattering_angle_cosine );

    const double scaled_random_number = max_occupation_number*
      Utility::RandomNumberGenerator::getRandomNumber<double>();

    if( scaled_random_number <= adjoint_occupation_number )
      break;
  }

  // Make sure the scattering angle cosine is valid
  testPostcondition( scattering_angle_cosine >= min_scattering_angle_cosine );
  testPostcondition( scattering_angle_cosine <= 1.0 );
  // Make sure the adjoint Compton line energy is valid
  testPostcondition( outgoing_energy >= incoming_energy );
}

// Randomly scatter the photon and return the shell that was interacted with
void SubshellIncoherentAdjointPhotonScatteringDistribution::scatterAdjointPhoton(
                               AdjointPhotonState& adjoint_photon,
                               ParticleBank& bank,
                               Data::SubshellType& shell_of_interaction ) const
{
  // Make sure the adjoint photon energy is valid
  testPrecondition( adjoint_photon.getEnergy() <= this->getMaxEnergy() );

  // Generate probe particles
  this->createProbeParticles( adjoint_photon, bank );

  // Scattering the adjoint photon
  double outgoing_energy, scattering_angle_cosine;

  this->sample( adjoint_photon.getEnergy(),
                outgoing_energy,
                scattering_angle_cosine );

  shell_of_interaction = d_subshell;

  adjoint_photon.setEnergy( outgoing_energy );
  adjoint_photon.rotateDirection( scattering_angle_cosine,
                                  this->sampleAzimuthalAngle() );
}

// Check if an energy is above the scattering window
/*! \details An adjoint incoherent interaction can only occur when enough
 * energy is transferred to the electron to free it from its subshell. This
 * sets an upper limit for the window to energy_of_interest - binding_energy.
 */
bool SubshellIncoherentAdjointPhotonScatteringDistribution::isEnergyAboveScatteringWindow(
                                            const double energy_of_interest,
                                            const double initial_energy ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( initial_energy > 0.0 );
  // Make sure the energy of interest is valid
  testPrecondition( energy_of_interest >= 0.0 );

  return initial_energy >= energy_of_interest - this->getMaxEnergy();
}

// Calculate the occupation number arguments
double SubshellIncoherentAdjointPhotonScatteringDistribution::calculateOccupationNumberArguments(
                                          const double incoming_energy,
                                          const double max_energy,
                                          const double scattering_angle_cosine,
                                          double& pz_min,
                                          double& pz_max ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy > 0.0 );
  testPrecondition( incoming_energy <= max_energy - d_binding_energy );
  // Make sure the scattering angle cosine is valid
  testPrecondition( scattering_angle_cosine >=
                    calculateMinScatteringAngleCosine( incoming_energy, max_energy ) );
  testPrecondition( scattering_angle_cosine <= 1.0 );

  pz_max = this->calculateOccupationNumberUpperArgument(
                                                     incoming_energy,
                                                     scattering_angle_cosine );

  pz_min = calculateMinElectronMomentumProjectionAdjoint(
                                                     incoming_energy,
                                                     d_binding_energy,
                                                     scattering_angle_cosine );

  // Note: pz_max has no bound. We therefore limit it to the maximum pz
  // stored in the occupation number grid. If pz_max is very large, pz_min
  // can also be very large (especially at small scattering angles) but can
  // never be larger than pz_max. Because we are limiting pz_max we must
  // also limit pz_min.
  if( pz_min > pz_max )
    pz_min = pz_max;
}

// Evaluate the occupation number
double SubshellIncoherentAdjointPhotonScatteringDistribution::evaluateAdjointOccupationNumber(
                                   const double incoming_energy,
                                   const double max_energy,
                                   const double scattering_angle_cosine ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy > 0.0 );
  testPrecondition( incoming_energy <= max_energy - d_binding_energy );
  // Make sure the scattering angle cosine is valid
  testPrecondition( scattering_angle_cosine >=
                    calculateMinScatteringAngleCosine( incoming_energy, max_energy ) );
  testPrecondition( scattering_angle_cosine <= 1.0 );

  // Evaluate the occupation number at pz_max and pz_min
  double pz_min, pz_max;

  this->calculateOccupationNumberArguments( incoming_energy,
                                            max_energy,
                                            scattering_angle_cosine,
                                            pz_min,
                                            pz_max );
  
  const double upper_occupation_number_value =
    d_occupation_number->evaluate( pz_max );

  const double lower_occupation_number_value =
    d_occupation_number->evaluate( pz_min );

  // Evaluate the adjoint occupation number
  const double adjoint_occupation_number =
    upper_occupation_number - lower_occupation_number;

  // Make sure the adjoint occupation number is valid
  testPrecondition( adjoint_occupation_number >= 0.0 );
  testPrecondition( adjoint_occupation_number <= 1.0 );

  return adjoint_occupation_number;
}

// Calculate the occupation number upper argument
double SubshellIncoherentAdjointPhotonScatteringDistribution::calculateOccupationNumberUpperArgument(
                                   const double incoming_energy,
                                   const double scattering_angle_cosine ) const
{
  // Make sure the incoming energy is valid
  testPrecondition( incoming_energy > 0.0 );
  // Make sure the scattering angle cosine is valid
  testPrecondition( scattering_angle_cosine >=
                    calculateMinScatteringAngleCosine( incoming_energy, max_energy ) );
  testPrecondition( scattering_angle_cosine <= 1.0 );

  double occupation_number_arg =
    calculateMaxElectronMomentumProjectionAdjoint( incoming_energy,
                                                   d_binding_energy,
                                                   scattering_angle_cosine );

  if( occupation_number_arg >= d_occupation_number->getUpperBoundOfIndepVar() )
    occupation_number_arg = d_occupation_number->getUpperBoundOfIndepVar();

  // Make sure the occupation number arg is valid
  testPrecondition( occupation_number_arg >=
                    d_occupation_number->getLowerBoundOfIndepVar() );
  testPrecondition( occupation_number_arg <=
                    d_occupation_number->getUpperBoundOfIndepVar() );

  return occupation_number_arg;
}
  
} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_SubshellIncoherentAdjointPhotonScatteringDistribution.cpp
//---------------------------------------------------------------------------//
