//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_ElectroatomicReactionNativeFactory_def.hpp
//! \author Luke Kersting
//! \brief  The electroatomic reaction Native data factory template definition
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_ELECTROATOMIC_REACTION_NATIVE_FACTORY_DEF_HPP
#define MONTE_CARLO_ELECTROATOMIC_REACTION_NATIVE_FACTORY_DEF_HPP

// FRENSIE Includes
#include "MonteCarlo_CoupledElasticElectroatomicReaction.hpp"
#include "MonteCarlo_HybridElasticElectroatomicReaction.hpp"
#include "MonteCarlo_DecoupledElasticElectroatomicReaction.hpp"
#include "MonteCarlo_CutoffElasticElectroatomicReaction.hpp"
#include "MonteCarlo_ScreenedRutherfordElasticElectroatomicReaction.hpp"
#include "MonteCarlo_MomentPreservingElasticElectroatomicReaction.hpp"
#include "MonteCarlo_AtomicExcitationElectroatomicReaction.hpp"
#include "MonteCarlo_BremsstrahlungElectroatomicReaction.hpp"
#include "MonteCarlo_ElectroionizationSubshellElectroatomicReaction.hpp"
#include "Data_SubshellType.hpp"
#include "Utility_SortAlgorithms.hpp"
#include "Utility_TabularDistribution.hpp"
#include "Utility_ContractException.hpp"

namespace MonteCarlo{

// Create the coupled elastic scattering electroatomic reactions
template<typename TwoDGridPolicy>
void ElectroatomicReactionNativeFactory::createCoupledElasticReaction(
            const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
            const Teuchos::ArrayRCP<const double>& energy_grid,
            const Teuchos::RCP<const Utility::HashBasedGridSearcher>& grid_searcher,
            std::shared_ptr<ElectroatomicReaction>& elastic_reaction,
            const CoupledElasticSamplingMethod& sampling_method,
            const double evaluation_tol )
{
  // Make sure the energy grid is valid
  testPrecondition( raw_electroatom_data.getElectronEnergyGrid().size() ==
                    energy_grid.size() );
  testPrecondition( Utility::Sort::isSortedAscending( energy_grid.begin(),
                                                      energy_grid.end() ) );

  // Cutoff elastic cross section
  Teuchos::ArrayRCP<double> cutoff_cross_section;
  cutoff_cross_section.assign(
    raw_electroatom_data.getCutoffElasticCrossSection().begin(),
    raw_electroatom_data.getCutoffElasticCrossSection().end() );

  // Total elastic cross section
  Teuchos::ArrayRCP<double> total_cross_section;
  total_cross_section.assign(
    raw_electroatom_data.getTotalElasticCrossSection().begin(),
    raw_electroatom_data.getTotalElasticCrossSection().end() );

  // Create the coupled elastic scattering distribution
  std::shared_ptr<const CoupledElasticElectronScatteringDistribution> distribution;
  ElasticFactory::createCoupledElasticDistribution<TwoDGridPolicy>(
    distribution,
    energy_grid,
    cutoff_cross_section,
    total_cross_section,
    raw_electroatom_data,
    sampling_method,
    evaluation_tol );

  elastic_reaction.reset(
    new CoupledElasticElectroatomicReaction<Utility::LogLog>(
      energy_grid,
      total_cross_section,
      raw_electroatom_data.getTotalElasticCrossSectionThresholdEnergyIndex(),
      grid_searcher,
      distribution ) );
}

// Create the decoupled elastic scattering electroatomic reactions
template<typename TwoDGridPolicy>
void ElectroatomicReactionNativeFactory::createDecoupledElasticReaction(
            const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
            const Teuchos::ArrayRCP<const double>& energy_grid,
            const Teuchos::RCP<const Utility::HashBasedGridSearcher>& grid_searcher,
            std::shared_ptr<ElectroatomicReaction>& elastic_reaction,
            const double evaluation_tol )
{
  // Make sure the energy grid is valid
  testPrecondition( raw_electroatom_data.getElectronEnergyGrid().size() ==
                    energy_grid.size() );
  testPrecondition( Utility::Sort::isSortedAscending( energy_grid.begin(),
                                                      energy_grid.end() ) );

  // Cutoff elastic cross section
  Teuchos::ArrayRCP<double> cutoff_cross_section;
  cutoff_cross_section.assign(
    raw_electroatom_data.getCutoffElasticCrossSection().begin(),
    raw_electroatom_data.getCutoffElasticCrossSection().end() );

  // Total elastic cross section
  Teuchos::ArrayRCP<double> total_cross_section;
  total_cross_section.assign(
    raw_electroatom_data.getTotalElasticCrossSection().begin(),
    raw_electroatom_data.getTotalElasticCrossSection().end() );

  // Total elastic cross section threshold energy bin index
  unsigned threshold_energy_index =
    raw_electroatom_data.getTotalElasticCrossSectionThresholdEnergyIndex();

  // Create the tabular cutoff elastic scattering distribution
  std::shared_ptr<const CutoffElasticElectronScatteringDistribution> tabular_distribution;
  ElasticFactory::createCutoffElasticDistribution<TwoDGridPolicy>(
    tabular_distribution,
    raw_electroatom_data,
    Utility::ElasticElectronTraits::mu_peak,
    evaluation_tol );

  // Create the analytical screened Rutherford elastic scattering distribution
  std::shared_ptr<const ScreenedRutherfordElasticElectronScatteringDistribution> analytical_distribution;
  ElasticFactory::createScreenedRutherfordElasticDistribution(
    analytical_distribution,
    raw_electroatom_data.getAtomicNumber() );

  elastic_reaction.reset(
    new DecoupledElasticElectroatomicReaction<Utility::LogLog>(
                                        energy_grid,
                                        total_cross_section,
                                        cutoff_cross_section,
                                        threshold_energy_index,
                                        grid_searcher,
                                        tabular_distribution,
                                        analytical_distribution ) );
}

// Create a hybrid elastic scattering electroatomic reaction
template<typename TwoDGridPolicy>
void ElectroatomicReactionNativeFactory::createHybridElasticReaction(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<const Utility::HashBasedGridSearcher>& grid_searcher,
    std::shared_ptr<ElectroatomicReaction>& elastic_reaction,
    const double cutoff_angle_cosine,
    const double evaluation_tol )
{
  // Make sure the energy grid is valid
  testPrecondition( raw_electroatom_data.getElectronEnergyGrid().size() ==
                    energy_grid.size() );
  testPrecondition( Utility::Sort::isSortedAscending( energy_grid.begin(),
                                                      energy_grid.end() ) );

  // Cutoff elastic cross section
  Teuchos::ArrayRCP<double> cutoff_cross_section;
  cutoff_cross_section.assign(
    raw_electroatom_data.getCutoffElasticCrossSection().begin(),
    raw_electroatom_data.getCutoffElasticCrossSection().end() );

  // Cutoff elastic cross section threshold energy bin index
  unsigned cutoff_threshold_energy_index =
    raw_electroatom_data.getCutoffElasticCrossSectionThresholdEnergyIndex();

  // Moment preserving elastic cross section
  std::vector<double> moment_preserving_cross_sections;
  unsigned mp_threshold_energy_index;
  ElasticFactory::calculateMomentPreservingCrossSections<TwoDGridPolicy>(
                                moment_preserving_cross_sections,
                                mp_threshold_energy_index,
                                raw_electroatom_data,
                                energy_grid,
                                evaluation_tol );

  Teuchos::ArrayRCP<double> mp_cross_section;
  mp_cross_section.assign(
    moment_preserving_cross_sections.begin(),
    moment_preserving_cross_sections.end() );

  // Create the hybrid elastic scattering distribution
  std::shared_ptr<const HybridElasticElectronScatteringDistribution> distribution;
  std::shared_ptr<const CutoffElasticElectronScatteringDistribution> cutoff_distribution;

  // Create the hybrid elastic scattering distribution
  ElasticFactory::createHybridElasticDistribution<TwoDGridPolicy>(
            distribution,
            energy_grid,
            cutoff_cross_section,
            mp_cross_section,
            raw_electroatom_data,
            cutoff_angle_cosine,
            evaluation_tol );

  // Create the cutoff elastic scattering distribution
  ElasticFactory::createCutoffElasticDistribution<TwoDGridPolicy>(
            cutoff_distribution,
            raw_electroatom_data,
            cutoff_angle_cosine,
            evaluation_tol );

  // Calculate the hybrid cross section
  unsigned hybrid_threshold_energy_index =
    std::min( mp_threshold_energy_index, cutoff_threshold_energy_index );

  unsigned mp_threshold_diff =
    mp_threshold_energy_index - hybrid_threshold_energy_index;
  unsigned cutoff_threshold_diff =
    cutoff_threshold_energy_index - hybrid_threshold_energy_index;

  std::vector<double> combined_cross_section(
                           energy_grid.size() - hybrid_threshold_energy_index );

  for (unsigned i = 0; i < combined_cross_section.size(); ++i )
  {
    double energy = energy_grid[i + hybrid_threshold_energy_index];
    double reduced_cutoff_ratio =
                cutoff_distribution->evaluateCutoffCrossSectionRatio( energy );

    if ( i < mp_threshold_diff )
    {
      combined_cross_section[i] = cutoff_cross_section[i]*reduced_cutoff_ratio;
    }
    else if ( i < cutoff_threshold_diff )
    {
      combined_cross_section[i] = mp_cross_section[i];
    }
    else
    {
      combined_cross_section[i] =
        cutoff_cross_section[i-cutoff_threshold_diff]*reduced_cutoff_ratio +
        mp_cross_section[i-mp_threshold_diff];
    }
  }

  Teuchos::ArrayRCP<double> hybrid_cross_section;
  hybrid_cross_section.assign( combined_cross_section.begin(),
                               combined_cross_section.end() );


  // Create the hybrid elastic reaction
  elastic_reaction.reset(
    new HybridElasticElectroatomicReaction<Utility::LogLog>(
            energy_grid,
            hybrid_cross_section,
            hybrid_threshold_energy_index,
            grid_searcher,
            cutoff_angle_cosine,
            distribution ) );
}

// Create the cutoff elastic scattering electroatomic reactions
template<typename TwoDGridPolicy>
void ElectroatomicReactionNativeFactory::createCutoffElasticReaction(
            const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
            const Teuchos::ArrayRCP<const double>& energy_grid,
            const Teuchos::RCP<const Utility::HashBasedGridSearcher>& grid_searcher,
            std::shared_ptr<ElectroatomicReaction>& elastic_reaction,
            const double cutoff_angle_cosine,
            const double evaluation_tol )
{
  // Make sure the energy grid is valid
  testPrecondition( raw_electroatom_data.getElectronEnergyGrid().size() ==
                    energy_grid.size() );
  testPrecondition( Utility::Sort::isSortedAscending( energy_grid.begin(),
                                                      energy_grid.end() ) );

  // Cutoff elastic cross section
  Teuchos::ArrayRCP<double> elastic_cross_section;
  elastic_cross_section.assign(
    raw_electroatom_data.getCutoffElasticCrossSection().begin(),
    raw_electroatom_data.getCutoffElasticCrossSection().end() );

  // Cutoff elastic cross section threshold energy bin index
  unsigned threshold_energy_index =
    raw_electroatom_data.getCutoffElasticCrossSectionThresholdEnergyIndex();

  // Create the cutoff elastic scattering distribution using the cutoff angle cosine
  std::shared_ptr<const CutoffElasticElectronScatteringDistribution> distribution;
  ElasticFactory::createCutoffElasticDistribution<TwoDGridPolicy>(
    distribution,
    raw_electroatom_data,
    cutoff_angle_cosine,
    evaluation_tol );

  elastic_reaction.reset(
    new CutoffElasticElectroatomicReaction<Utility::LogLog>(
                          energy_grid,
                          elastic_cross_section,
                          threshold_energy_index,
                          grid_searcher,
                          distribution ) );
}

// Create the moment preserving elastic scattering electroatomic reaction
template<typename TwoDGridPolicy>
void ElectroatomicReactionNativeFactory::createMomentPreservingElasticReaction(
            const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
            const Teuchos::ArrayRCP<const double>& energy_grid,
            const Teuchos::RCP<const Utility::HashBasedGridSearcher>& grid_searcher,
            std::shared_ptr<ElectroatomicReaction>& elastic_reaction,
            const double cutoff_angle_cosine,
            const double evaluation_tol )
{
  // Make sure the energy grid is valid
  testPrecondition( raw_electroatom_data.getElectronEnergyGrid().size() ==
                    energy_grid.size() );
  testPrecondition( Utility::Sort::isSortedAscending( energy_grid.begin(),
                                                      energy_grid.end() ) );

  // Create the moment preserving elastic scattering distribution
  std::shared_ptr<const MomentPreservingElasticElectronScatteringDistribution>
    distribution;

  ElasticFactory::createMomentPreservingElasticDistribution<TwoDGridPolicy>(
    distribution,
    raw_electroatom_data,
    cutoff_angle_cosine,
    evaluation_tol );

  // Moment preserving elastic cross section
  std::vector<double> moment_preserving_cross_sections;
  unsigned mp_threshold_energy_index;
  ElasticFactory::calculateMomentPreservingCrossSections<TwoDGridPolicy>(
                                moment_preserving_cross_sections,
                                mp_threshold_energy_index,
                                raw_electroatom_data,
                                energy_grid,
                                evaluation_tol );

  Teuchos::ArrayRCP<double> mp_cross_section;
  mp_cross_section.assign(
    moment_preserving_cross_sections.begin(),
    moment_preserving_cross_sections.end() );

  elastic_reaction.reset(
    new MomentPreservingElasticElectroatomicReaction<Utility::LogLog>(
                          energy_grid,
                          mp_cross_section,
                          mp_threshold_energy_index,
                          grid_searcher,
                          distribution ) );
}

// Create the subshell electroionization electroatomic reactions
template<typename ReactionType, typename TwoDGridPolicy>
void ElectroatomicReactionNativeFactory::createSubshellElectroionizationReaction(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<const Utility::HashBasedGridSearcher>& grid_searcher,
    const unsigned subshell,
    std::shared_ptr<ReactionType>& electroionization_subshell_reaction,
    const double evaluation_tol )
{
  // Convert subshell number to enum
  Data::SubshellType subshell_type =
    Data::convertENDFDesignatorToSubshellEnum( subshell );

  // Electroionization cross section
  Teuchos::ArrayRCP<double> subshell_cross_section;
  subshell_cross_section.assign(
      raw_electroatom_data.getElectroionizationCrossSection( subshell ).begin(),
      raw_electroatom_data.getElectroionizationCrossSection( subshell ).end() );

  // Electroionization cross section threshold energy bin index
  unsigned threshold_energy_index =
      raw_electroatom_data.getElectroionizationCrossSectionThresholdEnergyIndex(
      subshell );

  // The electroionization subshell distribution
  std::shared_ptr<const ElectroionizationSubshellElectronScatteringDistribution>
      electroionization_subshell_distribution;

  // Create the electroionization subshell distribution
  ElectroionizationFactory::createElectroionizationSubshellDistribution<TwoDGridPolicy>(
      raw_electroatom_data,
      subshell,
      raw_electroatom_data.getSubshellBindingEnergy( subshell ),
      electroionization_subshell_distribution,
      evaluation_tol );


  // Create the subshell electroelectric reaction
  electroionization_subshell_reaction.reset(
      new ElectroionizationSubshellElectroatomicReaction<Utility::LogLog>(
              energy_grid,
              subshell_cross_section,
              threshold_energy_index,
              grid_searcher,
              subshell_type,
              electroionization_subshell_distribution ) );
}

// Create the subshell electroionization electroatomic reactions
template<typename ReactionType, typename TwoDGridPolicy>
void ElectroatomicReactionNativeFactory::createSubshellElectroionizationReactions(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<const Utility::HashBasedGridSearcher>& grid_searcher,
    std::vector<std::shared_ptr<ReactionType> >&
    electroionization_subshell_reactions,
    const double evaluation_tol )
{
  electroionization_subshell_reactions.clear();

  // Extract the subshell information
  std::set<unsigned> subshells = raw_electroatom_data.getSubshells();

  std::shared_ptr<ElectroatomicReaction> electroionization_subshell_reaction;

  std::set<unsigned>::iterator shell = subshells.begin();

  for( shell; shell != subshells.end(); ++shell )
  {
    ThisType::createSubshellElectroionizationReaction<ElectroatomicReaction,TwoDGridPolicy>(
      raw_electroatom_data,
      energy_grid,
      grid_searcher,
      *shell,
      electroionization_subshell_reaction,
      evaluation_tol );

    electroionization_subshell_reactions.push_back(
                      electroionization_subshell_reaction );
  }

  // Make sure the subshell electroelectric reactions have been created
  testPostcondition( electroionization_subshell_reactions.size() > 0 );
}

// Create a bremsstrahlung electroatomic reactions
template<typename ReactionType, typename TwoDGridPolicy>
void ElectroatomicReactionNativeFactory::createBremsstrahlungReaction(
    const Data::ElectronPhotonRelaxationDataContainer& raw_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<const Utility::HashBasedGridSearcher>& grid_searcher,
    std::shared_ptr<ReactionType>& bremsstrahlung_reaction,
    BremsstrahlungAngularDistributionType photon_distribution_function,
    const double evaluation_tol )
{
  // Make sure the energy grid is valid
  testPrecondition( raw_electroatom_data.getElectronEnergyGrid().size() ==
                    energy_grid.size() );
  testPrecondition( Utility::Sort::isSortedAscending( energy_grid.begin(),
                                                      energy_grid.end() ) );

  // Bremsstrahlung cross section
  Teuchos::ArrayRCP<double> bremsstrahlung_cross_section;
  bremsstrahlung_cross_section.assign(
       raw_electroatom_data.getBremsstrahlungCrossSection().begin(),
       raw_electroatom_data.getBremsstrahlungCrossSection().end() );

  // Index of first non zero cross section in the energy grid
  unsigned threshold_energy_index =
    raw_electroatom_data.getBremsstrahlungCrossSectionThresholdEnergyIndex();

  // Create bremsstrahlung scattering distribution
  std::shared_ptr<const BremsstrahlungElectronScatteringDistribution>
    bremsstrahlung_distribution;

  if( photon_distribution_function = DIPOLE_DISTRIBUTION )
  {
    BremsstrahlungFactory::createBremsstrahlungDistribution<TwoDGridPolicy>(
      raw_electroatom_data,
      bremsstrahlung_distribution,
      evaluation_tol );

  }
  else if( photon_distribution_function = TABULAR_DISTRIBUTION )
  {
  THROW_EXCEPTION( std::logic_error,
          "Error! The detailed bremsstrahlung reaction has not been implemented");
  }
  else if( photon_distribution_function = TWOBS_DISTRIBUTION )
  {
    BremsstrahlungFactory::createBremsstrahlungDistribution<TwoDGridPolicy>(
      raw_electroatom_data,
      raw_electroatom_data.getAtomicNumber(),
      bremsstrahlung_distribution,
      evaluation_tol );
  }

  // Create the bremsstrahlung reaction
  bremsstrahlung_reaction.reset(
        new BremsstrahlungElectroatomicReaction<Utility::LogLog>(
                        energy_grid,
                        bremsstrahlung_cross_section,
                        threshold_energy_index,
                        grid_searcher,
                        bremsstrahlung_distribution ) );
}

} // end MontCarlo namespace

#endif // end MONTE_CARLO_ELECTROATOMIC_REACTION_NATIVE_FACTORY_DEF_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_ElectroatomicReactionNativeFactory_def.hpp
//---------------------------------------------------------------------------//