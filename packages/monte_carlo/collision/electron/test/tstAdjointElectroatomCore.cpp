//---------------------------------------------------------------------------//
//!
//! \file   tstAdjointElectroatomCore.cpp
//! \author Luke Kersting
//! \brief  AdjointElectroatomCore unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <algorithm>

// FRENSIE Includes
#include "MonteCarlo_AdjointElectroatomCore.hpp"
#include "MonteCarlo_BremsstrahlungAdjointElectroatomicReaction.hpp"
#include "MonteCarlo_BremsstrahlungAdjointElectronScatteringDistributionNativeFactory.hpp"
#include "MonteCarlo_AtomicExcitationAdjointElectroatomicReaction.hpp"
#include "MonteCarlo_AtomicExcitationAdjointElectronScatteringDistributionNativeFactory.hpp"
#include "MonteCarlo_AbsorptionElectroatomicReaction.hpp"
#include "MonteCarlo_AdjointElectroatomicReactionNativeFactory.hpp"
#include "MonteCarlo_AdjointElectronState.hpp"
#include "Data_AdjointElectronPhotonRelaxationDataContainer.hpp"
#include "Utility_TabularDistribution.hpp"
#include "Utility_HistogramDistribution.hpp"
#include "Utility_InterpolationPolicy.hpp"
#include "Utility_PhysicalConstants.hpp"
#include "Utility_UnitTestHarnessWithMain.hpp"

typedef MonteCarlo::AtomicExcitationAdjointElectronScatteringDistributionNativeFactory
            AtomicNativeFactory;
typedef MonteCarlo::BremsstrahlungAdjointElectronScatteringDistributionNativeFactory
            BremsstrahlungNativeFactory;

//---------------------------------------------------------------------------//
// Testing Variables
//---------------------------------------------------------------------------//

std::shared_ptr<MonteCarlo::AdjointElectroatomCore> electroatom_core;

//---------------------------------------------------------------------------//
// Check that the total forward reaction can be returned
FRENSIE_UNIT_TEST( AdjointElectroatomCore, getTotalForwardReaction )
{
  const MonteCarlo::ElectroatomicReaction& total_forward_reaction =
    electroatom_core->getTotalForwardReaction();

  FRENSIE_CHECK_EQUAL( total_forward_reaction.getReactionType(),
                       MonteCarlo::TOTAL_ELECTROATOMIC_REACTION );
  FRENSIE_CHECK_FLOATING_EQUALITY( total_forward_reaction.getThresholdEnergy(),
                          1e-5,
                          1e-15 );

  double cross_section = total_forward_reaction.getCrossSection( 1e-5 );
  FRENSIE_CHECK_FLOATING_EQUALITY( cross_section, 2.97832E+01, 1e-12 );

  cross_section = total_forward_reaction.getCrossSection( 1e-3 );
  FRENSIE_CHECK_FLOATING_EQUALITY( cross_section, 2.0449492968423121e+07, 1e-12 );

  cross_section = total_forward_reaction.getCrossSection( 20.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( cross_section, 1.6467035552999546e+05, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the scattering reactions can be returned
FRENSIE_UNIT_TEST( AdjointElectroatomCore, getScatteringReactions )
{
  const MonteCarlo::AdjointElectroatomCore::ConstReactionMap& scattering_reactions =
    electroatom_core->getScatteringReactions();

  FRENSIE_CHECK_EQUAL( scattering_reactions.size(), 2 );

  const MonteCarlo::AdjointElectroatomicReaction& ae_reaction =
    *(scattering_reactions.find(MonteCarlo::ATOMIC_EXCITATION_ADJOINT_ELECTROATOMIC_REACTION)->second);

  const MonteCarlo::AdjointElectroatomicReaction& b_reaction =
    *(scattering_reactions.find(MonteCarlo::BREMSSTRAHLUNG_ADJOINT_ELECTROATOMIC_REACTION)->second);

  double cross_section = ae_reaction.getCrossSection( 1e-5 ) +
                          b_reaction.getCrossSection( 1e-5 );

  FRENSIE_CHECK_FLOATING_EQUALITY( cross_section,
                          3.9800795006423726e+01 + 6.1243057898416743e+07,
                          1e-12 );

  cross_section = ae_reaction.getCrossSection( 1e-3 ) +
                   b_reaction.getCrossSection( 1e-3 );

  FRENSIE_CHECK_FLOATING_EQUALITY( cross_section,
                          1.4246702389204639e+01 + 1.0551636170350602e+07,
                          1e-12 );

  cross_section = ae_reaction.getCrossSection( 20.0 ) +
                   b_reaction.getCrossSection( 20.0 );

  FRENSIE_CHECK_FLOATING_EQUALITY( cross_section,
                          2.4971444066404619e-01 + 8.1829299836129925e+04,
                          1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the absorption reactions can be returned
FRENSIE_UNIT_TEST( AdjointElectroatomCore, getAbsorptionReactions )
{
  const MonteCarlo::AdjointElectroatomCore::ConstReactionMap& absorption_reactions =
    electroatom_core->getAbsorptionReactions();

  FRENSIE_CHECK_EQUAL( absorption_reactions.size(), 0 );

}

//---------------------------------------------------------------------------//
// Check that the grid searcher can be returned
FRENSIE_UNIT_TEST( AdjointElectroatomCore, getGridSearcher )
{
  const Utility::HashBasedGridSearcher<double>& grid_searcher =
    electroatom_core->getGridSearcher();

  unsigned grid_index = grid_searcher.findLowerBinIndex( 1e-5 );
  FRENSIE_CHECK_EQUAL( grid_index, 0u );

  grid_index = grid_searcher.findLowerBinIndex( 1e-3 );
  FRENSIE_CHECK_EQUAL( grid_index, 53 );

  grid_index = grid_searcher.findLowerBinIndex( 20.0 );
  FRENSIE_CHECK_EQUAL( grid_index, 157 );
}

//---------------------------------------------------------------------------//
// Check if all of the reactions share a common energy grid
FRENSIE_UNIT_TEST( AdjointElectroatomCore, hasSharedEnergyGrid )
{
  FRENSIE_CHECK( electroatom_core->hasSharedEnergyGrid() );
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
  // Create the Native adjoint electroatom core
  {
    // Create the native data file container
    Data::AdjointElectronPhotonRelaxationDataContainer data_container(
        test_native_file_name );

    // Create the atomic excitation, bremsstrahlung cross sections
    std::shared_ptr<const std::vector<double> > energy_grid(
       new std::vector<double>( data_container.getAdjointElectronEnergyGrid() ) );

    // Create the hash-based grid searcher
    std::shared_ptr<Utility::HashBasedGridSearcher<double> > grid_searcher(
       new Utility::StandardHashBasedGridSearcher<std::vector<double>,false>(
                                             energy_grid,
                                             100 ) );

    // Get void reaction
    std::shared_ptr<const std::vector<double> > void_cross_section(
                         new std::vector<double>( energy_grid->size(), 0.0 ) );
    
    std::shared_ptr<const MonteCarlo::ElectroatomicReaction> void_reaction(
     new MonteCarlo::AbsorptionElectroatomicReaction<Utility::LinLin,false>(
                       energy_grid,
                       void_cross_section,
                       0u,
                       grid_searcher,
                       MonteCarlo::COUPLED_ELASTIC_ELECTROATOMIC_REACTION ) );

    // Create the total forward reaction
    std::shared_ptr<const MonteCarlo::ElectroatomicReaction> total_forward_reaction;

    MonteCarlo::AdjointElectroatomicReactionNativeFactory::createTotalForwardReaction(
                                       data_container,
                                       energy_grid,
                                       grid_searcher,
                                       void_reaction,
                                       total_forward_reaction );

    // Atomic Excitation cross section
    std::shared_ptr<const std::vector<double> > ae_cross_section(
       new std::vector<double>( data_container.getAdjointAtomicExcitationCrossSection() ) );

    size_t ae_threshold_index =
        data_container.getAdjointAtomicExcitationCrossSectionThresholdEnergyIndex();

    std::shared_ptr<const MonteCarlo::AtomicExcitationAdjointElectronScatteringDistribution>
        ae_distribution;

    // Create the atomic excitation distribution
    AtomicNativeFactory::createAtomicExcitationDistribution(
        data_container,
        ae_distribution );

    // Create the atomic excitation reaction
    std::shared_ptr<MonteCarlo::AdjointElectroatomicReaction> ae_reaction(
        new MonteCarlo::AtomicExcitationAdjointElectroatomicReaction<Utility::LinLin>(
            energy_grid,
            ae_cross_section,
            ae_threshold_index,
            grid_searcher,
            ae_distribution ) );


    // Bremsstrahlung cross section
    std::shared_ptr<const std::vector<double> > b_cross_section(
       new std::vector<double>( data_container.getAdjointBremsstrahlungElectronCrossSection() ) );

    size_t b_threshold_index =
        data_container.getAdjointBremsstrahlungElectronCrossSectionThresholdEnergyIndex();

    std::shared_ptr<const MonteCarlo::BremsstrahlungAdjointElectronScatteringDistribution>
        b_distribution;

    double evaluation_tol = 1e-7;

    // Create the Bremsstrahlung distribution
    BremsstrahlungNativeFactory::createBremsstrahlungDistribution<Utility::LinLinLog,Utility::UnitBaseCorrelated>(
        data_container,
        data_container.getAdjointElectronEnergyGrid(),
        b_distribution,
        evaluation_tol );

    // Create the bremsstrahlung scattering reaction
    std::shared_ptr<MonteCarlo::AdjointElectroatomicReaction> b_reaction(
            new MonteCarlo::BremsstrahlungAdjointElectroatomicReaction<Utility::LinLin>(
            energy_grid,
            b_cross_section,
            b_threshold_index,
            b_distribution ) );

    // Create the reaction maps
    MonteCarlo::AdjointElectroatomCore::ConstReactionMap scattering_reactions,
      absorption_reactions;

    scattering_reactions[ae_reaction->getReactionType()] = ae_reaction;

    scattering_reactions[b_reaction->getReactionType()] = b_reaction;

    // Create a test  adjoint electroatom core
    electroatom_core.reset( new MonteCarlo::AdjointElectroatomCore(
        grid_searcher,
        std::make_shared<const std::vector<double> >(),
        total_forward_reaction,
        scattering_reactions,
        absorption_reactions,
        MonteCarlo::AdjointElectroatomCore::ConstLineEnergyReactionMap() ) );
  }
}

FRENSIE_CUSTOM_UNIT_TEST_SETUP_END();

//---------------------------------------------------------------------------//
// end tstAdjointElectroatomCore.cpp
//---------------------------------------------------------------------------//
