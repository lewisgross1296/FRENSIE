//---------------------------------------------------------------------------//
//!
//! \file   tstBremsstrahlungAdjointElectronScatteringDistribution.cpp
//! \author Luke Kersting
//! \brief  Bremsstrahlung electron scattering distribution unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <limits>

// FRENSIE Includes
#include "MonteCarlo_BremsstrahlungAdjointElectronScatteringDistribution.hpp"
#include "Data_AdjointElectronPhotonRelaxationDataContainer.hpp"
#include "Utility_RandomNumberGenerator.hpp"
#include "Utility_UnitTestHarnessWithMain.hpp"
#include "MonteCarlo_AdjointElectronProbeState.hpp"

//---------------------------------------------------------------------------//
// Testing Structs
//---------------------------------------------------------------------------//
class TestBremsstrahlungAdjointElectronScatteringDistribution : public MonteCarlo::BremsstrahlungAdjointElectronScatteringDistribution
{
public:

  // Constructor
  TestBremsstrahlungAdjointElectronScatteringDistribution(
    const std::shared_ptr<const BasicBivariateDist>& adjoint_brem_scatter_dist )
    : MonteCarlo::BremsstrahlungAdjointElectronScatteringDistribution( adjoint_brem_scatter_dist )
  { /* ... */ }

  // Destructor
  ~TestBremsstrahlungAdjointElectronScatteringDistribution()
  { /* ... */ }

  using MonteCarlo::BremsstrahlungAdjointElectronScatteringDistribution::LineEnergyIterator;
  using MonteCarlo::BremsstrahlungAdjointElectronScatteringDistribution::isEnergyAboveScatteringWindow;
  using MonteCarlo::BremsstrahlungAdjointElectronScatteringDistribution::isEnergyInScatteringWindow;
  using MonteCarlo::BremsstrahlungAdjointElectronScatteringDistribution::getCriticalLineEnergiesInScatteringWindow;
};

//---------------------------------------------------------------------------//
// Testing Variables
//---------------------------------------------------------------------------//

std::shared_ptr<TestBremsstrahlungAdjointElectronScatteringDistribution>
  distribution;

double max_incoming_energy = 1.99999789891999988e+01;

//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
// Check that the min incoming electron energy
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   getMinEnergy )
{
  FRENSIE_CHECK_EQUAL( distribution->getMinEnergy(), 1E-5 );
}

//---------------------------------------------------------------------------//
// Check that the max incoming electron energy
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   getMaxEnergy )
{
  FRENSIE_CHECK_EQUAL( distribution->getMaxEnergy(), max_incoming_energy );
}

//---------------------------------------------------------------------------//
// Check that the min outgoing energy can be returned
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   getOutgoingMinEnergy )
{
  FRENSIE_CHECK_FLOATING_EQUALITY( distribution->getOutgoingMinEnergy( 1e-5 ), 1.0101e-05, 1e-12 );

  FRENSIE_CHECK_EQUAL( distribution->getOutgoingMinEnergy( 1.999997898919999884e+01 ), 1.99999790902e+01 );
}

//---------------------------------------------------------------------------//
// Check that the max outgoing energy can be returned
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   getOutgoingMaxEnergy )
{
  FRENSIE_CHECK_EQUAL( distribution->getOutgoingMaxEnergy( 1e-5 ), 20.0 );

  FRENSIE_CHECK_EQUAL( distribution->getOutgoingMaxEnergy( 1.999997898919999884e+01 ), 20.0 );
}

//---------------------------------------------------------------------------//
// Check if an energy is above the scattering window
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   isEnergyAboveScatteringWindow )
{
  FRENSIE_CHECK( !distribution->isEnergyAboveScatteringWindow( 0.1, 9.9999e-6 ) );

  FRENSIE_CHECK( !distribution->isEnergyAboveScatteringWindow( 0.1, 1e-5 ) );

  FRENSIE_CHECK( !distribution->isEnergyAboveScatteringWindow( 0.1, 0.1 - 2e-7 ) );

  FRENSIE_CHECK( distribution->isEnergyAboveScatteringWindow( 0.1, 0.1 ) );
}

//---------------------------------------------------------------------------//
// Check if an energy is in the scattering window
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   isEnergyInScatteringWindow )
{
  FRENSIE_CHECK( !distribution->isEnergyInScatteringWindow( 0.1, 9.9999e-6 ) );

  FRENSIE_CHECK( distribution->isEnergyInScatteringWindow( 0.1, 1e-5 ));

  FRENSIE_CHECK( distribution->isEnergyInScatteringWindow( 0.1, 0.1 - 2e-7 ) );

  FRENSIE_CHECK( !distribution->isEnergyInScatteringWindow( 0.1, 0.1 ) );

  FRENSIE_CHECK( !distribution->isEnergyInScatteringWindow( 21.0, 1.0 ) );

  FRENSIE_CHECK( !distribution->isEnergyInScatteringWindow( 21.0, 21.0 ) );

  FRENSIE_CHECK( !distribution->isEnergyInScatteringWindow( 21.0, 22.0 ) );
}

//---------------------------------------------------------------------------//
// Check that the critical line energies in scattering window can be returned
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   getCriticalLineEnergiesInScatteringWindow )
{
  TestBremsstrahlungAdjointElectronScatteringDistribution::LineEnergyIterator
    start_energy, end_energy;

  distribution->getCriticalLineEnergiesInScatteringWindow( 9.9999e-6,
                                                                start_energy,
                                                                end_energy );

  FRENSIE_CHECK_EQUAL( start_energy, end_energy );

  distribution->getCriticalLineEnergiesInScatteringWindow( 1e-5,
                                                                start_energy,
                                                                end_energy );

  FRENSIE_CHECK_EQUAL( *start_energy, 0.08 );
  FRENSIE_CHECK_EQUAL( *end_energy, 21.0 );
  FRENSIE_CHECK_EQUAL( std::distance( start_energy, end_energy ), 4 );

  distribution->getCriticalLineEnergiesInScatteringWindow( 0.18,
                                                                start_energy,
                                                                end_energy );

  FRENSIE_CHECK_EQUAL( *start_energy,
                       Utility::PhysicalConstants::electron_rest_mass_energy );
  FRENSIE_CHECK_EQUAL( *end_energy, 21.0 );
  FRENSIE_CHECK_EQUAL( std::distance( start_energy, end_energy), 3 );

  distribution->getCriticalLineEnergiesInScatteringWindow( 0.21,
                                                                start_energy,
                                                                end_energy );

  FRENSIE_CHECK_EQUAL( *start_energy,
                       Utility::PhysicalConstants::electron_rest_mass_energy );
  FRENSIE_CHECK_EQUAL( *end_energy, 21.0 );
  FRENSIE_CHECK_EQUAL( std::distance( start_energy, end_energy ), 3 );

  distribution->getCriticalLineEnergiesInScatteringWindow( 0.25,
                                                          start_energy,
                                                          end_energy );

  FRENSIE_CHECK_EQUAL( *start_energy,
                       Utility::PhysicalConstants::electron_rest_mass_energy );
  FRENSIE_CHECK_EQUAL( *end_energy, 21.0 );
  FRENSIE_CHECK_EQUAL( std::distance( start_energy, end_energy ), 3 );

  distribution->getCriticalLineEnergiesInScatteringWindow( 0.52,
                                                          start_energy,
                                                          end_energy );

  FRENSIE_CHECK_EQUAL( *start_energy, 1.0 );
  FRENSIE_CHECK_EQUAL( *end_energy, 21.0 );
  FRENSIE_CHECK_EQUAL( std::distance( start_energy, end_energy ), 2 );

  distribution->getCriticalLineEnergiesInScatteringWindow( 1.1,
                                                          start_energy,
                                                          end_energy );

  FRENSIE_CHECK_EQUAL( *start_energy, 5.0 );
  FRENSIE_CHECK_EQUAL( *end_energy, 21.0 );
  FRENSIE_CHECK_EQUAL( std::distance( start_energy, end_energy ), 1 );

  distribution->getCriticalLineEnergiesInScatteringWindow( 5.1,
                                                           start_energy,
                                                           end_energy );

  FRENSIE_CHECK_EQUAL( std::distance( start_energy, end_energy ), 0 );
}

//---------------------------------------------------------------------------//
// Check that the distribution can be evaluated for a given incoming and knock-on energy
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   evaluate )
{
  double pdf;

  // Check below lowest bin
  pdf = distribution->evaluate( 1.0e-6, 2.0e-5 );
  FRENSIE_CHECK_SMALL( pdf, 1e-12 );

  // Check on lowest bin
  pdf = distribution->evaluate( 1.0e-5, 20.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( pdf, 2.177753029805790199e-10, 1e-12 );

  // Check between bins
  pdf = distribution->evaluate( 1.1e-5, 1.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( pdf, 2.571644170655698372e-06, 1e-12 );

  // Check on highest bin
  pdf = distribution->evaluate( max_incoming_energy, 20.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( pdf, 4.593903115800840169e+03, 1e-12 );

  // Check above highest bin
  pdf = distribution->evaluate( 21.0, 22.0 );
  FRENSIE_CHECK_SMALL( pdf, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the PDF can be evaluated for a given incoming and knock-on energy
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   evaluatePDF )
{
  double pdf;

  // Check below lowest bin
  pdf = distribution->evaluatePDF( 1.0e-6, 2.0e-5 );
  FRENSIE_CHECK_SMALL( pdf, 1e-12 );

  // Check on lowest bin
  pdf = distribution->evaluatePDF( 1.0e-5, 20.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( pdf, 1.209507513800998639e-10, 1e-12 );

  // Check between bins
  pdf = distribution->evaluatePDF( 1.1e-5, 1.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( pdf, 1.603637276504853974e-06, 1e-6 );

  // Check on highest bin
  pdf = distribution->evaluatePDF( max_incoming_energy, 20.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( pdf, 3.192847644595705788e+03, 1e-12 );

  // Check above highest bin
  pdf = distribution->evaluatePDF( 21.0, 22.0 );
  FRENSIE_CHECK_SMALL( pdf, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the CDF can be evaluated for a given incoming and knock-on energy
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   evaluateCDF )
{
  double cdf;

  // Check below lowest bin
  cdf = distribution->evaluateCDF( 1.0e-6, 2.0e-5 );
  FRENSIE_CHECK_SMALL( cdf, 1e-12 );

  // Check on lowest bin
  cdf = distribution->evaluateCDF( 1.0e-5, 10.1000050505 );
  FRENSIE_CHECK_FLOATING_EQUALITY( cdf, 9.999999423197977189e-01, 1e-12 );

  // Check in between bins
  cdf = distribution->evaluateCDF( 1.1e-5, 1.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( cdf, 9.9999767762830605e-01, 1e-6 );

  // Check on highest bin
  cdf = distribution->evaluateCDF( max_incoming_energy, 20.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( cdf, 1.0, 1e-12 );

  // Check above highest bin
  cdf = distribution->evaluateCDF( 21.0, 22.0 );
  FRENSIE_CHECK_SMALL( cdf, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that a bremsstrahlung photon can be sampled using detailed 2BS
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   sample )
{
  // Set up the random number stream
  std::vector<double> fake_stream( 2 );
  fake_stream[0] = 0.5; // Correlated sample the 1e-5 MeV and 1.1192e-5 MeV distributions
  fake_stream[1] = 0.5; // Sample an E_out of 1.292132338527119526e-05

  Utility::RandomNumberGenerator::setFakeStream( fake_stream );

  double outgoing_energy, scattering_angle, incoming_energy = 1.1e-5;

  distribution->sample( incoming_energy, outgoing_energy, scattering_angle );

  Utility::RandomNumberGenerator::unsetFakeStream();

  FRENSIE_CHECK_FLOATING_EQUALITY( outgoing_energy, 1.292132338527119526e-05, 1e-12 );
  FRENSIE_CHECK_FLOATING_EQUALITY( scattering_angle, 1.0, 1e-12 );
}

//---------------------------------------------------------------------------//
/* Check that a bremsstrahlung photon can be sampled and the trial number
 * recorded using detailed 2BS
 */
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   sampleAndRecordTrials )
{
  // Set up the random number stream
  std::vector<double> fake_stream( 2 );
  fake_stream[0] = 0.5; // Correlated sample the 1e-5 MeV and 1.1192e-5 MeV distributions
  fake_stream[1] = 0.5; // Sample an E_out of 1.292132338527119526e-05

  Utility::RandomNumberGenerator::setFakeStream( fake_stream );

  MonteCarlo::AdjointElectronScatteringDistribution::Counter trials = 0;

  double outgoing_energy, scattering_angle, incoming_energy = 1.1e-5;

  distribution->sampleAndRecordTrials( incoming_energy,
                                       outgoing_energy,
                                       scattering_angle,
                                       trials );

  Utility::RandomNumberGenerator::unsetFakeStream();

  FRENSIE_CHECK_FLOATING_EQUALITY( outgoing_energy, 1.292132338527119526e-05, 1e-12 );
  FRENSIE_CHECK_FLOATING_EQUALITY( scattering_angle, 1.0, 1e-12 );
  FRENSIE_CHECK_EQUAL( trials, 1.0 );
}

//---------------------------------------------------------------------------//
// Check that an electron can undergo adjoint bremsstrahlung scattering
FRENSIE_UNIT_TEST( BremsstrahlungAdjointElectronScatteringDistribution,
                   scatterAdjointElectron )
{
  MonteCarlo::ParticleBank bank;

  MonteCarlo::AdjointElectronState adjoint_electron( 1 );
  adjoint_electron.setEnergy( 1.1e-5 );
  adjoint_electron.setDirection( 0.0, 0.0, 1.0 );

  Data::SubshellType shell_of_interaction;

  // Set up the random number stream
  std::vector<double> fake_stream( 2 );
  fake_stream[0] = 0.5; // Correlated sample the 1e-5 MeV and 1.1192e-5 MeV distributions
  fake_stream[1] = 0.5; // Sample an E_out of 1.292132338527119526e-05

  Utility::RandomNumberGenerator::setFakeStream( fake_stream );

  distribution->scatterAdjointElectron( adjoint_electron,
                                        bank,
                                        shell_of_interaction );

  Utility::RandomNumberGenerator::unsetFakeStream();

  FRENSIE_CHECK_FLOATING_EQUALITY( adjoint_electron.getEnergy(), 1.292132338527119526e-05, 1e-12 );
  FRENSIE_CHECK_SMALL( adjoint_electron.getXDirection(), 1e-12 );
  FRENSIE_CHECK_SMALL( adjoint_electron.getYDirection(), 1e-12 );
  FRENSIE_CHECK_FLOATING_EQUALITY( adjoint_electron.getZDirection(), 1.0, 1e-12 );
  FRENSIE_CHECK_EQUAL( shell_of_interaction, Data::UNKNOWN_SUBSHELL );
  FRENSIE_CHECK_EQUAL( bank.size(), 4 );

  double pdf = distribution->evaluate( 1.1e-05, 0.08 );
  FRENSIE_CHECK_EQUAL( bank.top().getEnergy(), 0.08 );
  FRENSIE_CHECK_FLOATING_EQUALITY( bank.top().getWeight(), pdf, 1e-15 );

  bank.pop();

  pdf = distribution->evaluate(
              1.1e-05, Utility::PhysicalConstants::electron_rest_mass_energy );
  FRENSIE_CHECK_EQUAL( bank.top().getEnergy(),
                       Utility::PhysicalConstants::electron_rest_mass_energy );
  FRENSIE_CHECK_FLOATING_EQUALITY( bank.top().getWeight(), pdf, 1e-15 );

  bank.pop();

  pdf = distribution->evaluate( 1.1e-05, 1.0 );
  FRENSIE_CHECK_EQUAL( bank.top().getEnergy(), 1.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( bank.top().getWeight(), pdf, 1e-15 );

  bank.pop();

  pdf = distribution->evaluate( 1.1e-05, 5.0 );
  FRENSIE_CHECK_EQUAL( bank.top().getEnergy(), 5.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( bank.top().getWeight(), pdf, 1e-15 );

  bank.pop();

  Utility::RandomNumberGenerator::unsetFakeStream();

  // Generate a probe with energy 5.0
  adjoint_electron.setWeight( 1.0 );
  adjoint_electron.setEnergy( 4.9 );
  adjoint_electron.setDirection( 0.0, 0.0, 1.0 );

  distribution->scatterAdjointElectron( adjoint_electron,
                                        bank,
                                        shell_of_interaction );

  FRENSIE_CHECK_EQUAL( bank.size(), 1 );

  pdf = distribution->evaluate( 4.9, 5.0 );
  FRENSIE_CHECK_EQUAL( bank.top().getEnergy(), 5.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( bank.top().getWeight(), pdf, 1e-15 );

  bank.pop();

  // Make sure that probes do not generate more probe particles
  MonteCarlo::AdjointElectronProbeState adjoint_electron_probe( 0 );
  adjoint_electron_probe.setEnergy( 0.3 );
  adjoint_electron_probe.setDirection( 0.0, 0.0, 1.0 );

  distribution->scatterAdjointElectron( adjoint_electron_probe,
                                        bank,
                                        shell_of_interaction );

  FRENSIE_CHECK_EQUAL( bank.size(), 0 );
}

//---------------------------------------------------------------------------//
// Custom setup
//---------------------------------------------------------------------------//
FRENSIE_CUSTOM_UNIT_TEST_SETUP_BEGIN();

std::string test_native_file_name;

FRENSIE_CUSTOM_UNIT_TEST_COMMAND_LINE_OPTIONS()
{
  ADD_STANDARD_OPTION_AND_ASSIGN_VALUE( "test_native_file",
                                        test_native_file_name, "",
                                        "Test Native file name" );
}

FRENSIE_CUSTOM_UNIT_TEST_INIT()
{
  // Create the native data file container
  std::shared_ptr<Data::AdjointElectronPhotonRelaxationDataContainer>
    data_container( new Data::AdjointElectronPhotonRelaxationDataContainer(
        test_native_file_name ) );

  // Get the energy grid
  std::vector<double> energy_grid;

  if ( data_container->separateAdjointBremsstrahlungEnergyGrid() )
    energy_grid = data_container->getAdjointElectronBremsstrahlungEnergyGrid();
  else
    energy_grid = data_container->getAdjointElectronEnergyGrid();

  // Get the function data
  size_t size = energy_grid.size();
  std::vector<double> primary_grid( size );
  std::vector<std::shared_ptr<const Utility::TabularUnivariateDistribution> >
    secondary_dists( size );

  for( unsigned n = 0; n < size; ++n )
  {
    primary_grid[n] = energy_grid[n];

    // Get the outgoing energy of the adjoint bremsstrahlung electron at the incoming energy
    std::vector<double> outgoing_energy(
      data_container->getAdjointElectronBremsstrahlungEnergy( energy_grid[n] ) );

    // Get the adjoint bremsstrahlung pdf at the incoming energy
    std::vector<double> pdf(
      data_container->getAdjointElectronBremsstrahlungPDF( energy_grid[n] ) );

    secondary_dists[n].reset(
      new const Utility::TabularDistribution<Utility::LinLin>( outgoing_energy,
                                                               pdf ) );
  }

  double evaluation_tol = 1e-7;

  // Create the energy gain function
  std::shared_ptr<Utility::FullyTabularBasicBivariateDistribution> energy_gain_function(
     new Utility::InterpolatedFullyTabularBasicBivariateDistribution<Utility::UnitBaseCorrelated<Utility::LogLogLog> >(
                                                            primary_grid,
                                                            secondary_dists,
                                                            1e-6,
                                                            evaluation_tol ) );

  // Create the scattering distribution
  distribution.reset(
    new TestBremsstrahlungAdjointElectronScatteringDistribution(
        energy_gain_function ) );

  // Create the scattering distribution
  std::shared_ptr<std::vector<double> >
    critical_line_energies( new std::vector<double>( 5 ) );

  (*critical_line_energies)[0] = 0.08;
  (*critical_line_energies)[1] =
    Utility::PhysicalConstants::electron_rest_mass_energy;
  (*critical_line_energies)[2] = 1.0;
  (*critical_line_energies)[3] = 5.0;
  (*critical_line_energies)[4] = 21.0;

  distribution->setCriticalLineEnergies( critical_line_energies );

  // Initialize the random number generator
  Utility::RandomNumberGenerator::createStreams();
}

FRENSIE_CUSTOM_UNIT_TEST_SETUP_END();

//---------------------------------------------------------------------------//
// end tstBremsstrahlungAdjointElectronScatteringDistribution.cpp
//---------------------------------------------------------------------------//
