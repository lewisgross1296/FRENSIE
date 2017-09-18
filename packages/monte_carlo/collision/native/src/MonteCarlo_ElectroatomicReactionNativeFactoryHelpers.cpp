//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_ElectroatomicReactionNativeFactoryHelpers.cpp
//! \author Luke Kersting
//! \brief  The electroatomic reaction native factory helpers definitions
//!
//---------------------------------------------------------------------------//

// FRENSIE Includes
#include "MonteCarlo_ElectroatomicReactionNativeFactoryHelpers.hpp"

namespace MonteCarlo{

//----------------------------------------------------------------------------//
//      ****ELASTIC REACTIONS****
//----------------------------------------------------------------------------//

//! Create an coupled elastic scattering electroatomic reaction
std::shared_ptr<ElectroatomicReaction>
createCoupledElasticReaction(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const std::string two_d_interp_policy_name,
    const std::string sampling_method,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol )
{
  // Extract the common energy grid
  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.assign( raw_electroatom_data.getElectronEnergyGrid().begin(),
                      raw_electroatom_data.getElectronEnergyGrid().end() );

  // Construct the hash-based grid searcher
  Teuchos::RCP<Utility::HashBasedGridSearcher> grid_searcher(
     new Utility::StandardHashBasedGridSearcher<Teuchos::ArrayRCP<const double>, false>(
                              energy_grid,
                              energy_grid.size()/10 ) );

  // Convert the sampling method
  CoupledElasticSamplingMethod method =
    convertStringToCoupledElasticSamplingMethod( sampling_method );


  // Create the reaction
  std::shared_ptr<ElectroatomicReaction> reaction;

  if ( two_d_interp_policy_name == Utility::LinLinLog::name() )
  {
    ElectroatomicReactionNativeFactory::createCoupledElasticReaction<Utility::LinLinLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        method,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LogLogLog::name() )
  {
    ElectroatomicReactionNativeFactory::createCoupledElasticReaction<Utility::LogLogLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        method,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LinLinLin::name() )
  {
    ElectroatomicReactionNativeFactory::createCoupledElasticReaction<Utility::LinLinLin>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        method,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else
  {
    THROW_EXCEPTION( std::runtime_error,
                     "Error: the TwoDInterpPolicy " <<
                     two_d_interp_policy_name <<
                     " is invalid or currently not supported!" );
  }

  // Make sure the reaction was created correctly
  testPostcondition( reaction.use_count() > 0 );

  return reaction;
}

//! Create a decoupled elastic scattering electroatomic reaction
std::shared_ptr<ElectroatomicReaction>
createDecoupledElasticReaction(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol )
{
  // Extract the common energy grid
  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.assign( raw_electroatom_data.getElectronEnergyGrid().begin(),
                      raw_electroatom_data.getElectronEnergyGrid().end() );

  // Construct the hash-based grid searcher
  Teuchos::RCP<Utility::HashBasedGridSearcher> grid_searcher(
     new Utility::StandardHashBasedGridSearcher<Teuchos::ArrayRCP<const double>, false>(
                              energy_grid,
                              energy_grid.size()/10 ) );

  // Create the reaction
  std::shared_ptr<ElectroatomicReaction> reaction;

  if ( two_d_interp_policy_name == Utility::LinLinLog::name() )
  {
    ElectroatomicReactionNativeFactory::createDecoupledElasticReaction<Utility::LinLinLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LogLogLog::name() )
  {
    ElectroatomicReactionNativeFactory::createDecoupledElasticReaction<Utility::LogLogLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LinLinLin::name() )
  {
    ElectroatomicReactionNativeFactory::createDecoupledElasticReaction<Utility::LinLinLin>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else
  {
    THROW_EXCEPTION( std::runtime_error,
                     "Error: the TwoDInterpPolicy " <<
                     two_d_interp_policy_name <<
                     " is invalid or currently not supported!" );
  }

  // Make sure the reaction was created correctly
  testPostcondition( reaction.use_count() > 0 );

  return reaction;
}

//! Create a hybrid elastic scattering electroatomic reaction
std::shared_ptr<ElectroatomicReaction>
createHybridElasticReaction(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const double cutoff_angle_cosine,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol )
{
  // Extract the common energy grid
  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.assign( raw_electroatom_data.getElectronEnergyGrid().begin(),
                      raw_electroatom_data.getElectronEnergyGrid().end() );

  // Construct the hash-based grid searcher
  Teuchos::RCP<Utility::HashBasedGridSearcher> grid_searcher(
     new Utility::StandardHashBasedGridSearcher<Teuchos::ArrayRCP<const double>, false>(
                              energy_grid,
                              energy_grid.size()/10 ) );

  // Create the reaction
  std::shared_ptr<ElectroatomicReaction> reaction;

  if ( two_d_interp_policy_name == Utility::LinLinLog::name() )
  {
    ElectroatomicReactionNativeFactory::createHybridElasticReaction<Utility::LinLinLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        cutoff_angle_cosine,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LogLogLog::name() )
  {
    ElectroatomicReactionNativeFactory::createHybridElasticReaction<Utility::LogLogLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        cutoff_angle_cosine,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LinLinLin::name() )
  {
    ElectroatomicReactionNativeFactory::createHybridElasticReaction<Utility::LinLinLin>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        cutoff_angle_cosine,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else
  {
    THROW_EXCEPTION( std::runtime_error,
                     "Error: the TwoDInterpPolicy " <<
                     two_d_interp_policy_name <<
                     " is invalid or currently not supported!" );
  }

  // Make sure the reaction was created correctly
  testPostcondition( reaction.use_count() > 0 );

  return reaction;
}

//! Create an cutoff elastic scattering electroatomic reaction
std::shared_ptr<ElectroatomicReaction>
createCutoffElasticReaction(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const double cutoff_angle_cosine,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol )
{
  // Extract the common energy grid
  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.assign( raw_electroatom_data.getElectronEnergyGrid().begin(),
                      raw_electroatom_data.getElectronEnergyGrid().end() );

  // Construct the hash-based grid searcher
  Teuchos::RCP<Utility::HashBasedGridSearcher> grid_searcher(
     new Utility::StandardHashBasedGridSearcher<Teuchos::ArrayRCP<const double>, false>(
                              energy_grid,
                              energy_grid.size()/10 ) );

  // Create the reaction
  std::shared_ptr<ElectroatomicReaction> reaction;

  if ( two_d_interp_policy_name == Utility::LinLinLog::name() )
  {
    ElectroatomicReactionNativeFactory::createCutoffElasticReaction<Utility::LinLinLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        cutoff_angle_cosine,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LogLogLog::name() )
  {
    ElectroatomicReactionNativeFactory::createCutoffElasticReaction<Utility::LogLogLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        cutoff_angle_cosine,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LinLinLin::name() )
  {
    ElectroatomicReactionNativeFactory::createCutoffElasticReaction<Utility::LinLinLin>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        cutoff_angle_cosine,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else
  {
    THROW_EXCEPTION( std::runtime_error,
                     "Error: the TwoDInterpPolicy " <<
                     two_d_interp_policy_name <<
                     " is invalid or currently not supported!" );
  }

  // Make sure the reaction was created correctly
  testPostcondition( reaction.use_count() > 0 );

  return reaction;
}

//! Create a screened Rutherford elastic scattering electroatomic reaction
std::shared_ptr<ElectroatomicReaction>
createScreenedRutherfordElasticReaction(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const double cutoff_angle_cosine,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol )
{
  // Extract the common energy grid
  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.assign( raw_electroatom_data.getElectronEnergyGrid().begin(),
                      raw_electroatom_data.getElectronEnergyGrid().end() );

  // Construct the hash-based grid searcher
  Teuchos::RCP<Utility::HashBasedGridSearcher> grid_searcher(
     new Utility::StandardHashBasedGridSearcher<Teuchos::ArrayRCP<const double>, false>(
                              energy_grid,
                              energy_grid.size()/10 ) );

  // Create the reaction
  std::shared_ptr<ElectroatomicReaction> reaction;

  if ( two_d_interp_policy_name == Utility::LinLinLog::name() )
  {
 ElectroatomicReactionNativeFactory::createScreenedRutherfordElasticReaction<Utility::LinLinLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        cutoff_angle_cosine,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LogLogLog::name() )
  {
 ElectroatomicReactionNativeFactory::createScreenedRutherfordElasticReaction<Utility::LogLogLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        cutoff_angle_cosine,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LinLinLin::name() )
  {
 ElectroatomicReactionNativeFactory::createScreenedRutherfordElasticReaction<Utility::LinLinLin>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        cutoff_angle_cosine,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else
  {
    THROW_EXCEPTION( std::runtime_error,
                     "Error: the TwoDInterpPolicy " <<
                     two_d_interp_policy_name <<
                     " is invalid or currently not supported!" );
  }

  // Make sure the reaction was created correctly
  testPostcondition( reaction.use_count() > 0 );

  return reaction;
}

//! Create the moment preserving elastic scattering electroatomic reaction
std::shared_ptr<ElectroatomicReaction>
createMomentPreservingElasticReaction(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const double cutoff_angle_cosine,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol )
{
  // Extract the common energy grid
  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.assign( raw_electroatom_data.getElectronEnergyGrid().begin(),
                      raw_electroatom_data.getElectronEnergyGrid().end() );

  // Construct the hash-based grid searcher
  Teuchos::RCP<Utility::HashBasedGridSearcher> grid_searcher(
     new Utility::StandardHashBasedGridSearcher<Teuchos::ArrayRCP<const double>, false>(
                              energy_grid,
                              energy_grid.size()/10 ) );

  // Create the reaction
  std::shared_ptr<ElectroatomicReaction> reaction;

  if ( two_d_interp_policy_name == Utility::LinLinLog::name() )
  {
    ElectroatomicReactionNativeFactory::createMomentPreservingElasticReaction<Utility::LinLinLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        cutoff_angle_cosine,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LogLogLog::name() )
  {
    ElectroatomicReactionNativeFactory::createMomentPreservingElasticReaction<Utility::LogLogLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        cutoff_angle_cosine,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LinLinLin::name() )
  {
    ElectroatomicReactionNativeFactory::createMomentPreservingElasticReaction<Utility::LinLinLin>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        cutoff_angle_cosine,
        correlated_sampling_mode_on,
        evaluation_tol );
  }
  else
  {
    THROW_EXCEPTION( std::runtime_error,
                     "Error: the TwoDInterpPolicy " <<
                     two_d_interp_policy_name <<
                     " is invalid or currently not supported!" );
  }

  // Make sure the reaction was created correctly
  testPostcondition( reaction.use_count() > 0 );

  return reaction;
}

//----------------------------------------------------------------------------//
//      ****ATOMIC EXCITATION DISTRIBUTION****
//----------------------------------------------------------------------------//

//! Create an atomic excitation scattering electroatomic reaction
std::shared_ptr<ElectroatomicReaction>
createAtomicExcitationReaction(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data )
{
  // Extract the common energy grid
  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.assign( raw_electroatom_data.getElectronEnergyGrid().begin(),
                      raw_electroatom_data.getElectronEnergyGrid().end() );

  // Construct the hash-based grid searcher
  Teuchos::RCP<Utility::HashBasedGridSearcher> grid_searcher(
     new Utility::StandardHashBasedGridSearcher<Teuchos::ArrayRCP<const double>, false>(
                              energy_grid,
                              energy_grid.size()/10 ) );

  // Create the reaction
  std::shared_ptr<ElectroatomicReaction> reaction;

  ElectroatomicReactionNativeFactory::createAtomicExcitationReaction(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction );

  return reaction;
}

//----------------------------------------------------------------------------//
//      ****ELECTROIONIZATION SUBSHELL REACTIONS****
//----------------------------------------------------------------------------//

//! Create the subshell electroionization electroatomic reaction
std::shared_ptr<ElectroatomicReaction>
createSubshellElectroionizationReaction(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const unsigned subshell,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const bool unit_based_interpolation_mode_on,
    const double evaluation_tol )
{
  // Extract the common energy grid
  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.assign( raw_electroatom_data.getElectronEnergyGrid().begin(),
                      raw_electroatom_data.getElectronEnergyGrid().end() );

  // Construct the hash-based grid searcher
  Teuchos::RCP<Utility::HashBasedGridSearcher> grid_searcher(
     new Utility::StandardHashBasedGridSearcher<Teuchos::ArrayRCP<const double>, false>(
                              energy_grid,
                              energy_grid.size()/10 ) );

  // Create the reaction
  std::shared_ptr<ElectroatomicReaction> reaction;

  if ( two_d_interp_policy_name == Utility::LinLinLog::name() )
  {
    ElectroatomicReactionNativeFactory::createSubshellElectroionizationReaction<MonteCarlo::ElectroatomicReaction,Utility::LinLinLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        subshell,
        reaction,
        correlated_sampling_mode_on,
        unit_based_interpolation_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LogLogLog::name() )
  {
    ElectroatomicReactionNativeFactory::createSubshellElectroionizationReaction<MonteCarlo::ElectroatomicReaction,Utility::LogLogLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        subshell,
        reaction,
        correlated_sampling_mode_on,
        unit_based_interpolation_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LinLinLin::name() )
  {
    ElectroatomicReactionNativeFactory::createSubshellElectroionizationReaction<MonteCarlo::ElectroatomicReaction,Utility::LinLinLin>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        subshell,
        reaction,
        correlated_sampling_mode_on,
        unit_based_interpolation_mode_on,
        evaluation_tol );
  }
  else
  {
    THROW_EXCEPTION( std::runtime_error,
                     "Error: the TwoDInterpPolicy " <<
                     two_d_interp_policy_name <<
                     " is invalid or currently not supported!" );
  }

  // Make sure the reaction was created correctly
  testPostcondition( reaction.use_count() > 0 );

  return reaction;
}

//! Create the subshell electroionization electroatomic reactions
std::vector<std::shared_ptr<ElectroatomicReaction> >
createSubshellElectroionizationReactions(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const bool unit_based_interpolation_mode_on,
    const double evaluation_tol )
{
  // Extract the common energy grid
  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.assign( raw_electroatom_data.getElectronEnergyGrid().begin(),
                      raw_electroatom_data.getElectronEnergyGrid().end() );

  // Construct the hash-based grid searcher
  Teuchos::RCP<Utility::HashBasedGridSearcher> grid_searcher(
     new Utility::StandardHashBasedGridSearcher<Teuchos::ArrayRCP<const double>, false>(
                              energy_grid,
                              energy_grid.size()/10 ) );

  // Create the reaction
  std::vector<std::shared_ptr<ElectroatomicReaction> > reactions;

  if ( two_d_interp_policy_name == Utility::LinLinLog::name() )
  {
    ElectroatomicReactionNativeFactory::createSubshellElectroionizationReactions<MonteCarlo::ElectroatomicReaction,Utility::LinLinLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reactions,
        correlated_sampling_mode_on,
        unit_based_interpolation_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LogLogLog::name() )
  {
    ElectroatomicReactionNativeFactory::createSubshellElectroionizationReactions<MonteCarlo::ElectroatomicReaction,Utility::LogLogLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reactions,
        correlated_sampling_mode_on,
        unit_based_interpolation_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LinLinLin::name() )
  {
    ElectroatomicReactionNativeFactory::createSubshellElectroionizationReactions<MonteCarlo::ElectroatomicReaction,Utility::LinLinLin>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reactions,
        correlated_sampling_mode_on,
        unit_based_interpolation_mode_on,
        evaluation_tol );
  }
  else
  {
    THROW_EXCEPTION( std::runtime_error,
                     "Error: the TwoDInterpPolicy " <<
                     two_d_interp_policy_name <<
                     " is invalid or currently not supported!" );
  }

  return reactions;
}

//----------------------------------------------------------------------------//
//      ****BREMSSTRAHLUNG REACTIONS****
//----------------------------------------------------------------------------//

//! Create the bremsstrahlung electroatomic reaction
std::shared_ptr<ElectroatomicReaction>
createBremsstrahlungReaction(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const bool dipole_distribution_mode_on,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const bool unit_based_interpolation_mode_on,
    const double evaluation_tol )
{
  // Extract the common energy grid
  Teuchos::ArrayRCP<double> energy_grid;
  energy_grid.assign( raw_electroatom_data.getElectronEnergyGrid().begin(),
                      raw_electroatom_data.getElectronEnergyGrid().end() );

  // Construct the hash-based grid searcher
  Teuchos::RCP<Utility::HashBasedGridSearcher> grid_searcher(
     new Utility::StandardHashBasedGridSearcher<Teuchos::ArrayRCP<const double>, false>(
                              energy_grid,
                              energy_grid.size()/10 ) );


  BremsstrahlungAngularDistributionType photon_distribution_function;

  if ( dipole_distribution_mode_on )
  {
    photon_distribution_function = DIPOLE_DISTRIBUTION;
  }
  else
  {
    photon_distribution_function = TWOBS_DISTRIBUTION;
  }

  // Create the reaction
  std::shared_ptr<ElectroatomicReaction> reaction;

  if ( two_d_interp_policy_name == Utility::LinLinLog::name() )
  {
    ElectroatomicReactionNativeFactory::createBremsstrahlungReaction<MonteCarlo::ElectroatomicReaction,Utility::LinLinLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        photon_distribution_function,
        correlated_sampling_mode_on,
        unit_based_interpolation_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LogLogLog::name() )
  {
    ElectroatomicReactionNativeFactory::createBremsstrahlungReaction<MonteCarlo::ElectroatomicReaction,Utility::LogLogLog>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        photon_distribution_function,
        correlated_sampling_mode_on,
        unit_based_interpolation_mode_on,
        evaluation_tol );
  }
  else if ( two_d_interp_policy_name == Utility::LinLinLin::name() )
  {
    ElectroatomicReactionNativeFactory::createBremsstrahlungReaction<MonteCarlo::ElectroatomicReaction,Utility::LinLinLin>(
        raw_electroatom_data,
        energy_grid,
        grid_searcher,
        reaction,
        photon_distribution_function,
        correlated_sampling_mode_on,
        unit_based_interpolation_mode_on,
        evaluation_tol );
  }
  else
  {
    THROW_EXCEPTION( std::runtime_error,
                     "Error: the TwoDInterpPolicy " <<
                     two_d_interp_policy_name <<
                     " is invalid or currently not supported!" );
  }

  // Make sure the reaction was created correctly
  testPostcondition( reaction.use_count() > 0 );

  return reaction;
}

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// end MonteCarlo_ElectroatomicReactionNativeFactoryHelpers.cpp
//---------------------------------------------------------------------------//