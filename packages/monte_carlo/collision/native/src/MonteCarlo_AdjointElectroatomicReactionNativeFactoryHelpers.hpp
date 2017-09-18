//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_AdjointElectroatomicReactionNativeFactoryHelpers.hpp
//! \author Luke Kersting
//! \brief  The adjoint electroatomic reaction native factory helpers declaration
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_ADJOINT_ELECTROATOMIC_REACTION_NATIVE_FACTORY_HELPERS_HPP
#define MONTE_CARLO_ADJOINT_ELECTROATOMIC_REACTION_NATIVE_FACTORY_HELPERS_HPP

// FRENSIE Includes
#include "MonteCarlo_AdjointElectroatomicReactionNativeFactory.hpp"
#include "MonteCarlo_CoupledElasticAdjointElectroatomicReaction.hpp"

namespace MonteCarlo{

//----------------------------------------------------------------------------//
//      ****ELASTIC REACTIONS****
//----------------------------------------------------------------------------//

//! Create an coupled elastic scattering adjoint electroatomic reaction
std::shared_ptr<AdjointElectroatomicReaction>
createCoupledElasticReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer& raw_adjoint_electroatom_data,
    const std::string two_d_interp_policy_name,
    const std::string sampling_method,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol );

//! Create a decoupled elastic scattering adjoint electroatomic reaction
std::shared_ptr<AdjointElectroatomicReaction>
createDecoupledElasticReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer& raw_adjoint_electroatom_data,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol );

//! Create a hybrid elastic scattering adjoint electroatomic reaction
std::shared_ptr<AdjointElectroatomicReaction>
createHybridElasticReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer& raw_adjoint_electroatom_data,
    const double cutoff_angle_cosine,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol );

//! Create an cutoff elastic scattering adjoint electroatomic reaction
std::shared_ptr<AdjointElectroatomicReaction>
createCutoffElasticReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer& raw_adjoint_electroatom_data,
    const double cutoff_angle_cosine,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol );

//! Create a screened Rutherford elastic scattering adjoint electroatomic reaction
std::shared_ptr<AdjointElectroatomicReaction>
createScreenedRutherfordElasticReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer& raw_adjoint_electroatom_data,
    const double cutoff_angle_cosine,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol );

//! Create the moment preserving elastic scattering adjoint electroatomic reaction
std::shared_ptr<AdjointElectroatomicReaction>
createMomentPreservingElasticReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer& raw_adjoint_electroatom_data,
    const double cutoff_angle_cosine,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const double evaluation_tol );

//----------------------------------------------------------------------------//
//      ****ATOMIC EXCITATION DISTRIBUTION****
//----------------------------------------------------------------------------//

//! Create an atomic excitation scattering adjoint electroatomic reaction
std::shared_ptr<AdjointElectroatomicReaction>
createAtomicExcitationReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer& raw_adjoint_electroatom_data );

//----------------------------------------------------------------------------//
//      ****ELECTROIONIZATION SUBSHELL REACTIONS****
//----------------------------------------------------------------------------//

//! Create the subshell electroionization adjoint electroatomic reaction
std::shared_ptr<AdjointElectroatomicReaction>
createSubshellElectroionizationReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer& raw_adjoint_electroatom_data,
    const unsigned subshell,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const bool unit_based_interpolation_mode_on,
    const double evaluation_tol );

//! Create the subshell electroionization adjoint electroatomic reactions
std::vector<std::shared_ptr<AdjointElectroatomicReaction> >
createSubshellElectroionizationReactions(
    const Data::AdjointElectronPhotonRelaxationDataContainer& raw_adjoint_electroatom_data,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const bool unit_based_interpolation_mode_on,
    const double evaluation_tol );

//----------------------------------------------------------------------------//
//      ****BREMSSTRAHLUNG REACTIONS****
//----------------------------------------------------------------------------//

//! Create the bremsstrahlung adjoint electroatomic reaction
std::shared_ptr<AdjointElectroatomicReaction>
createBremsstrahlungReaction(
    const Data::AdjointElectronPhotonRelaxationDataContainer& raw_adjoint_electroatom_data,
    const bool dipole_distribution_mode_on,
    const std::string two_d_interp_policy_name,
    const bool correlated_sampling_mode_on,
    const bool unit_based_interpolation_mode_on,
    const double evaluation_tol );

} // end MonteCarlo namespace

#endif // end MONTE_CARLO_ADJOINT_ELECTROATOMIC_REACTION_NATIVE_FACTORY_HELPERS_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_AdjointElectroatomicReactionNativeFactoryHelpers.hpp
//---------------------------------------------------------------------------//