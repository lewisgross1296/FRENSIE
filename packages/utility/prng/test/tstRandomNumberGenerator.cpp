//---------------------------------------------------------------------------//
//!
//! \file   tstRandomNumberGenerator.cpp
//! \author Alex Robinson
//! \brief  Random number generator class unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <set>

// Trilinos Includes
#include <Teuchos_UnitTestHarness.hpp>
#include <Teuchos_ScalarTraits.hpp>

// FRENSIE Includes
#include "Utility_UnitTestHarnessExtensions.hpp"
#include "Utility_RandomNumberGenerator.hpp"

//---------------------------------------------------------------------------//
// Instantiation Macros.
//---------------------------------------------------------------------------//
#define UNIT_TEST_INSTANTIATION( type, name ) \
  TEUCHOS_UNIT_TEST_TEMPLATE_1_INSTANT( type, name, float ) \
  TEUCHOS_UNIT_TEST_TEMPLATE_1_INSTANT( type, name, double ) 

//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
// Check that the random number generator can be initialized
TEUCHOS_UNIT_TEST_TEMPLATE_1_DECL( RandomNumberGenerator,
				   initialize,
				   ScalarType )
{
  Utility::RandomNumberGenerator::initialize();

  // An exception will be thrown if the initialization failed
  double random_number = 
    Utility::RandomNumberGenerator::getRandomNumber<ScalarType>();
}

UNIT_TEST_INSTANTIATION( RandomNumberGenerator, initialize );

//---------------------------------------------------------------------------//
// Check that a fake stream can be set
TEUCHOS_UNIT_TEST_TEMPLATE_1_DECL( RandomNumberGenerator,
				   setFakeStream,
				   ScalarType )
{
  // Create the fake stream
  std::vector<double> fake_stream( 3 );
  fake_stream[0] = 0.2;
  fake_stream[1] = 0.4;
  fake_stream[2] = 0.6;

  // Set the fake stream
  Utility::RandomNumberGenerator::setFakeStream( fake_stream );

  // Test the the fake stream returns the correct values
  ScalarType random_number = 
    Utility::RandomNumberGenerator::getRandomNumber<ScalarType>();
  TEST_EQUALITY_CONST( random_number, (ScalarType)0.2 );
  
  random_number = 
    Utility::RandomNumberGenerator::getRandomNumber<ScalarType>();
  
  TEST_EQUALITY_CONST( random_number, (ScalarType)0.4 );

  random_number = 
    Utility::RandomNumberGenerator::getRandomNumber<ScalarType>();
  
  TEST_EQUALITY_CONST( random_number, (ScalarType)0.6 );

  // Unset the fake stream
  Utility::RandomNumberGenerator::unsetFakeStream();
}

UNIT_TEST_INSTANTIATION( RandomNumberGenerator, setFakeStream );

//---------------------------------------------------------------------------//
// Check that the random number generator can be initialized to a new history
TEUCHOS_UNIT_TEST_TEMPLATE_1_DECL( RandomNumberGenerator,
				   initialize_history,
				   ScalarType )
{
  // Initialize the generator to a particular history depending on the process
  Utility::RandomNumberGenerator::initialize( 
					Teuchos::GlobalMPISession::getRank() );

  // Generate a random number
  double random_number = 
    Utility::RandomNumberGenerator::getRandomNumber<ScalarType>();

  // Retrieve the random numbers generated by the other processes and store
  // them in an array
  Teuchos::Array<int> all_random_numbers( 
				       Teuchos::GlobalMPISession::getNProc() );
  Teuchos::GlobalMPISession::allGather( static_cast<int>( random_number ), 
					all_random_numbers() );
  
  // Store all of the array elements in a set
  std::set<int> random_set;

  for( int i = 0; i < all_random_numbers.size(); ++i )
    random_set.insert( all_random_numbers[i] );

  TEST_EQUALITY( all_random_numbers.size(), random_set.size() );
}

UNIT_TEST_INSTANTIATION( RandomNumberGenerator, initialize_history );

//---------------------------------------------------------------------------//
// end tstRandomNumberGenerator.cpp
//---------------------------------------------------------------------------//

