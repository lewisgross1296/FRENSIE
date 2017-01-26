//---------------------------------------------------------------------------//
//!
//! \file   MonteCarlo_ElasticElectronScatteringDistributionACEFactory.hpp
//! \author Luke Kersting
//! \brief  The elastic scattering distribution ACE factory declaration
//!
//---------------------------------------------------------------------------//

#ifndef MONTE_CARLO_ELASTIC_SCATTERING_DISTRIBUTION_ACE_FACTORY_HPP
#define MONTE_CARLO_ELASTIC_SCATTERING_DISTRIBUTION_ACE_FACTORY_HPP

// FRENSIE Includes
#include "MonteCarlo_CutoffElasticElectronScatteringDistribution.hpp"
#include "Data_XSSEPRDataExtractor.hpp"

namespace MonteCarlo{

//! The elastic scattering distribution factory class that uses ACE data
class ElasticElectronScatteringDistributionACEFactory
{

public:

  //! Create a cutoff elastic distribution
  static void createCutoffElasticDistribution(
    std::shared_ptr<const CutoffElasticElectronScatteringDistribution>&
        cutoff_elastic_distribution,
    const Data::XSSEPRDataExtractor& raw_electroatom_data,
    const double upper_cutoff_angle_cosine = 1.0 );

protected:

  //! Create the elastic scattering function
  static void createScatteringFunction(
    const Data::XSSEPRDataExtractor& raw_electroatom_data,
    std::shared_ptr<Utility::FullyTabularTwoDDistribution>&
        scattering_function );
};

} // end MonteCarlo namespace

#endif // end MONTE_CARLO_ELASTIC_ELECTRON_SCATTERING_DISTRIBUTION_ACE_FACTORY_HPP

//---------------------------------------------------------------------------//
// end MonteCarlo_ElasticElectronScatteringDistributionACEFactory.hpp
//---------------------------------------------------------------------------//

