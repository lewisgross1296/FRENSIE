//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_AdjointElectroatomicReactionNativeFactory.hpp
//! \author Luke Kersting
//! \brief  The adjoint electroatomic reaction Native data factory declaration
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_ADJOINT_ELECTROATOMIC_REACTION_NATIVE_FACTORY_HPP
#define MONTE_CARLO_ADJOINT_ELECTROATOMIC_REACTION_NATIVE_FACTORY_HPP

// Std Lib Includes
#include <memory>
#include <vector>

// Trilinos Includes
#include <Teuchos_RCP.hpp>
#include <Teuchos_Array.hpp>

// FRENSIE Includes
#include "MonteCarlo_AdjointElectroatomicReaction.hpp"
#include "MonteCarlo_ElectroatomicReaction.hpp"
#include "Data_AdjointElectronPhotonRelaxationDataContainer.hpp"
#include "Utility_FullyTabularTwoDDistribution.hpp"
#include "Utility_HashBasedGridSearcher.hpp"
#include "Utility_TwoDInterpolationPolicy.hpp"

namespace MonteCarlo{

//! The adjoint electroatomic reaction factory class that uses Native data
class AdjointElectroatomicReactionNativeFactory
{

private:

  // Typedef for this type
  typedef AdjointElectroatomicReactionNativeFactory ThisType;

public:

  //! Create an analog elastic scattering adjoint electroatomic reaction
  template< typename TwoDInterpPolicy = Utility::LinLinLog>
  static void createAnalogElasticReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer&
        raw_adjoint_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<Utility::HashBasedGridSearcher>& grid_searcher,
    std::shared_ptr<AdjointElectroatomicReaction>& elastic_reaction,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol );

  //! Create a hybrid elastic scattering adjoint electroatomic reaction
  template< typename TwoDInterpPolicy = Utility::LinLinLog>
  static void createHybridElasticReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer&
        raw_adjoint_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<Utility::HashBasedGridSearcher>& grid_searcher,
    std::shared_ptr<AdjointElectroatomicReaction>& elastic_reaction,
    const double cutoff_angle_cosine,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol );

  //! Create an cutoff elastic scattering adjoint electroatomic reaction
  template< typename TwoDInterpPolicy = Utility::LinLinLog>
  static void createCutoffElasticReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer&
        raw_adjoint_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<Utility::HashBasedGridSearcher>& grid_searcher,
    std::shared_ptr<AdjointElectroatomicReaction>& elastic_reaction,
    const double cutoff_angle_cosine,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol );

  //! Create a screened Rutherford elastic scattering adjoint electroatomic reaction
  template< typename TwoDInterpPolicy = Utility::LinLinLog>
  static void createScreenedRutherfordElasticReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer&
        raw_adjoint_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<Utility::HashBasedGridSearcher>& grid_searcher,
    std::shared_ptr<AdjointElectroatomicReaction>& elastic_reaction,
    const double cutoff_angle_cosine,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol );

  //! Create the moment preserving elastic scattering adjoint electroatomic reaction
  template< typename TwoDInterpPolicy = Utility::LinLinLog>
  static void createMomentPreservingElasticReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer&
        raw_adjoint_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<Utility::HashBasedGridSearcher>& grid_searcher,
    std::shared_ptr<AdjointElectroatomicReaction>& elastic_reaction,
    const double cutoff_angle_cosine,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol );

  //! Create an atomic excitation scattering adjoint electroatomic reaction
  static void createAtomicExcitationReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer&
        raw_adjoint_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<Utility::HashBasedGridSearcher>& grid_searcher,
    std::shared_ptr<AdjointElectroatomicReaction>& atomic_excitation_reaction );

  //! Create the subshell electroionization adjoint electroatomic reaction
  template< typename TwoDInterpPolicy = Utility::LinLinLog>
  static void createSubshellElectroionizationReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer&
        raw_adjoint_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<Utility::HashBasedGridSearcher>& grid_searcher,
    const unsigned subshell,
    std::shared_ptr<AdjointElectroatomicReaction>& electroionization_subshell_reaction,
    const bool correlated_sampling_mode_on,
    const bool unit_based_interpolation_mode_on,
    const double evaluation_tol );

  //! Create the subshell electroionization adjoint electroatomic reactions
  template< typename TwoDInterpPolicy = Utility::LinLinLog>
  static void createSubshellElectroionizationReactions(
    const Data::AdjointElectronPhotonRelaxationDataContainer&
        raw_adjoint_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<Utility::HashBasedGridSearcher>& grid_searcher,
    std::vector<std::shared_ptr<AdjointElectroatomicReaction> >&
        electroionization_subshell_reactions,
    const bool correlated_sampling_mode_on,
    const bool unit_based_interpolation_mode_on,
    const double evaluation_tol );

  //! Create the bremsstrahlung adjoint electroatomic reaction
  template< typename TwoDInterpPolicy = Utility::LinLinLog>
  static void createBremsstrahlungReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer&
        raw_adjoint_electroatom_data,
    const Teuchos::ArrayRCP<const double>& energy_grid,
    const Teuchos::RCP<Utility::HashBasedGridSearcher>& grid_searcher,
    std::shared_ptr<AdjointElectroatomicReaction>& bremsstrahlung_reaction,
    const bool correlated_sampling_mode_on,
    const bool unit_based_interpolation_mode_on,
    const double evaluation_tol );

  //! Create the forward total reaction (only used to get the cross section)
  template<typename ReactionType>
  static void createTotalForwardReaction(
      const Data::AdjointElectronPhotonRelaxationDataContainer&
        raw_adjoint_electroatom_data,
      const Teuchos::ArrayRCP<const double>& energy_grid,
      const Teuchos::RCP<const Utility::HashBasedGridSearcher>& grid_searcher,
      const std::shared_ptr<ReactionType>& elastic_reaction,
      std::shared_ptr<ElectroatomicReaction>& total_forward_reaction );
};

} // end MonteCarlo namespace

//---------------------------------------------------------------------------//
// Template Includes
//---------------------------------------------------------------------------//

#include "MonteCarlo_AdjointElectroatomicReactionNativeFactory_def.hpp"

//---------------------------------------------------------------------------//

#endif // end MONTE_CARLO_ADJOINT_ELECTROATOMIC_REACTION_NATIVE_FACTORY_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_AdjointElectroatomicReactionNativeFactory.hpp
//---------------------------------------------------------------------------//