//---------------------------------------------------------------------------//
//!
//! \file   tstSharedParallelDagMC.cpp
//! \author Alex Robinson
//! \brief  DagMC wrapper class shared parallel unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <memory>

// FRENSIE Includes
#include "Geometry_DagMCNavigator.hpp"
#include "Geometry_DagMCModel.hpp"
#include "Utility_GlobalOpenMPSession.hpp"
#include "Utility_Vector.hpp"
#include "Utility_Tuple.hpp"
#include "Utility_UnitTestHarnessWithMain.hpp"
#include "ArchiveTestHelpers.hpp"

//---------------------------------------------------------------------------//
// Testing Types
//---------------------------------------------------------------------------//

namespace cgs = boost::units::cgs;

//---------------------------------------------------------------------------//
// Testing Variables
//---------------------------------------------------------------------------//
std::shared_ptr<const Geometry::DagMCModel> model;

//---------------------------------------------------------------------------//
// Tests
//---------------------------------------------------------------------------//
// Check that a parallel internal ray trace can be done
FRENSIE_UNIT_TEST( DagMC, parallel_ray_trace )
{
  std::vector<std::tuple<Geometry::Navigator::InternalCellHandle,
                         Geometry::Navigator::InternalCellHandle,
                         Geometry::Navigator::InternalCellHandle> >
    cell_ids( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  std::vector<std::tuple<Geometry::Navigator::InternalSurfaceHandle,
                         Geometry::Navigator::InternalSurfaceHandle,
                         Geometry::Navigator::InternalSurfaceHandle,
                         Geometry::Navigator::InternalSurfaceHandle> >
    surface_ids( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  std::vector<std::tuple<double,double,double,double> >
    distances( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  #pragma omp parallel num_threads( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() )
  {
    // Create the thread navigators
    std::shared_ptr<Geometry::Navigator> navigator =
      model->createNavigator();
    
    // Initialize the ray
    if( Utility::GlobalOpenMPSession::getThreadId()%6 == 0 )
    {
      navigator->setState( -40.0*cgs::centimeter,
                           -40.0*cgs::centimeter,
                           59.0*cgs::centimeter,
                           0.0, 0.0, 1.0 );
    }
    else if( Utility::GlobalOpenMPSession::getThreadId()%6 == 1 )
    {
      navigator->setState( -41.0*cgs::centimeter,
                           -41.0*cgs::centimeter,
                           59.0*cgs::centimeter,
                           0.0, 0.0, 1.0 );
    }
    else if( Utility::GlobalOpenMPSession::getThreadId()%6 == 2 )
    {
      navigator->setState( -39.0*cgs::centimeter,
                           -39.0*cgs::centimeter,
                           59.0*cgs::centimeter,
                           0.0, 0.0, 1.0 );
    }
    else if( Utility::GlobalOpenMPSession::getThreadId()%6 == 3 )
    {
      navigator->setState( -38.0*cgs::centimeter,
                           -38.0*cgs::centimeter,
                           59.0*cgs::centimeter,
                           0.0, 0.0, 1.0 );
    }
    else if( Utility::GlobalOpenMPSession::getThreadId()%6 == 4)
    {
      navigator->setState( -37.0*cgs::centimeter,
                           -37.0*cgs::centimeter,
                           59.0*cgs::centimeter,
                           0.0, 0.0, 1.0 );
    }
    else
    {
      navigator->setState( -40.0*cgs::centimeter,
                           -40.0*cgs::centimeter,
                           59.0*cgs::centimeter,
                           0.0, 0.0, 1.0 );
    }

    // Find the cell that contains the ray
    Geometry::Navigator::InternalCellHandle cell = navigator->getCurrentCell();

    Utility::get<0>(cell_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      cell;

    // Fire the ray
    Geometry::Navigator::InternalSurfaceHandle surface_hit;

    Geometry::Navigator::Length distance_to_surface_hit =
      navigator->fireRay( &surface_hit );

    Utility::get<0>(surface_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      surface_hit;

    Utility::get<0>(distances[Utility::GlobalOpenMPSession::getThreadId()]) =
      distance_to_surface_hit.value();

    // Advance the ray to the boundary surface
    navigator->advanceToCellBoundary();

    cell = navigator->getCurrentCell();

    Utility::get<1>(cell_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      cell;

    // Fire the ray
    distance_to_surface_hit = navigator->fireRay( &surface_hit );

    Utility::get<1>(surface_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      surface_hit;

    Utility::get<1>(distances[Utility::GlobalOpenMPSession::getThreadId()]) =
      distance_to_surface_hit.value();

    // Advance the ray to the boundary surface
    navigator->advanceToCellBoundary();

    cell = navigator->getCurrentCell();

    Utility::get<2>(cell_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      cell;

    // Fire the ray
    distance_to_surface_hit = navigator->fireRay( &surface_hit );

    Utility::get<2>(surface_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      surface_hit;

    Utility::get<2>(distances[Utility::GlobalOpenMPSession::getThreadId()]) =
      distance_to_surface_hit.value();

    // Advance the ray a substep
    navigator->advanceBySubstep( 0.5*distance_to_surface_hit);

    // Change the ray direction
    navigator->changeDirection( 0.0, 0.0, -1.0 );

    // Fire the ray
    distance_to_surface_hit = navigator->fireRay( &surface_hit );

    Utility::get<3>(surface_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      surface_hit;

    Utility::get<3>(distances[Utility::GlobalOpenMPSession::getThreadId()]) =
      distance_to_surface_hit.value();
  }

  // Check that each of the rays traces were successful
  std::vector<std::tuple<Geometry::Navigator::InternalCellHandle,
                         Geometry::Navigator::InternalCellHandle,
                         Geometry::Navigator::InternalCellHandle> >
    correct_cell_ids( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  for( size_t i = 0; i < correct_cell_ids.size(); ++i )
    correct_cell_ids[i] = std::make_tuple( 53, 54, 55 );

  FRENSIE_CHECK_EQUAL( cell_ids, correct_cell_ids );

  std::vector<std::tuple<Geometry::Navigator::InternalSurfaceHandle,
                         Geometry::Navigator::InternalSurfaceHandle,
                         Geometry::Navigator::InternalSurfaceHandle,
                         Geometry::Navigator::InternalSurfaceHandle> >
    correct_surface_ids( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  for( size_t i = 0; i < correct_cell_ids.size(); ++i )
    correct_surface_ids[i] = std::make_tuple( 242, 248, 254, 248 );

  FRENSIE_CHECK_EQUAL( surface_ids, correct_surface_ids );

  std::vector<std::tuple<double,double,double,double> >
    correct_distances( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  for( size_t i = 0; i < correct_distances.size(); ++i )
    correct_distances[i] = std::make_tuple( 1.959999084, 2.54, 2.54, 1.27 );

  FRENSIE_CHECK_FLOATING_EQUALITY( distances, correct_distances, 1e-6 );
}

//---------------------------------------------------------------------------//
// Check that a parallel internal ray trace can be done
FRENSIE_UNIT_TEST( DagMC, parallel_ray_trace_with_reflection )
{
  std::vector<std::tuple<Geometry::Navigator::InternalCellHandle,
                         Geometry::Navigator::InternalCellHandle,
                         Geometry::Navigator::InternalCellHandle> >
    cell_ids( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );
  
  std::vector<std::tuple<Geometry::Navigator::InternalSurfaceHandle,
                         Geometry::Navigator::InternalSurfaceHandle,
                         Geometry::Navigator::InternalSurfaceHandle> >
    surface_ids( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  std::vector<std::tuple<double,double,double> >
    distances( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  std::vector<std::tuple<int,int> >
    reflection( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  #pragma omp parallel num_threads( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() )
  {
    // Create the thread navigators
    std::shared_ptr<Geometry::Navigator> navigator =
      model->createNavigator();

    navigator->setState( -40.0*cgs::centimeter,
                         -40.0*cgs::centimeter,
                         108.0*cgs::centimeter,
                         0.0, 0.0, 1.0 );

    // Find the cell that contains the ray
    Geometry::Navigator::InternalCellHandle cell = navigator->getCurrentCell();

    Utility::get<0>(cell_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      cell;

    // Fire the ray
    Geometry::Navigator::InternalSurfaceHandle surface_hit;

    Geometry::Navigator::Length distance_to_surface_hit =
      navigator->fireRay( &surface_hit );

    Utility::get<0>(surface_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      surface_hit;

    Utility::get<0>(distances[Utility::GlobalOpenMPSession::getThreadId()]) =
      distance_to_surface_hit.value();

    // Advance the ray to the boundary surface
    Utility::get<0>(reflection[Utility::GlobalOpenMPSession::getThreadId()]) =
      navigator->advanceToCellBoundary();

    cell = navigator->getCurrentCell();

    Utility::get<1>(cell_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      cell;

    distance_to_surface_hit = navigator->fireRay( &surface_hit );

    Utility::get<1>(surface_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      surface_hit;

    Utility::get<1>(distances[Utility::GlobalOpenMPSession::getThreadId()]) =
      distance_to_surface_hit.value();

    // Advance the ray to the boundary surface (reflecting)
    Utility::get<1>(reflection[Utility::GlobalOpenMPSession::getThreadId()]) =
      navigator->advanceToCellBoundary();

    cell = navigator->getCurrentCell();

    Utility::get<2>(cell_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      cell;

    distance_to_surface_hit = navigator->fireRay( &surface_hit );

    Utility::get<2>(surface_ids[Utility::GlobalOpenMPSession::getThreadId()]) =
      surface_hit;

    Utility::get<2>(distances[Utility::GlobalOpenMPSession::getThreadId()]) =
      distance_to_surface_hit.value();
  }

  // Check that each of the rays traces were successful
  std::vector<std::tuple<Geometry::Navigator::InternalCellHandle,
                               Geometry::Navigator::InternalCellHandle,
                               Geometry::Navigator::InternalCellHandle> >
    correct_cell_ids( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  for( size_t i = 0; i < correct_cell_ids.size(); ++i )
    correct_cell_ids[i] = std::make_tuple( 82, 83, 83 );

  FRENSIE_CHECK_EQUAL( cell_ids, correct_cell_ids );

  std::vector<std::tuple<Geometry::Navigator::InternalSurfaceHandle,
                               Geometry::Navigator::InternalSurfaceHandle,
                               Geometry::Navigator::InternalSurfaceHandle> >
    correct_surface_ids( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  for( size_t i = 0; i < correct_cell_ids.size(); ++i )
    correct_surface_ids[i] = std::make_tuple( 394, 408, 394 );

  FRENSIE_CHECK_EQUAL( surface_ids, correct_surface_ids );

  std::vector<std::tuple<double,double,double> >
    correct_distances( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  for( size_t i = 0; i < correct_distances.size(); ++i )
    correct_distances[i] = std::make_tuple( 1.474, 17.526, 17.526 );

  FRENSIE_CHECK_FLOATING_EQUALITY( distances, correct_distances, 1e-6 );

  std::vector<std::tuple<int,int> >
    correct_reflection( Utility::GlobalOpenMPSession::getRequestedNumberOfThreads() );

  for( size_t i = 0; i < correct_reflection.size(); ++i )
    correct_reflection[i] = std::make_tuple( false, true );

  FRENSIE_CHECK_EQUAL( reflection, correct_reflection );
}

//---------------------------------------------------------------------------//
// Custom setup
//---------------------------------------------------------------------------//
FRENSIE_CUSTOM_UNIT_TEST_SETUP_BEGIN();

std::string test_dagmc_geom_file_name;
int threads;

FRENSIE_CUSTOM_UNIT_TEST_COMMAND_LINE_OPTIONS()
{
  ADD_STANDARD_OPTION_AND_ASSIGN_VALUE( "test_cad_file",
                                        test_dagmc_geom_file_name, "",
                                        "Test CAD file name" );
  ADD_STANDARD_OPTION_AND_ASSIGN_VALUE( "threads",
                                        threads, 1,
                                        "Number of threads to use" );
}

FRENSIE_CUSTOM_UNIT_TEST_INIT()
{
  Geometry::DagMCModelProperties local_properties( test_dagmc_geom_file_name );
  
  local_properties.setFacetTolerance( 1e-3 );
  local_properties.setTerminationCellPropertyName( "graveyard" );
  local_properties.setMaterialPropertyName( "mat" );
  local_properties.setDensityPropertyName( "rho" );
  local_properties.setEstimatorPropertyName( "tally" );

  std::shared_ptr<Geometry::DagMCModel> tmp_model =
    Geometry::DagMCModel::getInstance();

  tmp_model->initialize( local_properties );

  model = tmp_model;

  // Set the number of threads to use
  Utility::GlobalOpenMPSession::setNumberOfThreads( threads );
}

FRENSIE_CUSTOM_UNIT_TEST_SETUP_END();

//---------------------------------------------------------------------------//
// end tstSharedParallelDagMC.cpp
//---------------------------------------------------------------------------//
