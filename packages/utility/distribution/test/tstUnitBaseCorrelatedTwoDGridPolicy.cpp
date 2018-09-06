//---------------------------------------------------------------------------//
//!
//! \file   tstCorrelatedTwoDGridPolicy.cpp
//! \author Luke Kersting
//! \brief  The Unit-base Correlated two-dimensional sampling policy unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <sstream>
#include <memory>

// Boost Includes
#include <boost/units/systems/cgs.hpp>
#include <boost/units/io.hpp>

// FRENSIE Includes
#include "Utility_TwoDGridPolicy.hpp"
#include "Utility_UnitTestHarnessWithMain.hpp"
#include "Utility_DynamicOutputFormatter.hpp"
#include "Utility_InterpolatedFullyTabularBasicBivariateDistribution.hpp"
#include "Utility_DeltaDistribution.hpp"
#include "Utility_UniformDistribution.hpp"
#include "Utility_ExponentialDistribution.hpp"
#include "Utility_ElectronVoltUnit.hpp"
#include "Utility_BarnUnit.hpp"

using boost::units::quantity;
using Utility::Units::MegaElectronVolt;
using Utility::Units::MeV;
using Utility::Units::Barn;
using Utility::Units::barn;
using Utility::Units::barns;
namespace cgs = boost::units::cgs;

//---------------------------------------------------------------------------//
// Testing Typenames
//---------------------------------------------------------------------------//

using DistributionType = Utility::FullyTabularBasicBivariateDistribution::DistributionType;
using UnitAwareDistributionType = Utility::UnitAwareFullyTabularBasicBivariateDistribution<MegaElectronVolt,cgs::length,Barn>::DistributionType;
using XIndepType = Utility::UnitTraits<MegaElectronVolt>::template GetQuantityType<double>::type;
using YIndepType = Utility::UnitTraits<cgs::length>::template GetQuantityType<double>::type;
using ZDepType = Utility::UnitTraits<Barn>::template GetQuantityType<double>::type;

//---------------------------------------------------------------------------//
// Testing Variables
//---------------------------------------------------------------------------//

std::shared_ptr<Utility::FullyTabularBasicBivariateDistribution::DistributionType> distribution;
std::shared_ptr<Utility::UnitAwareFullyTabularBasicBivariateDistribution<MegaElectronVolt,cgs::length,Barn>::DistributionType> unit_aware_distribution;

std::function<double(const Utility::TabularUnivariateDistribution&)> functor;
std::function<YIndepType(const Utility::UnitAwareTabularUnivariateDistribution<cgs::length,Barn>&)> ua_functor;

std::function<double (double)> min_func, max_func;
std::function<YIndepType(const XIndepType)> ua_min_func, ua_max_func;

Utility::FullyTabularBasicBivariateDistribution::DistributionType::const_iterator
  lower_bin, upper_bin, sampled_bin, start_bin;

Utility::UnitAwareFullyTabularBasicBivariateDistribution<MegaElectronVolt,cgs::length,Barn>::DistributionType::const_iterator
  ua_lower_bin, ua_upper_bin, ua_sampled_bin, ua_start_bin;

//---------------------------------------------------------------------------//
// Testing Functions.
//---------------------------------------------------------------------------//
// Return the sampling functor
std::function<double(const Utility::TabularUnivariateDistribution&)> getFunctor()
{
  // Use this random number to do create the correlated sampling functor
  const double random_number =
    Utility::RandomNumberGenerator::getRandomNumber<double>();

  // Create the sampling functor
  return std::bind<double>(
                    &Utility::TabularUnivariateDistribution::sampleWithRandomNumber,
                    std::placeholders::_1,
                    random_number );
}
// Return the sampling functor
std::function<YIndepType(const Utility::UnitAwareTabularUnivariateDistribution<cgs::length,Barn>&)> getUnitAwareFunctor()
{
  // Use this random number to do create the correlated sampling functor
  const double random_number =
    Utility::RandomNumberGenerator::getRandomNumber<double>();

  // Create the sampling functor
  return std::bind<YIndepType>(
    &Utility::UnitAwareTabularUnivariateDistribution<cgs::length,Barn>::sampleWithRandomNumber,
    std::placeholders::_1,
    random_number );
}

//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
// Check that the distribution is tabular in the primary dimension
FRENSIE_UNIT_TEST( UnitBaseCorrelated,
                   name )
{
  std::string name = Utility::UnitBaseCorrelated<Utility::LinLinLin>::name();
  FRENSIE_CHECK( name == "Unit-base Correlated" );

  name = Utility::UnitBaseCorrelated<Utility::LinLinLin>::TwoDInterpPolicy::name();
  FRENSIE_CHECK( name == "LinLinLin" );
}

//---------------------------------------------------------------------------//
// Check that the Y lower bound can be calculated
FRENSIE_UNIT_TEST( UnitBaseCorrelated, calculateLowerBound )
{
  lower_bin = distribution->begin();
  upper_bin = lower_bin;
  ++upper_bin;

  // On the first bin boundary
  double x_value = 0.0;
  double bound = Utility::UnitBaseCorrelated<Utility::LinLinLin>::calculateLowerBound<double>(
                    x_value, lower_bin, upper_bin );
  FRENSIE_CHECK_EQUAL( bound, 0.0 );

  // In the first bin
  x_value = 0.5;
  bound = Utility::UnitBaseCorrelated<Utility::LinLinLin>::calculateLowerBound<double>(
                    x_value, lower_bin, upper_bin );
  FRENSIE_CHECK_EQUAL( bound, 1.25 );

  // On the second bin boundary
  ++lower_bin; ++upper_bin;
  x_value = 1.0;
  bound = Utility::UnitBaseCorrelated<Utility::LinLinLin>::calculateLowerBound<double>(
                    x_value, lower_bin, upper_bin );
  FRENSIE_CHECK_EQUAL( bound, 2.5 );

  // In the second bin
  x_value = 1.5;
  bound = Utility::UnitBaseCorrelated<Utility::LinLinLin>::calculateLowerBound<double>(
                    x_value, lower_bin, upper_bin );
  FRENSIE_CHECK_EQUAL( bound, 1.25 );

  // On the upper bin boundary
  x_value = 2.0;
  bound = Utility::UnitBaseCorrelated<Utility::LinLinLin>::calculateLowerBound<double>(
                    x_value, lower_bin, upper_bin );
  FRENSIE_CHECK_EQUAL( bound, 0.0 );
}

//---------------------------------------------------------------------------//
// Check that the Y lower bound can be calculated
FRENSIE_UNIT_TEST( UnitBaseCorrelated, calculateUpperBound )
{
  lower_bin = distribution->begin();
  upper_bin = lower_bin;
  ++upper_bin;

  // On the first bin boundary
  double x_value = 0.0;
  double bound = Utility::UnitBaseCorrelated<Utility::LinLinLin>::calculateUpperBound<double>(
                    x_value, lower_bin, upper_bin );
  FRENSIE_CHECK_EQUAL( bound, 10.0 );

  // In the first bin
  x_value = 0.5;
  bound = Utility::UnitBaseCorrelated<Utility::LinLinLin>::calculateUpperBound<double>(
                    x_value, lower_bin, upper_bin );
  FRENSIE_CHECK_EQUAL( bound, 8.75 );

  // On the second bin boundary
  ++lower_bin; ++upper_bin;
  x_value = 1.0;
  bound = Utility::UnitBaseCorrelated<Utility::LinLinLin>::calculateUpperBound<double>(
                    x_value, lower_bin, upper_bin );
  FRENSIE_CHECK_EQUAL( bound, 7.5 );

  // In the second bin
  x_value = 1.5;
  bound = Utility::UnitBaseCorrelated<Utility::LinLinLin>::calculateUpperBound<double>(
                    x_value, lower_bin, upper_bin );
  FRENSIE_CHECK_EQUAL( bound, 8.75 );

  // On the upper bin boundary
  x_value = 2.0;
  bound = Utility::UnitBaseCorrelated<Utility::LinLinLin>::calculateUpperBound<double>(
                    x_value, lower_bin, upper_bin );
  FRENSIE_CHECK_EQUAL( bound, 10.0 );
}

//---------------------------------------------------------------------------//
// Check that the distribution can be evaluated
FRENSIE_UNIT_TEST( UnitBaseCorrelated, evaluatePDF )
{
  std::function<double(double,double)> evaluate =
  [&min_func, &max_func, &lower_bin, &upper_bin](double x_value, double y_value)
  {
    return Utility::UnitBaseCorrelated<Utility::LinLinLin>::evaluatePDF<Utility::TabularUnivariateDistribution,double,double,double>(
      x_value, y_value, min_func, max_func, &Utility::TabularUnivariateDistribution::evaluate, lower_bin, upper_bin, 1e-3, 1e-15 );
  };

  lower_bin = distribution->begin();
  upper_bin = lower_bin;
  ++upper_bin;

  double x_value = 0.0;
  min_func = [](double x){return 0.0;}; max_func = [](double x){return 10.0;};

  // On the first bin boundary
  FRENSIE_CHECK_EQUAL( evaluate( 0.0, -1.0 ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0, 0.0 ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0, 5.0 ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0, 10.0 ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0, 11.0 ), 0.0 );

  // In the first bin
  min_func = [](double x){return 1.25;}; max_func = [](double x){return 8.75;};

  FRENSIE_CHECK_EQUAL( evaluate( 0.5, 1.0 ), 0.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 0.5, 1.25 ), 1.2698412698412698e-01, 1e-15 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 0.5, 5.0 ), 8.7057683953223552e-01, 1e-12 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 0.5, 8.75 ), 5.3333333333333333e-01, 1e-15 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.5, 9.0 ), 0.0 );

  // On the second bin boundary
  ++lower_bin; ++upper_bin;
  min_func = [](double x){return 2.5;}; max_func = [](double x){return 7.5;};

  FRENSIE_CHECK_EQUAL( evaluate( 1.0, 2.0 ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0, 2.5 ), 0.1 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0, 5.0 ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0, 7.5 ), 0.5 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0, 8.0 ), 0.0 );

  // In the second bin
  min_func = [](double x){return 1.25;}; max_func = [](double x){return 8.75;};

  FRENSIE_CHECK_EQUAL( evaluate( 1.5, 1.0 ), 0.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 1.5, 1.25 ), 0.1, 1e-15 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 1.5, 5.0 ), 2.2105975814721399e-01, 1e-12 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 1.5, 8.75 ), 1.9047619047619047e-01, 1e-15 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.5, 9.0 ), 0.0 );

  // On the upper bin boundary
  min_func = [](double x){return 0.0;}; max_func = [](double x){return 10.0;};

  FRENSIE_CHECK_EQUAL( evaluate( 2.0, -1.0 ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0, 0.0 ), 0.1 );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0, 5.0 ), 0.1 );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0, 10.0 ), 0.1 );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0, 11.0 ), 0.0 );
}

//---------------------------------------------------------------------------//
// Check that the unit-aware distribution can be evaluated
FRENSIE_UNIT_TEST( UnitAwareCorrelated, evaluatePDF )
{
  std::function<ZDepType(XIndepType,YIndepType)> evaluate =
  [&ua_min_func, &ua_max_func, &ua_lower_bin, &ua_upper_bin](XIndepType x_value, YIndepType y_value)
  {
    return Utility::UnitBaseCorrelated<Utility::LinLinLin>::evaluatePDF<Utility::UnitAwareTabularUnivariateDistribution<cgs::length,Barn>,XIndepType,YIndepType,ZDepType>(
      x_value, y_value, ua_min_func, ua_max_func, &Utility::UnitAwareTabularUnivariateDistribution<cgs::length,Barn>::evaluate, ua_lower_bin, ua_upper_bin, 1e-3, 1e-15 );
  };

  ua_lower_bin = unit_aware_distribution->begin();
  ua_upper_bin = ua_lower_bin;
  ++ua_upper_bin;

  ua_min_func = [](XIndepType x){return 0.0*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 10.0*cgs::centimeter;};

  // On the first bin boundary
  FRENSIE_CHECK_EQUAL( evaluate( 0.0*MeV, -1.0*cgs::centimeter ), 0.0*barn );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0*MeV, 0.0*cgs::centimeter ), 1.0*barn );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0*MeV, 5.0*cgs::centimeter ), 1.0*barn );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0*MeV, 10.0*cgs::centimeter ), 1.0*barn );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0*MeV, 11.0*cgs::centimeter ), 0.0*barn );

  // In the first bin
  ua_min_func = [](XIndepType x){return 1.25*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 8.75*cgs::centimeter;};

  FRENSIE_CHECK_EQUAL( evaluate( 0.5*MeV, 1.0*cgs::centimeter ), 0.0*barn );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 0.5*MeV, 1.25*cgs::centimeter ),
                                  1.2698412698412698e-01*barn,
                                  1e-15 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 0.5*MeV, 5.0*cgs::centimeter ),
                                  8.7057683953223552e-01*barn,
                                  1e-12 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 0.5*MeV, 8.75*cgs::centimeter ),
                                  5.3333333333333333e-01*barn,
                                  1e-15 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.5*MeV, 9.0*cgs::centimeter ), 0.0*barn );

  // On the second bin boundary
  ++ua_lower_bin; ++ua_upper_bin;
  ua_min_func = [](XIndepType x){return 2.5*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 7.5*cgs::centimeter;};

  FRENSIE_CHECK_EQUAL( evaluate( 1.0*MeV, 2.0*cgs::centimeter ), 0.0*barn );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0*MeV, 2.5*cgs::centimeter ), 0.1*barn );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0*MeV, 5.0*cgs::centimeter ), 1.0*barn );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0*MeV, 7.5*cgs::centimeter ), 0.5*barn );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0*MeV, 8.0*cgs::centimeter ), 0.0*barn );

  // In the second bin
  ua_min_func = [](XIndepType x){return 1.25*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 8.75*cgs::centimeter;};

  FRENSIE_CHECK_EQUAL( evaluate( 1.5*MeV, 1.0*cgs::centimeter ), 0.0*barn );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 1.5*MeV, 1.25*cgs::centimeter ),
                                  0.1*barn,
                                  1e-15 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 1.5*MeV, 5.0*cgs::centimeter ),
                                  2.2105975814721399e-01*barn,
                                  1e-12 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 1.5*MeV, 8.75*cgs::centimeter ),
                                  1.9047619047619047e-01*barn,
                                  1e-15 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.5*MeV, 9.0*cgs::centimeter ), 0.0*barn );

  // On the upper bin boundary
  ua_min_func = [](XIndepType x){return 0.0*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 10.0*cgs::centimeter;};

  FRENSIE_CHECK_EQUAL( evaluate( 2.0*MeV, -1.0*cgs::centimeter ), 0.0*barn );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0*MeV, 0.0*cgs::centimeter ), 0.1*barn );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0*MeV, 5.0*cgs::centimeter ), 0.1*barn );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0*MeV, 10.0*cgs::centimeter ), 0.1*barn );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0*MeV, 11.0*cgs::centimeter ), 0.0*barn );
}

//---------------------------------------------------------------------------//
// Check that the distribution can be evaluated
FRENSIE_UNIT_TEST( UnitBaseCorrelated, evaluateCDF )
{
  std::function<double(double,double)> evaluate =
  [&min_func, &max_func, &lower_bin, &upper_bin](double x_value, double y_value)
  {
    return Utility::UnitBaseCorrelated<Utility::LinLinLin>::evaluateCDF<Utility::TabularUnivariateDistribution,double,double>(
      x_value, y_value, min_func, max_func, &Utility::TabularUnivariateDistribution::evaluateCDF, lower_bin, upper_bin, 1e-3, 1e-15 );
  };

  lower_bin = distribution->begin();
  upper_bin = lower_bin;
  ++upper_bin;

  double x_value = 0.0;
  min_func = [](double x){return 0.0;}; max_func = [](double x){return 10.0;};

  // On the first bin boundary
  FRENSIE_CHECK_EQUAL( evaluate( 0.0, -1.0 ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0, 0.0 ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0, 5.0 ), 0.5 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0, 10.0 ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0, 11.0 ), 1.0 );

  // In the first bin
  min_func = [](double x){return 1.25;}; max_func = [](double x){return 8.75;};

  FRENSIE_CHECK_EQUAL( evaluate( 0.5, 1.0 ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.5, 1.25 ), 0.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 0.5, 4.7115384615384617 ), 0.4230769230769231, 1e-12 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.5, 8.75 ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.5, 9.0 ), 1.0 );

  // On the second bin boundary
  ++lower_bin; ++upper_bin;
  min_func = [](double x){return 2.5;}; max_func = [](double x){return 7.5;};

  FRENSIE_CHECK_EQUAL( evaluate( 1.0, 2.0 ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0, 2.5 ), 0.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 1.0, 5.0 ), 0.4230769230769231, 1e-15 );

  FRENSIE_CHECK_EQUAL( evaluate( 1.0, 7.5 ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0, 8.0 ), 1.0 );

  // In the second bin
  min_func = [](double x){return 1.25;}; max_func = [](double x){return 8.75;};

  FRENSIE_CHECK_EQUAL( evaluate( 1.5, 1.0 ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.5, 1.25 ), 0.0);
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 1.5, 4.7115384615384617 ), 0.4230769230769231, 1e-12 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.5, 8.75 ), 1.0);
  FRENSIE_CHECK_EQUAL( evaluate( 1.5, 9.0 ), 1.0 );

  // On the upper bin boundary
  min_func = [](double x){return 0.0;}; max_func = [](double x){return 10.0;};

  FRENSIE_CHECK_EQUAL( evaluate( 2.0, -1.0 ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0, 0.0 ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0, 5.0 ), 0.5 );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0, 10.0 ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0, 11.0 ), 1.0 );
}

//---------------------------------------------------------------------------//
// Check that the unit-aware distribution can be evaluated
FRENSIE_UNIT_TEST( UnitAwareCorrelated, evaluateCDF )
{
  std::function<double(XIndepType,YIndepType)> evaluate =
  [&ua_min_func, &ua_max_func, &ua_lower_bin, &ua_upper_bin](XIndepType x_value, YIndepType y_value)
  {
    return Utility::UnitBaseCorrelated<Utility::LinLinLin>::evaluateCDF<Utility::UnitAwareTabularUnivariateDistribution<cgs::length,Barn>,XIndepType,YIndepType>(
      x_value, y_value, ua_min_func, ua_max_func, &Utility::UnitAwareTabularUnivariateDistribution<cgs::length,Barn>::evaluateCDF, ua_lower_bin, ua_upper_bin, 1e-3, 1e-15 );
  };

  ua_lower_bin = unit_aware_distribution->begin();
  ua_upper_bin = ua_lower_bin;
  ++ua_upper_bin;

  ua_min_func = [](XIndepType x){return 0.0*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 10.0*cgs::centimeter;};

  // On the first bin boundary
  FRENSIE_CHECK_EQUAL( evaluate( 0.0*MeV, -1.0*cgs::centimeter ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0*MeV, 0.0*cgs::centimeter ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0*MeV, 5.0*cgs::centimeter ), 0.5 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0*MeV, 10.0*cgs::centimeter ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.0*MeV, 11.0*cgs::centimeter ), 1.0 );

  // In the first bin
  ua_min_func = [](XIndepType x){return 1.25*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 8.75*cgs::centimeter;};

  FRENSIE_CHECK_EQUAL( evaluate( 0.5*MeV, 1.0*cgs::centimeter ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.5*MeV, 1.25*cgs::centimeter ), 0.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 0.5*MeV, 4.7115384615384617*cgs::centimeter ),
                                  0.4230769230769231,
                                  1e-6 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.5*MeV, 8.75*cgs::centimeter ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 0.5*MeV, 9.0*cgs::centimeter ), 1.0 );

  // On the second bin boundary
  ++ua_lower_bin; ++ua_upper_bin;
  ua_min_func = [](XIndepType x){return 2.5*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 7.5*cgs::centimeter;};

  FRENSIE_CHECK_EQUAL( evaluate( 1.0*MeV, 2.0*cgs::centimeter ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0*MeV, 2.5*cgs::centimeter ), 0.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 1.0*MeV, 5.0*cgs::centimeter ),
                                  0.4230769230769231,
                                  1e-15 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0*MeV, 7.5*cgs::centimeter ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.0*MeV, 8.0*cgs::centimeter ), 1.0 );

  // In the second bin
  ua_min_func = [](XIndepType x){return 1.25*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 8.75*cgs::centimeter;};

  FRENSIE_CHECK_EQUAL( evaluate( 1.5*MeV, 1.0*cgs::centimeter ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.5*MeV, 1.25*cgs::centimeter ), 0.0 );
  FRENSIE_CHECK_FLOATING_EQUALITY( evaluate( 1.5*MeV, 4.7115384615384617*cgs::centimeter ),
                                  0.4230769230769231,
                                  1e-6 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.5*MeV, 8.75*cgs::centimeter ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 1.5*MeV, 9.0*cgs::centimeter ), 1.0 );

  // On the upper bin boundary
  ua_min_func = [](XIndepType x){return 0.0*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 10.0*cgs::centimeter;};

  FRENSIE_CHECK_EQUAL( evaluate( 2.0*MeV, -1.0*cgs::centimeter ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0*MeV, 0.0*cgs::centimeter ), 0.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0*MeV, 5.0*cgs::centimeter ), 0.5 );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0*MeV, 10.0*cgs::centimeter ), 1.0 );
  FRENSIE_CHECK_EQUAL( evaluate( 2.0*MeV, 11.0*cgs::centimeter ), 1.0 );
}

//---------------------------------------------------------------------------//
// Check that a secondary conditional PDF can be sampled
FRENSIE_UNIT_TEST( UnitBaseCorrelated, sample )
{
  // On the first bin
  std::vector<double> fake_stream( 3 );
  fake_stream[0] = 0.0;
  fake_stream[1] = 0.5;
  fake_stream[2] = 1.0-1e-15;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );

  std::function<double(double)> sample_function =
  [&functor, &min_func, &max_func, &lower_bin, &upper_bin](double x_value)
  {
    return Utility::UnitBaseCorrelated<Utility::LinLinLin>::sample<double,double>(
      functor, min_func, max_func, x_value, lower_bin, upper_bin );
  };

  lower_bin = distribution->begin();
  upper_bin = lower_bin;
  ++upper_bin;

  double x_value = 0.0;
  min_func = [](double x){return 0.0;}; max_func = [](double x){return 10.0;};
  functor = getFunctor();

  double sample = sample_function( x_value );
  FRENSIE_CHECK_EQUAL( sample, 0.0 );

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_EQUAL( sample, 5.0 );

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 10.0, 1e-14 );

  // In the first bin
  fake_stream[0] = 0.0;
  fake_stream[1] = 0.4230769230769231;
  fake_stream[2] = 1.0-1e-15;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  x_value = 0.5;
  min_func = [](double x){return 1.25;}; max_func = [](double x){return 8.75;};

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_EQUAL( sample, 1.25 );

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_EQUAL( sample, 4.7115384615384617 );

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 8.75, 1e-14 );

  // On the second bin
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  x_value = 1.0;
  min_func = [](double x){return 2.5;}; max_func = [](double x){return 7.5;};

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_EQUAL( sample, 2.5 );

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 5.0, 1e-15 );

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 7.5, 1e-15 );

  // In the second bin
  ++lower_bin; ++upper_bin;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  x_value = 1.5;
  min_func = [](double x){return 1.25;}; max_func = [](double x){return 8.75;};

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_EQUAL( sample, 1.25 );

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 4.7115384615384617, 1e-15 );

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 8.75, 1e-14 );


  // On the upper bin boundary
  fake_stream[0] = 0.0;
  fake_stream[1] = 0.5;
  fake_stream[2] = 1.0-1e-15;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  x_value = 2.0;
  min_func = [](double x){return 0.0;}; max_func = [](double x){return 10.0;};

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_EQUAL( sample, 0.0 );

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_EQUAL( sample, 5.0 );

  functor = getFunctor();
  sample = sample_function( x_value );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 10.0, 1e-14 );

  Utility::RandomNumberGenerator::unsetFakeStream();
}

//---------------------------------------------------------------------------//
// Check that a unit-aware secondary conditional PDF can be sampled
FRENSIE_UNIT_TEST( UnitAwareCorrelated, sample )
{
  std::function<YIndepType(XIndepType)> sample_function =
  [&ua_functor, &ua_min_func, &ua_max_func, &ua_lower_bin, &ua_upper_bin](XIndepType x_value)
  {
    return Utility::UnitBaseCorrelated<Utility::LinLinLin>::sample<XIndepType,YIndepType>(
      ua_functor, ua_min_func, ua_max_func, x_value, ua_lower_bin, ua_upper_bin );
  };

  // On the first bin
  std::vector<double> fake_stream( 3 );
  fake_stream[0] = 0.0;
  fake_stream[1] = 0.5;
  fake_stream[2] = 1.0-1e-15;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );

  ua_lower_bin = unit_aware_distribution->begin();
  ua_upper_bin = ua_lower_bin;
  ++ua_upper_bin;
  XIndepType x_value = 0.0*MeV;
  ua_min_func = [](XIndepType x){return 0.0*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 10.0*cgs::centimeter;};

  ua_functor = getUnitAwareFunctor();
  quantity<cgs::length> sample = sample_function( x_value );
  FRENSIE_CHECK_EQUAL( sample, 0.0*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_EQUAL( sample, 5.0*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 10.0*cgs::centimeter, 1e-14 );

  // In the first bin
  fake_stream[0] = 0.0;
  fake_stream[1] = 0.4230769230769231;
  fake_stream[2] = 1.0-1e-15;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  x_value = 0.5*MeV;
  ua_min_func = [](XIndepType x){return 1.25*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 8.75*cgs::centimeter;};

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_EQUAL( sample, 1.25*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_EQUAL( sample, 4.7115384615384617*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 8.75*cgs::centimeter, 1e-14 );

  // On the second bin
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  x_value = 1.0*MeV;
  ua_min_func = [](XIndepType x){return 2.5*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 7.5*cgs::centimeter;};

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_EQUAL( sample, 2.5*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 5.0*cgs::centimeter, 1e-15 );

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 7.5*cgs::centimeter, 1e-15 );

  // In the second bin
  ++ua_lower_bin; ++ua_upper_bin;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  x_value = 1.5*MeV;
  ua_min_func = [](XIndepType x){return 1.25*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 8.75*cgs::centimeter;};

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_EQUAL( sample, 1.25*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 4.7115384615384617*cgs::centimeter, 1e-15 );

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 8.75*cgs::centimeter, 1e-14 );

  // On the upper bin boundary
  fake_stream[0] = 0.0;
  fake_stream[1] = 0.5;
  fake_stream[2] = 1.0-1e-15;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  x_value = 2.0*MeV;
  ua_min_func = [](XIndepType x){return 0.0*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 10.0*cgs::centimeter;};

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_EQUAL( sample, 0.0*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_EQUAL( sample, 5.0*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample = sample_function( x_value);
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 10.0*cgs::centimeter, 1e-14 );

  Utility::RandomNumberGenerator::unsetFakeStream();
}

//---------------------------------------------------------------------------//
// Check that a secondary conditional PDF can be sampled
FRENSIE_UNIT_TEST( UnitBaseCorrelated, sampleDetailed )
{
  double raw_sample;
  std::function<double(double)> sample_function =
  [&functor, &min_func, &max_func, &lower_bin, &upper_bin, &sampled_bin, &raw_sample](double x_value)
  {
    return Utility::UnitBaseCorrelated<Utility::LinLinLin>::sampleDetailed<double,double>(
      functor, min_func, max_func, x_value, lower_bin, upper_bin, sampled_bin, raw_sample );
  };

  // On the first bin
  std::vector<double> fake_stream( 3 );
  fake_stream[0] = 0.0;
  fake_stream[1] = 0.5;
  fake_stream[2] = 1.0-1e-15;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );

  unsigned bin_index;

  start_bin = distribution->begin();
  lower_bin = start_bin;
  upper_bin = lower_bin;
  ++upper_bin;
  min_func = [](double x){return 0.0;}; max_func = [](double x){return 10.0;};

  functor = getFunctor();
  double sample = sample_function( 0.0 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 0u );
  FRENSIE_CHECK_EQUAL( sample, 0.0 );
  FRENSIE_CHECK_EQUAL( raw_sample, 0.0 );

  functor = getFunctor();
  sample = sample_function( 0.0 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 0u );
  FRENSIE_CHECK_EQUAL( sample, 5.0 );
  FRENSIE_CHECK_EQUAL( raw_sample, 5.0 );

  functor = getFunctor();
  sample = sample_function( 0.0 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 0u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 10.0, 1e-14 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 10.0, 1e-14 );

  // In the first bin
  fake_stream[0] = 0.0;
  fake_stream[1] = 0.4230769230769231;
  fake_stream[2] = 1.0-1e-15;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  min_func = [](double x){return 1.25;}; max_func = [](double x){return 8.75;};

  functor = getFunctor();
  sample = sample_function( 0.5 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 0u );
  FRENSIE_CHECK_EQUAL( sample, 1.25 );
  FRENSIE_CHECK_EQUAL( raw_sample, 1.25 );

  functor = getFunctor();
  sample = sample_function( 0.5 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 0u );
  FRENSIE_CHECK_EQUAL( sample, 4.7115384615384617 );
  FRENSIE_CHECK_EQUAL( raw_sample, 4.7115384615384617 );

  functor = getFunctor();
  sample = sample_function( 0.5 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 0u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 8.75, 1e-14 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 8.75, 1e-14 );

  // On the second bin
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  min_func = [](double x){return 2.5;}; max_func = [](double x){return 7.5;};

  functor = getFunctor();
  sample = sample_function( 1.0 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 1u );
  FRENSIE_CHECK_EQUAL( sample, 2.5 );
  FRENSIE_CHECK_EQUAL( raw_sample, 2.5 );

  functor = getFunctor();
  sample = sample_function( 1.0 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 1u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 5.0, 1e-15 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 5.0, 1e-15 );

  functor = getFunctor();
  sample = sample_function( 1.0 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 1u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 7.5, 1e-15 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 7.5, 1e-15 );

  // In the second bin
  ++lower_bin; ++upper_bin;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  min_func = [](double x){return 1.25;}; max_func = [](double x){return 8.75;};

  functor = getFunctor();
  sample = sample_function( 1.5 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 1u );
  FRENSIE_CHECK_EQUAL( sample, 1.25 );
  FRENSIE_CHECK_EQUAL( raw_sample, 1.25 );

  functor = getFunctor();
  sample = sample_function( 1.5 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 1u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 4.7115384615384617, 1e-15 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 4.7115384615384617, 1e-15 );

  functor = getFunctor();
  sample = sample_function( 1.5 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 1u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 8.75, 1e-14 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 8.75, 1e-14 );

  // On the upper bin boundary
  fake_stream[0] = 0.0;
  fake_stream[1] = 0.5;
  fake_stream[2] = 1.0-1e-15;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  min_func = [](double x){return 0.0;}; max_func = [](double x){return 10.0;};

  functor = getFunctor();
  sample = sample_function( 2.0 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 2u );
  FRENSIE_CHECK_EQUAL( sample, 0.0 );
  FRENSIE_CHECK_EQUAL( raw_sample, 0.0 );

  functor = getFunctor();
  sample = sample_function( 2.0 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 2u );
  FRENSIE_CHECK_EQUAL( sample, 5.0 );
  FRENSIE_CHECK_EQUAL( raw_sample, 5.0 );

  functor = getFunctor();
  sample = sample_function( 2.0 );

  bin_index = std::distance( start_bin, sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 2u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 10.0, 1e-14 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 10.0, 1e-14 );

  Utility::RandomNumberGenerator::unsetFakeStream();
}

//---------------------------------------------------------------------------//
// Check that a unit-aware secondary conditional PDF can be sampled
FRENSIE_UNIT_TEST( UnitAwareCorrelated, sampleDetailed )
{
  quantity<cgs::length> raw_sample;
  std::function<YIndepType(XIndepType)> sample_function =
  [&ua_functor, &ua_min_func, &ua_max_func, &ua_lower_bin, &ua_upper_bin, &ua_sampled_bin, &raw_sample](XIndepType x_value)
  {
    return Utility::UnitBaseCorrelated<Utility::LinLinLin>::sampleDetailed<XIndepType,YIndepType>(
      ua_functor, ua_min_func, ua_max_func, x_value, ua_lower_bin, ua_upper_bin, ua_sampled_bin, raw_sample );
  };

  // On the first bin
  std::vector<double> fake_stream( 3 );
  fake_stream[0] = 0.0;
  fake_stream[1] = 0.5;
  fake_stream[2] = 1.0-1e-15;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );

  unsigned bin_index;
  ua_start_bin = unit_aware_distribution->begin();
  ua_lower_bin = ua_start_bin;
  ua_upper_bin = ua_lower_bin;
  ++ua_upper_bin;
  ua_min_func = [](XIndepType x){return 0.0*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 10.0*cgs::centimeter;};

  ua_functor = getUnitAwareFunctor();
  quantity<cgs::length> sample = sample_function( 0.0*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 0u );
  FRENSIE_CHECK_EQUAL( sample, 0.0*cgs::centimeter );
  FRENSIE_CHECK_EQUAL( raw_sample, 0.0*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 0.0*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 0u );
  FRENSIE_CHECK_EQUAL( sample, 5.0*cgs::centimeter );
  FRENSIE_CHECK_EQUAL( raw_sample, 5.0*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 0.0*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 0u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 10.0*cgs::centimeter, 1e-14 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 10.0*cgs::centimeter, 1e-14 );

  // In the first bin
  fake_stream[0] = 0.0;
  fake_stream[1] = 0.4230769230769231;
  fake_stream[2] = 1.0-1e-15;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  ua_min_func = [](XIndepType x){return 1.25*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 8.75*cgs::centimeter;};

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 0.5*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 0u );
  FRENSIE_CHECK_EQUAL( sample, 1.25*cgs::centimeter );
  FRENSIE_CHECK_EQUAL( raw_sample, 1.25*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 0.5*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 0u );
  FRENSIE_CHECK_EQUAL( sample, 4.7115384615384617*cgs::centimeter );
  FRENSIE_CHECK_EQUAL( raw_sample, 4.7115384615384617*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 0.5*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 0u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 8.75*cgs::centimeter, 1e-14 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 8.75*cgs::centimeter, 1e-14 );

  // On the second bin
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  ua_min_func = [](XIndepType x){return 2.5*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 7.5*cgs::centimeter;};

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 1.0*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 1u );
  FRENSIE_CHECK_EQUAL( sample, 2.5*cgs::centimeter );
  FRENSIE_CHECK_EQUAL( raw_sample, 2.5*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 1.0*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 1u );
  FRENSIE_CHECK_EQUAL( sample, 5.0*cgs::centimeter );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 5.0*cgs::centimeter, 1e-16 );

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 1.0*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 1u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 7.5*cgs::centimeter, 1e-15 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 7.5*cgs::centimeter, 1e-15 );

  // In the second bin
  ++ua_lower_bin; ++ua_upper_bin;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  ua_min_func = [](XIndepType x){return 1.25*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 8.75*cgs::centimeter;};

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 1.5*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 1u );
  FRENSIE_CHECK_EQUAL( sample, 1.25*cgs::centimeter );
  FRENSIE_CHECK_EQUAL( raw_sample, 1.25*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 1.5*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 1u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 4.7115384615384617*cgs::centimeter, 1e-15 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 4.7115384615384617*cgs::centimeter, 1e-15 );

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 1.5*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 1u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 8.75*cgs::centimeter, 1e-14 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 8.75*cgs::centimeter, 1e-14 );

  // On the upper bin boundary
  fake_stream[0] = 0.0;
  fake_stream[1] = 0.5;
  fake_stream[2] = 1.0-1e-15;
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );
  ua_min_func = [](XIndepType x){return 0.0*cgs::centimeter;};
  ua_max_func = [](XIndepType x){return 10.0*cgs::centimeter;};

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 2.0*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 2u );
  FRENSIE_CHECK_EQUAL( sample, 0.0*cgs::centimeter );
  FRENSIE_CHECK_EQUAL( raw_sample, 0.0*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 2.0*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 2u );
  FRENSIE_CHECK_EQUAL( sample, 5.0*cgs::centimeter );
  FRENSIE_CHECK_EQUAL( raw_sample, 5.0*cgs::centimeter );

  ua_functor = getUnitAwareFunctor();
  sample =  sample_function( 2.0*MeV );

  bin_index = std::distance( ua_start_bin, ua_sampled_bin );
  FRENSIE_CHECK_EQUAL( bin_index, 2u );
  FRENSIE_CHECK_FLOATING_EQUALITY( sample, 10.0*cgs::centimeter, 1e-14 );
  FRENSIE_CHECK_FLOATING_EQUALITY( raw_sample, 10.0*cgs::centimeter, 1e-14 );

  Utility::RandomNumberGenerator::unsetFakeStream();
}

//---------------------------------------------------------------------------//
// Custom setup
//---------------------------------------------------------------------------//
FRENSIE_CUSTOM_UNIT_TEST_SETUP_BEGIN();

FRENSIE_CUSTOM_UNIT_TEST_INIT()
{
  // Create the two-dimensional distribution
  {
    Utility::FullyTabularBasicBivariateDistribution::DistributionType
      distribution_data( 3 );

    // Create the secondary distribution in the first bin
    distribution_data[0].first = 0.0;
    distribution_data[0].second.reset( new Utility::UniformDistribution( 0.0, 10.0, 1.0 ) );

    // Create the secondary distribution in the second bin
    std::vector<double> bin_boundaries( 3 ), values( 3 );
    bin_boundaries[0] = 2.5; values[0] = 0.1;
    bin_boundaries[1] = 5.0; values[1] = 1.0;
    bin_boundaries[2] = 7.5; values[2] = 0.5;

    distribution_data[1].first = 1.0;
    distribution_data[1].second.reset( new Utility::TabularDistribution<Utility::LinLin>( bin_boundaries, values ) );

    // Create the secondary distribution beyond the second bin
    distribution_data[2].first = 2.0;
    distribution_data[2].second.reset( new Utility::UniformDistribution( 0.0, 10.0, 0.1 ) );

    distribution.reset( new Utility::FullyTabularBasicBivariateDistribution::DistributionType(
                                                        distribution_data ) );
  }

  // Create the unit-aware two-dimensional distribution
  {
    Utility::UnitAwareFullyTabularBasicBivariateDistribution<MegaElectronVolt,cgs::length,Barn>::DistributionType
    distribution_data( 3 );

    // Create the secondary distribution in the first bin
    distribution_data[0].first = 0.0*MeV;
    distribution_data[0].second.reset( new Utility::UnitAwareUniformDistribution<cgs::length,Barn>( 0.0*cgs::centimeter, 10.0*cgs::centimeter, 1.0*barn ) );

    // Create the secondary distribution in the second bin
    Teuchos::Array<quantity<cgs::length> > bin_boundaries( 3 );
    Teuchos::Array<quantity<Barn> > values( 3 );
    bin_boundaries[0] = 2.5*cgs::centimeter; values[0] = 0.1*barn;
    bin_boundaries[1] = 5.0*cgs::centimeter; values[1] = 1.0*barn;
    bin_boundaries[2] = 7.5*cgs::centimeter; values[2] = 0.5*barn;

    distribution_data[1].first = 1.0*MeV;
    distribution_data[1].second.reset( new Utility::UnitAwareTabularDistribution<Utility::LinLin,cgs::length,Barn>( bin_boundaries, values ) );

    // Create the secondary distribution beyond the second bin
    distribution_data[2].first = 2.0*MeV;
    distribution_data[2].second.reset( new Utility::UnitAwareUniformDistribution<cgs::length,Barn>( 0.0*cgs::centimeter, 10.0*cgs::centimeter, 0.1*barn ) );

    unit_aware_distribution.reset(
        new Utility::UnitAwareFullyTabularBasicBivariateDistribution<MegaElectronVolt,cgs::length,Barn>::DistributionType(
                                                        distribution_data ) );
  }

  // Initialize the random number generator
  Utility::RandomNumberGenerator::createStreams();
}

FRENSIE_CUSTOM_UNIT_TEST_SETUP_END();

//---------------------------------------------------------------------------//
// end tstCorrelatedTwoDGridPolicy.cpp
//---------------------------------------------------------------------------//
