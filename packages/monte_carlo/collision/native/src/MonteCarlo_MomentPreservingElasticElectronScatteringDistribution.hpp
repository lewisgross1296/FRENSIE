//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_MomentPreservingElasticElectronScatteringDistribution.hpp
//! \author Luke Kersting
//! \brief  The moment preserving electron elastic scattering distribution base class
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_MOMENT_PRESERVING_ELASTIC_ELECTRON_SCATTERING_DISTRIBUTION_HPP
#define MONTE_CARLO_MOMENT_PRESERVING_ELASTIC_ELECTRON_SCATTERING_DISTRIBUTION_HPP

// Std Lib Includes
#include <limits>

// FRENSIE Includes
#include "MonteCarlo_ElectronState.hpp"
#include "MonteCarlo_ParticleBank.hpp"
#include "MonteCarlo_ElectronScatteringDistribution.hpp"
#include "MonteCarlo_AdjointElectronScatteringDistribution.hpp"
#include "Utility_InterpolatedFullyTabularTwoDDistribution.hpp"




namespace MonteCarlo{

//! The moment preserving scattering distribution base class
class MomentPreservingElasticElectronScatteringDistribution : public ElectronScatteringDistribution,
        public AdjointElectronScatteringDistribution
{

public:

  typedef Utility::FullyTabularTwoDDistribution TwoDDist;

  //! Constructor
  MomentPreservingElasticElectronScatteringDistribution(
        const std::shared_ptr<TwoDDist>& discrete_scattering_distribution,
        const double cutoff_angle_cosine,
        const bool correlated_sampling_mode_on );

  //! Destructor
  virtual ~MomentPreservingElasticElectronScatteringDistribution()
  { /* ... */ }

  //! Evaluate the PDF
  double evaluate( const double incoming_energy,
                   const double scattering_angle_cosine ) const;

  //! Evaluate the distribution
  double evaluatePDF( const double incoming_energy,
                      const double scattering_angle_cosine ) const;

  //! Evaluate the CDF
  double evaluateCDF( const double incoming_energy,
                      const double scattering_angle_cosine ) const;

  //! Sample an outgoing energy and direction from the distribution
  void sample( const double incoming_energy,
               double& outgoing_energy,
               double& scattering_angle_cosine ) const;

  //! Sample an outgoing energy and direction and record the number of trials
  void sampleAndRecordTrials( const double incoming_energy,
                              double& outgoing_energy,
                              double& scattering_angle_cosine,
                              unsigned& trials ) const;

  //! Randomly scatter the electron
  void scatterElectron( MonteCarlo::ElectronState& electron,
                        MonteCarlo::ParticleBank& bank,
                        Data::SubshellType& shell_of_interaction ) const;

  //! Randomly scatter the adjoint electron
  void scatterAdjointElectron( MonteCarlo::AdjointElectronState& adjoint_electron,
                               MonteCarlo::ParticleBank& bank,
                               Data::SubshellType& shell_of_interaction ) const;

protected:

   //! Sample an outgoing direction from the distribution
  void sampleAndRecordTrialsImpl( const double incoming_energy,
                                  double& scattering_angle_cosine,
                                  unsigned& trials ) const;

private:

  // elastic discrete scattering distribution
  std::shared_ptr<TwoDDist> d_discrete_scattering_distribution;

  // The cutoff angle cosine
  double d_cutoff_angle_cosine;

  // The sample function pointer
  std::function<double ( const double )> d_sample_function;
};

} // end MonteCarlo namespace

#endif // end MONTE_CARLO_MOMENT_PRESERVING_ELASTIC_ELECTRON_SCATTERING_DISTRIBUTION_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_ElasticElectronScatteringDistribution.hpp
//---------------------------------------------------------------------------//
