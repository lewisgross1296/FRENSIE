//---------------------------------------------------------------------------//
//!
//! \file   tstLinLinLogTwoDInterpolationPolicy.cpp
//! \author Alex Robinson
//! \brief  2D interpolation policy unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>

// FRENSIE Includes
#include "Utility_InterpolationPolicy.hpp"
#include "Utility_TwoDInterpolationPolicy.hpp"
#include "Utility_Tuple.hpp"
#include "Utility_Vector.hpp"
#include "Utility_UnitTestHarnessWithMain.hpp"

//---------------------------------------------------------------------------//
// Testing Types
//---------------------------------------------------------------------------//
typedef std::tuple<
  std::tuple<std::integral_constant<size_t,0>,std::tuple<double,double>,
             std::integral_constant<size_t,0>,std::tuple<double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::tuple<double,double>,
             std::integral_constant<size_t,1>,std::tuple<double,double> >,
  std::tuple<std::integral_constant<size_t,1>,std::tuple<double,double>,
             std::integral_constant<size_t,0>,std::tuple<double,double> >,
  std::tuple<std::integral_constant<size_t,1>,std::tuple<double,double>,
             std::integral_constant<size_t,1>,std::tuple<double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::tuple<double,unsigned>,
             std::integral_constant<size_t,0>,std::tuple<double,unsigned> >,
  std::tuple<std::integral_constant<size_t,1>,std::tuple<unsigned,double>,
             std::integral_constant<size_t,1>,std::tuple<unsigned,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::tuple<double,double,double>,
             std::integral_constant<size_t,0>,std::tuple<double,double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::tuple<double,double,double>,
             std::integral_constant<size_t,1>,std::tuple<double,double,unsigned> >,
  std::tuple<std::integral_constant<size_t,0>,std::tuple<double,double,double>,
             std::integral_constant<size_t,2>,std::tuple<unsigned,double,double> >,
  std::tuple<std::integral_constant<size_t,1>,std::tuple<unsigned,double,double>,
             std::integral_constant<size_t,0>,std::tuple<double,unsigned,double> >,
  std::tuple<std::integral_constant<size_t,2>,std::tuple<double,unsigned,double>,
             std::integral_constant<size_t,0>,std::tuple<double,unsigned,double> >,
  std::tuple<std::integral_constant<size_t,2>,std::tuple<double,unsigned,double>,
             std::integral_constant<size_t,1>,std::tuple<unsigned,double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::tuple<double,double,double,double>,
             std::integral_constant<size_t,0>,std::tuple<double,double,double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::tuple<double,double,double,double>,
             std::integral_constant<size_t,2>,std::tuple<double,double,double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::tuple<double,double,double,double>,
             std::integral_constant<size_t,3>,std::tuple<double,double,double,double> >,
  std::tuple<std::integral_constant<size_t,1>,std::tuple<double,double,double,double>,
             std::integral_constant<size_t,0>,std::tuple<double,double,double,double> >,
  std::tuple<std::integral_constant<size_t,2>,std::tuple<double,double,double,double>,
             std::integral_constant<size_t,1>,std::tuple<double,double,double,double> >,
  std::tuple<std::integral_constant<size_t,3>,std::tuple<unsigned,double,double,double>,
             std::integral_constant<size_t,2>,std::tuple<unsigned,double,double,double> >
  > TestTwoTupleTwoElementTypes;

typedef std::tuple<
  std::tuple<std::integral_constant<size_t,0>,std::integral_constant<size_t,1>,std::tuple<double,double> >,
  std::tuple<std::integral_constant<size_t,1>,std::integral_constant<size_t,0>,std::tuple<double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::integral_constant<size_t,1>,std::tuple<double,double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::integral_constant<size_t,2>,std::tuple<double,unsigned,double> >,
  std::tuple<std::integral_constant<size_t,1>,std::integral_constant<size_t,0>,std::tuple<double,double,unsigned> >,
  std::tuple<std::integral_constant<size_t,1>,std::integral_constant<size_t,2>,std::tuple<unsigned,double,double> >,
  std::tuple<std::integral_constant<size_t,2>,std::integral_constant<size_t,0>,std::tuple<double,double,double> >,
  std::tuple<std::integral_constant<size_t,2>,std::integral_constant<size_t,1>,std::tuple<double,double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::integral_constant<size_t,1>,std::tuple<double,double,double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::integral_constant<size_t,2>,std::tuple<double,unsigned,double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::integral_constant<size_t,3>,std::tuple<double,double,unsigned,double> >,
  std::tuple<std::integral_constant<size_t,1>,std::integral_constant<size_t,0>,std::tuple<double,double,double,unsigned> >,
  std::tuple<std::integral_constant<size_t,1>,std::integral_constant<size_t,2>,std::tuple<unsigned,double,double,double> >,
  std::tuple<std::integral_constant<size_t,1>,std::integral_constant<size_t,3>,std::tuple<double,double,double,double> >,
  std::tuple<std::integral_constant<size_t,2>,std::integral_constant<size_t,0>,std::tuple<double,double,double,double> >,
  std::tuple<std::integral_constant<size_t,2>,std::integral_constant<size_t,1>,std::tuple<double,double,double,double> >,
  std::tuple<std::integral_constant<size_t,2>,std::integral_constant<size_t,3>,std::tuple<double,double,double,double> >,
  std::tuple<std::integral_constant<size_t,3>,std::integral_constant<size_t,0>,std::tuple<double,double,double,double> >,
  std::tuple<std::integral_constant<size_t,3>,std::integral_constant<size_t,1>,std::tuple<double,double,double,double> >,
  std::tuple<std::integral_constant<size_t,3>,std::integral_constant<size_t,2>,std::tuple<double,unsigned,double,double> >
  > TestTupleTwoElementTypes;

typedef std::tuple<
  std::tuple<std::integral_constant<size_t,0>,std::tuple<double,double> >,
  std::tuple<std::integral_constant<size_t,1>,std::tuple<double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::tuple<double,unsigned,double> >,
  std::tuple<std::integral_constant<size_t,1>,std::tuple<double,double,unsigned> >,
  std::tuple<std::integral_constant<size_t,2>,std::tuple<unsigned,double,double> >,
  std::tuple<std::integral_constant<size_t,0>,std::tuple<double,unsigned,double,double> >,
  std::tuple<std::integral_constant<size_t,1>,std::tuple<double,double,unsigned,double> >,
  std::tuple<std::integral_constant<size_t,2>,std::tuple<double,double,double,unsigned> >,
  std::tuple<std::integral_constant<size_t,3>,std::tuple<unsigned,double,double,double> >
  > TestTupleElementTypes;

//---------------------------------------------------------------------------//
// Tests
//---------------------------------------------------------------------------//
// Check that the linear-linear-log interpolation policy between four points
// can be done
FRENSIE_UNIT_TEST_TEMPLATE_EXPAND( LinLinLog,
                                   interpolate_separate_tuple_grids,
                                   TestTwoTupleTwoElementTypes )
{
  FETCH_TEMPLATE_PARAM( 0, WrappedYMember );
  FETCH_TEMPLATE_PARAM( 1, ytuple );
  FETCH_TEMPLATE_PARAM( 2, WrappedZMember );
  FETCH_TEMPLATE_PARAM( 3, ztuple );

  constexpr const size_t ymember = WrappedYMember::value;
  constexpr const size_t zmember = WrappedZMember::value;
  
  double x0 = 0.1, x1 = 1.0, x = 0.3, y = 0.0;

  std::vector<ytuple> y_0_grid( 4 );
  Utility::set<ymember>( y_0_grid[0], -10.0 );
  Utility::set<ymember>( y_0_grid[1], -1.0 );
  Utility::set<ymember>( y_0_grid[2], 1.0 );
  Utility::set<ymember>( y_0_grid[3], 10.0 );

  std::vector<ztuple> z_0_grid( 4 );
  Utility::set<zmember>( z_0_grid[0], 100.0 );
  Utility::set<zmember>( z_0_grid[1], 0.0 );
  Utility::set<zmember>( z_0_grid[2], 1.0 );
  Utility::set<zmember>( z_0_grid[3], 10.0 );

  std::vector<ytuple> y_1_grid( 3 );
  Utility::set<ymember>( y_1_grid[0], -10.0 );
  Utility::set<ymember>( y_1_grid[1], -5.0 );
  Utility::set<ymember>( y_1_grid[2], 10.0 );

  std::vector<ztuple> z_1_grid( 3 );
  Utility::set<zmember>( z_1_grid[0], 50.0 );
  Utility::set<zmember>( z_1_grid[1], 10.0 );
  Utility::set<zmember>( z_1_grid[2], 5.0 );

  double z = Utility::LinLinLog::interpolate<ymember,zmember>(x0,
                                  x1,
                                  x,
                                  y,
                                  y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 4.237449828637356, 1e-12 );

  x = 0.1;

  z = Utility::LinLinLog::interpolate<ymember,zmember>( x0,
                            x1,
                            x,
                            y,
                            y_0_grid.begin(),
                            y_0_grid.end(),
                            z_0_grid.begin(),
                            z_0_grid.end(),
                            y_1_grid.begin(),
                            y_1_grid.end(),
                            z_1_grid.begin(),
                            z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.5, 1e-12 );

  x = 1.0;

  z = Utility::LinLinLog::interpolate<ymember,zmember>( x0,
                            x1,
                            x,
                            y,
                            y_0_grid.begin(),
                            y_0_grid.end(),
                            z_0_grid.begin(),
                            z_0_grid.end(),
                            y_1_grid.begin(),
                            y_1_grid.end(),
                            z_1_grid.begin(),
                            z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 8.3333333333333, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the linear-linear-log interpolation policy between four points
// can be done
FRENSIE_UNIT_TEST_TEMPLATE_EXPAND( LinLinLog,
                                   interpolate_combined_tuple_grids,
                                   TestTupleTwoElementTypes )
{
  FETCH_TEMPLATE_PARAM( 0, WrappedYMember );
  FETCH_TEMPLATE_PARAM( 1, WrappedZMember );
  FETCH_TEMPLATE_PARAM( 2, tuple );

  constexpr const size_t ymember = WrappedYMember::value;
  constexpr const size_t zmember = WrappedZMember::value;
  
  double x0 = 0.1, x1 = 1.0, x = 0.3, y = 0.0;

  std::vector<tuple> grid_0( 4 );
  Utility::set<ymember>( grid_0[0], -10.0 );
  Utility::set<ymember>( grid_0[1], -1.0 );
  Utility::set<ymember>( grid_0[2], 1.0 );
  Utility::set<ymember>( grid_0[3], 10.0 );
  Utility::set<zmember>( grid_0[0], 100.0 );
  Utility::set<zmember>( grid_0[1], 0.0 );
  Utility::set<zmember>( grid_0[2], 1.0 );
  Utility::set<zmember>( grid_0[3], 10.0 );

  std::vector<tuple> grid_1( 3 );
  Utility::set<ymember>( grid_1[0], -10.0 );
  Utility::set<ymember>( grid_1[1], -5.0 );
  Utility::set<ymember>( grid_1[2], 10.0 );
  Utility::set<zmember>( grid_1[0], 50.0 );
  Utility::set<zmember>( grid_1[1], 10.0 );
  Utility::set<zmember>( grid_1[2], 5.0 );

  double z = Utility::LinLinLog::interpolate<ymember,zmember>( x0,
                                   x1,
                                   x,
                                   y,
                                   grid_0.begin(),
                                   grid_0.end(),
                                   grid_1.begin(),
                                   grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 4.237449828637356, 1e-12 );

  x = 0.1;

  z = Utility::LinLinLog::interpolate<ymember,zmember>( x0,
                            x1,
                            x,
                            y,
                            grid_0.begin(),
                            grid_0.end(),
                            grid_1.begin(),
                            grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.5, 1e-12 );

  x = 1.0;

  z = Utility::LinLinLog::interpolate<ymember,zmember>( x0,
                            x1,
                            x,
                            y,
                            grid_0.begin(),
                            grid_0.end(),
                            grid_1.begin(),
                            grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 8.3333333333333, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the linear-linear-log interpolation policy between four points
// can be done
FRENSIE_UNIT_TEST( LinLinLog, interpolate_no_tuple_grids )
{
  double x0 = 0.1, x1 = 1.0, x = 0.3, y = 0.0;

  std::vector<double> y_0_grid( 4 );
  y_0_grid[0] = -10.0;
  y_0_grid[1] = -1.0;
  y_0_grid[2] = 1.0;
  y_0_grid[3] = 10.0;

  std::vector<double> z_0_grid( 4 );
  z_0_grid[0] = 100.0;
  z_0_grid[1] = 0.0;
  z_0_grid[2] = 1.0;
  z_0_grid[3] = 10.0;

  std::vector<double> y_1_grid( 3 );
  y_1_grid[0] = -10.0;
  y_1_grid[1] = -5.0;
  y_1_grid[2] = 10.0;

  std::vector<double> z_1_grid( 3 );
  z_1_grid[0] = 50.0;
  z_1_grid[1] = 10.0;
  z_1_grid[2] = 5.0;

  double z = Utility::LinLinLog::interpolate( x0,
                          x1,
                          x,
                          y,
                          y_0_grid.begin(),
                          y_0_grid.end(),
                          z_0_grid.begin(),
                          z_0_grid.end(),
                          y_1_grid.begin(),
                          y_1_grid.end(),
                          z_1_grid.begin(),
                          z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 4.237449828637356, 1e-12 );

  x = 0.1;

  z = Utility::LinLinLog::interpolate( x0,
                       x1,
                       x,
                       y,
                       y_0_grid.begin(),
                       y_0_grid.end(),
                       z_0_grid.begin(),
                       z_0_grid.end(),
                       y_1_grid.begin(),
                       y_1_grid.end(),
                       z_1_grid.begin(),
                       z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.5, 1e-12 );

  x = 1.0;

  z = Utility::LinLinLog::interpolate( x0,
                       x1,
                       x,
                       y,
                       y_0_grid.begin(),
                       y_0_grid.end(),
                       z_0_grid.begin(),
                       z_0_grid.end(),
                       y_1_grid.begin(),
                       y_1_grid.end(),
                       z_1_grid.begin(),
                       z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 8.3333333333333, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the intermediate grid length can be calculated
FRENSIE_UNIT_TEST( LinLinLog, calculateIntermediateGridLength )
{
  double x0 = 0.1, x1 = 1.0, x = 0.3;
  double L0 = 3.0, L1 = 5.0;

  double Lx = Utility::LinLinLog::calculateIntermediateGridLength(
                                                            x0, x1, x, L0, L1 );

  FRENSIE_CHECK_FLOATING_EQUALITY( Lx, 3.9542425094393248, 1e-15 );

  x = 0.1;

  Lx = Utility::LinLinLog::calculateIntermediateGridLength(x0, x1, x, L0, L1 );

  FRENSIE_CHECK_FLOATING_EQUALITY( Lx, 3.0, 1e-15 );

  x = 1.0;

  Lx = Utility::LinLinLog::calculateIntermediateGridLength(x0, x1, x, L0, L1 );

  FRENSIE_CHECK_FLOATING_EQUALITY( Lx, 5.0, 1e-15 );
}

//---------------------------------------------------------------------------//
// Check that the intermediate grid min value can be calculated
FRENSIE_UNIT_TEST( LinLinLog, calculateIntermediateGridLimit )
{
  double x0 = 0.1, x1 = 1.0, x = 0.3;
  double y0_min = -10.0, y1_min = 10.0;

  double yx_min = Utility::LinLinLog::calculateIntermediateGridLimit(
                           x0, x1, x, y0_min, y1_min );

  FRENSIE_CHECK_FLOATING_EQUALITY( yx_min, -0.4575749056067533, 1e-15 );

  x = 0.1;

  yx_min = Utility::LinLinLog::calculateIntermediateGridLimit(
                           x0, x1, x, y0_min, y1_min );

  FRENSIE_CHECK_FLOATING_EQUALITY( yx_min, -10.0, 1e-15 );

  x = 1.0;

  yx_min = Utility::LinLinLog::calculateIntermediateGridLimit(
                           x0, x1, x, y0_min, y1_min );

  FRENSIE_CHECK_FLOATING_EQUALITY( yx_min, 10.0, 1e-15 );
}

//---------------------------------------------------------------------------//
// Check that the linear-linear-log unit base interpolation policy between
// four points can be done
FRENSIE_UNIT_TEST_TEMPLATE_EXPAND( LinLinLog,
                                   interpolateUnitBase_separate_tuple_grids,
                                   TestTwoTupleTwoElementTypes )
{
  FETCH_TEMPLATE_PARAM( 0, WrappedYMember );
  FETCH_TEMPLATE_PARAM( 1, ytuple );
  FETCH_TEMPLATE_PARAM( 2, WrappedZMember );
  FETCH_TEMPLATE_PARAM( 3, ztuple );

  constexpr const size_t ymember = WrappedYMember::value;
  constexpr const size_t zmember = WrappedZMember::value;
  
  double x0 = 0.1, x1 = 1.0, x = 0.3, y = 0.0;

  std::vector<ytuple> y_0_grid( 4 );
  Utility::set<ymember>( y_0_grid[0], -2.0 );
  Utility::set<ymember>( y_0_grid[1], -1.0 );
  Utility::set<ymember>( y_0_grid[2], 1.0 );
  Utility::set<ymember>( y_0_grid[3], 2.0 );

  std::vector<ztuple> z_0_grid( 4 );
  Utility::set<zmember>( z_0_grid[0], 0.0 );
  Utility::set<zmember>( z_0_grid[1], 1.0 );
  Utility::set<zmember>( z_0_grid[2], 2.0 );
  Utility::set<zmember>( z_0_grid[3], 3.0 );

  std::vector<ytuple> y_1_grid( 3 );
  Utility::set<ymember>( y_1_grid[0], -1.0 );
  Utility::set<ymember>( y_1_grid[1], 0.0 );
  Utility::set<ymember>( y_1_grid[2], 2.0 );

  std::vector<ztuple> z_1_grid( 3 );
  Utility::set<zmember>( z_1_grid[0], 1.0 );
  Utility::set<zmember>( z_1_grid[1], 2.0 );
  Utility::set<zmember>( z_1_grid[2], 3.0 );

  double z =
    Utility::LinLinLog::interpolateUnitBase<ymember,zmember>( x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.6830508003062505, 1e-12 );

  y = -1.5228787452803376; // min possible y at x = 0.3

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.40630514634561576, 1e-12 );

  y = 2.0; // max possible y at x = 0.3

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );

  y = 0.0;
  x = 0.1;

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.5, 1e-12 );

  y = -2.0;

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.0, 1e-12 );

  y = 2.0;

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );

  y = 0.0;
  x = 1.0;

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 2.0, 1e-12 );

  y = -1.0;

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.0, 1e-12 );

  y = 2.0;

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the linear-linear-log unit base interpolation policy between
// four points can be done
FRENSIE_UNIT_TEST_TEMPLATE_EXPAND( LinLinLog,
                                   interpolateUnitBase_combined_tuple_grids,
                                   TestTupleTwoElementTypes )
{
  FETCH_TEMPLATE_PARAM( 0, WrappedYMember );
  FETCH_TEMPLATE_PARAM( 1, WrappedZMember );
  FETCH_TEMPLATE_PARAM( 2, tuple );

  constexpr const size_t ymember = WrappedYMember::value;
  constexpr const size_t zmember = WrappedZMember::value;
  
  double x0 = 0.1, x1 = 1.0, x = 0.3, y = 0.0;

  std::vector<tuple> grid_0( 4 );
  Utility::set<ymember>( grid_0[0], -2.0 );
  Utility::set<ymember>( grid_0[1], -1.0 );
  Utility::set<ymember>( grid_0[2], 1.0 );
  Utility::set<ymember>( grid_0[3], 2.0 );
  Utility::set<zmember>( grid_0[0], 0.0 );
  Utility::set<zmember>( grid_0[1], 1.0 );
  Utility::set<zmember>( grid_0[2], 2.0 );
  Utility::set<zmember>( grid_0[3], 3.0 );

  std::vector<tuple> grid_1( 3 );
  Utility::set<ymember>( grid_1[0], -1.0 );
  Utility::set<ymember>( grid_1[1], 0.0 );
  Utility::set<ymember>( grid_1[2], 2.0 );
  Utility::set<zmember>( grid_1[0], 1.0 );
  Utility::set<zmember>( grid_1[1], 2.0 );
  Utility::set<zmember>( grid_1[2], 3.0 );

  double z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.6830508003062505, 1e-12 );

  y = -1.5228787452803376; // min possible y at x = 0.3

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.40630514634561576, 1e-12 );

  y = 2.0; // max possible y at x = 0.3

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );

  y = 0.0;
  x = 0.1;

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.5, 1e-12 );

  y = -2.0;

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.0, 1e-12 );

  y = 2.0;

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );

  y = 0.0;
  x = 1.0;

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 2.0, 1e-12 );

  y = -1.0;

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.0, 1e-12 );

  y = 2.0;

  z = Utility::LinLinLog::interpolateUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the linear-linear-log unit base interpolation policy between
// four points can be done
FRENSIE_UNIT_TEST( LinLinLog, interpolateUnitBase_no_tuple_grids )
{
  double x0 = 0.1, x1 = 1.0, x = 0.3, y = 0.0;

  std::vector<double> y_0_grid( 4 );
  y_0_grid[0] = -2.0;
  y_0_grid[1] = -1.0;
  y_0_grid[2] = 1.0;
  y_0_grid[3] = 2.0;

  std::vector<double> z_0_grid( 4 );

  z_0_grid[0] = 0.0;
  z_0_grid[1] = 1.0;
  z_0_grid[2] = 2.0;
  z_0_grid[3] = 3.0;

  std::vector<double> y_1_grid( 3 );
  y_1_grid[0] = -1.0;
  y_1_grid[1] = 0.0;
  y_1_grid[2] = 2.0;

  std::vector<double> z_1_grid( 3 );
  z_1_grid[0] = 1.0;
  z_1_grid[1] = 2.0;
  z_1_grid[2] = 3.0;

  double z = Utility::LinLinLog::interpolateUnitBase( x0,
                              x1,
                              x,
                              y,
                              y_0_grid.begin(),
                              y_0_grid.end(),
                              z_0_grid.begin(),
                              z_0_grid.end(),
                              y_1_grid.begin(),
                              y_1_grid.end(),
                              z_1_grid.begin(),
                              z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.6830508003062505, 1e-12 );

  y = -1.5228787452803376; // min possible y at x = 0.3

  z = Utility::LinLinLog::interpolateUnitBase( x0,
                           x1,
                           x,
                           y,
                           y_0_grid.begin(),
                           y_0_grid.end(),
                           z_0_grid.begin(),
                           z_0_grid.end(),
                           y_1_grid.begin(),
                           y_1_grid.end(),
                           z_1_grid.begin(),
                           z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.40630514634561576, 1e-12 );

  y = 2.0; // max possible y at x = 0.3

  z = Utility::LinLinLog::interpolateUnitBase( x0,
                           x1,
                           x,
                           y,
                           y_0_grid.begin(),
                           y_0_grid.end(),
                           z_0_grid.begin(),
                           z_0_grid.end(),
                           y_1_grid.begin(),
                           y_1_grid.end(),
                           z_1_grid.begin(),
                           z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );

  y = 0.0;
  x = 0.1;

  z = Utility::LinLinLog::interpolateUnitBase( x0,
                           x1,
                           x,
                           y,
                           y_0_grid.begin(),
                           y_0_grid.end(),
                           z_0_grid.begin(),
                           z_0_grid.end(),
                           y_1_grid.begin(),
                           y_1_grid.end(),
                           z_1_grid.begin(),
                           z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.5, 1e-12 );

  y = -2.0;

  z = Utility::LinLinLog::interpolateUnitBase( x0,
                           x1,
                           x,
                           y,
                           y_0_grid.begin(),
                           y_0_grid.end(),
                           z_0_grid.begin(),
                           z_0_grid.end(),
                           y_1_grid.begin(),
                           y_1_grid.end(),
                           z_1_grid.begin(),
                           z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.0, 1e-12 );

  y = 2.0;

  z = Utility::LinLinLog::interpolateUnitBase( x0,
                           x1,
                           x,
                           y,
                           y_0_grid.begin(),
                           y_0_grid.end(),
                           z_0_grid.begin(),
                           z_0_grid.end(),
                           y_1_grid.begin(),
                           y_1_grid.end(),
                           z_1_grid.begin(),
                           z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );

  y = 0.0;
  x = 1.0;

  z = Utility::LinLinLog::interpolateUnitBase( x0,
                           x1,
                           x,
                           y,
                           y_0_grid.begin(),
                           y_0_grid.end(),
                           z_0_grid.begin(),
                           z_0_grid.end(),
                           y_1_grid.begin(),
                           y_1_grid.end(),
                           z_1_grid.begin(),
                           z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 2.0, 1e-12 );

  y = -1.0;

  z = Utility::LinLinLog::interpolateUnitBase( x0,
                           x1,
                           x,
                           y,
                           y_0_grid.begin(),
                           y_0_grid.end(),
                           z_0_grid.begin(),
                           z_0_grid.end(),
                           y_1_grid.begin(),
                           y_1_grid.end(),
                           z_1_grid.begin(),
                           z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.0, 1e-12 );

  y = 2.0;

  z = Utility::LinLinLog::interpolateUnitBase( x0,
                           x1,
                           x,
                           y,
                           y_0_grid.begin(),
                           y_0_grid.end(),
                           z_0_grid.begin(),
                           z_0_grid.end(),
                           y_1_grid.begin(),
                           y_1_grid.end(),
                           z_1_grid.begin(),
                           z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the dependent variable can be processed
FRENSIE_UNIT_TEST( LinLinLog, processDepVar )
{
  FRENSIE_CHECK_EQUAL( -1.0, Utility::LinLinLog::processDepVar( -1.0 ) );
  FRENSIE_CHECK_EQUAL( 0.0, Utility::LinLinLog::processDepVar( 0.0 ) );
  FRENSIE_CHECK_EQUAL( 1.0, Utility::LinLinLog::processDepVar( 1.0 ) );
}

//---------------------------------------------------------------------------//
// Check that a processed dependent variable can be recovered
FRENSIE_UNIT_TEST( LinLinLog, recoverProcessedDepVar )
{
  FRENSIE_CHECK_EQUAL( -1.0, Utility::LinLinLog::recoverProcessedDepVar(-1.0));
  FRENSIE_CHECK_EQUAL( 0.0, Utility::LinLinLog::recoverProcessedDepVar(0.0) );
  FRENSIE_CHECK_EQUAL( 1.0, Utility::LinLinLog::recoverProcessedDepVar(1.0) );
}

//---------------------------------------------------------------------------//
// Check that the first independent variable can be processed
FRENSIE_UNIT_TEST( LinLinLog, processSecondIndepVar )
{
  FRENSIE_CHECK_EQUAL( -1.0, Utility::LinLinLog::processSecondIndepVar(-1.0) );
  FRENSIE_CHECK_EQUAL( 0.0, Utility::LinLinLog::processSecondIndepVar(0.0) );
  FRENSIE_CHECK_EQUAL( 1.0, Utility::LinLinLog::processSecondIndepVar(1.0) );
}

//---------------------------------------------------------------------------//
// Check that a processed first independent variable can be processed
FRENSIE_UNIT_TEST( LinLinLog, recoverProcessedSecondIndepVar )
{
  FRENSIE_CHECK_EQUAL(
		    -1.0,
		    Utility::LinLinLog::recoverProcessedSecondIndepVar(-1.0) );
  FRENSIE_CHECK_EQUAL(0.0,
		      Utility::LinLinLog::recoverProcessedSecondIndepVar(0.0));
  FRENSIE_CHECK_EQUAL(1.0,
		      Utility::LinLinLog::recoverProcessedSecondIndepVar(1.0));
}

//---------------------------------------------------------------------------//
// Check that the second independent variable can be processed
FRENSIE_UNIT_TEST( LinLinLog, processFirstIndepVar )
{
  FRENSIE_CHECK_FLOATING_EQUALITY( std::log(0.1),
			  Utility::LinLinLog::processFirstIndepVar(0.1),
			  1e-15 );
  FRENSIE_CHECK_FLOATING_EQUALITY( std::log(1.0),
			  Utility::LinLinLog::processFirstIndepVar(1.0),
			  1e-15 );
}

//---------------------------------------------------------------------------//
// Check that a processed second independent variable can be recovered
FRENSIE_UNIT_TEST( LinLinLog, recoverProcessedFirstIndepVar )
{
  FRENSIE_CHECK_FLOATING_EQUALITY(
		  0.1,
		  Utility::LinLinLog::recoverProcessedFirstIndepVar(std::log(0.1)),
		  1e-15 );
  FRENSIE_CHECK_FLOATING_EQUALITY(
		  1.0,
		  Utility::LinLinLog::recoverProcessedFirstIndepVar(std::log(1.0)),
		  1e-15 );
}

//---------------------------------------------------------------------------//
// Check that the linear-linear-log interpolation policy between four points
// can be done
FRENSIE_UNIT_TEST_TEMPLATE_EXPAND( LinLinLog,
                                   interpolateProcessed_separate_tuple_grids,
                                   TestTwoTupleTwoElementTypes )
{
  FETCH_TEMPLATE_PARAM( 0, WrappedYMember );
  FETCH_TEMPLATE_PARAM( 1, ytuple );
  FETCH_TEMPLATE_PARAM( 2, WrappedZMember );
  FETCH_TEMPLATE_PARAM( 3, ztuple );

  constexpr const size_t ymember = WrappedYMember::value;
  constexpr const size_t zmember = WrappedZMember::value;
  
  double x0 = Utility::LinLinLog::processFirstIndepVar(0.1);
  double x1 = Utility::LinLinLog::processFirstIndepVar(1.0);
  double x = Utility::LinLinLog::processFirstIndepVar(0.3);
  double y = 0.0;

  std::vector<ytuple> y_0_grid( 4 );
  Utility::set<ymember>( y_0_grid[0], -10.0 );
  Utility::set<ymember>( y_0_grid[1], -1.0 );
  Utility::set<ymember>( y_0_grid[2], 1.0 );
  Utility::set<ymember>( y_0_grid[3], 10.0 );

  std::vector<ztuple> z_0_grid( 4 );
  Utility::set<zmember>( z_0_grid[0], 100.0 );
  Utility::set<zmember>( z_0_grid[1], 0.0 );
  Utility::set<zmember>( z_0_grid[2], 1.0 );
  Utility::set<zmember>( z_0_grid[3], 10.0 );

  std::vector<ytuple> y_1_grid( 3 );
  Utility::set<ymember>( y_1_grid[0], -10.0 );
  Utility::set<ymember>( y_1_grid[1], -5.0 );
  Utility::set<ymember>( y_1_grid[2], 10.0 );

  std::vector<ztuple> z_1_grid( 3 );
  Utility::set<zmember>( z_1_grid[0], 50.0 );
  Utility::set<zmember>( z_1_grid[1], 10.0 );
  Utility::set<zmember>( z_1_grid[2], 5.0 );

  double z = Utility::LinLinLog::interpolateProcessed<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                  y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 4.237449828637356, 1e-12 );

  x = Utility::LinLinLog::processFirstIndepVar(0.1);

  z = Utility::LinLinLog::interpolateProcessed<ymember,zmember>(
                            x0,
                            x1,
                            x,
                            y,
                            y_0_grid.begin(),
                            y_0_grid.end(),
                            z_0_grid.begin(),
                            z_0_grid.end(),
                            y_1_grid.begin(),
                            y_1_grid.end(),
                            z_1_grid.begin(),
                            z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.5, 1e-12 );

  x = Utility::LinLinLog::processFirstIndepVar(1.0);

  z = Utility::LinLinLog::interpolateProcessed<ymember,zmember>(
                            x0,
                            x1,
                            x,
                            y,
                            y_0_grid.begin(),
                            y_0_grid.end(),
                            z_0_grid.begin(),
                            z_0_grid.end(),
                            y_1_grid.begin(),
                            y_1_grid.end(),
                            z_1_grid.begin(),
                            z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 8.3333333333333, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the linear-linear-log interpolation policy between four points
// can be done
FRENSIE_UNIT_TEST_TEMPLATE_EXPAND( LinLinLog,
                                   interpolateProcessed_combined_tuple_grids,
                                   TestTupleTwoElementTypes )
{
  FETCH_TEMPLATE_PARAM( 0, WrappedYMember );
  FETCH_TEMPLATE_PARAM( 1, WrappedZMember );
  FETCH_TEMPLATE_PARAM( 2, tuple );

  constexpr const size_t ymember = WrappedYMember::value;
  constexpr const size_t zmember = WrappedZMember::value;
  
  double x0 = Utility::LinLinLog::processFirstIndepVar(0.1);
  double x1 = Utility::LinLinLog::processFirstIndepVar(1.0);
  double x = Utility::LinLinLog::processFirstIndepVar(0.3);
  double y = 0.0;

  std::vector<tuple> grid_0( 4 );
  Utility::set<ymember>( grid_0[0], -10.0 );
  Utility::set<ymember>( grid_0[1], -1.0 );
  Utility::set<ymember>( grid_0[2], 1.0 );
  Utility::set<ymember>( grid_0[3], 10.0 );
  Utility::set<zmember>( grid_0[0], 100.0 );
  Utility::set<zmember>( grid_0[1], 0.0 );
  Utility::set<zmember>( grid_0[2], 1.0 );
  Utility::set<zmember>( grid_0[3], 10.0 );

  std::vector<tuple> grid_1( 3 );
  Utility::set<ymember>( grid_1[0], -10.0 );
  Utility::set<ymember>( grid_1[1], -5.0 );
  Utility::set<ymember>( grid_1[2], 10.0 );
  Utility::set<zmember>( grid_1[0], 50.0 );
  Utility::set<zmember>( grid_1[1], 10.0 );
  Utility::set<zmember>( grid_1[2], 5.0 );

  double z = Utility::LinLinLog::interpolateProcessed<ymember,zmember>(
                                   x0,
                                   x1,
                                   x,
                                   y,
                                   grid_0.begin(),
                                   grid_0.end(),
                                   grid_1.begin(),
                                   grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 4.237449828637356, 1e-12 );

  x = Utility::LinLinLog::processFirstIndepVar(0.1);

  z = Utility::LinLinLog::interpolateProcessed<ymember,zmember>(
                            x0,
                            x1,
                            x,
                            y,
                            grid_0.begin(),
                            grid_0.end(),
                            grid_1.begin(),
                            grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.5, 1e-12 );

  x = Utility::LinLinLog::processFirstIndepVar(1.0);

  z = Utility::LinLinLog::interpolateProcessed<ymember,zmember>(
                            x0,
                            x1,
                            x,
                            y,
                            grid_0.begin(),
                            grid_0.end(),
                            grid_1.begin(),
                            grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 8.3333333333333, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the linear-linear-log interpolation policy between four points
// can be done
FRENSIE_UNIT_TEST( LinLinLog, interpolateProcessed_no_tuple_grids )
{
  double x0 = Utility::LinLinLog::processFirstIndepVar(0.1);
  double x1 = Utility::LinLinLog::processFirstIndepVar(1.0);
  double x = Utility::LinLinLog::processFirstIndepVar(0.3);
  double y = 0.0;

  std::vector<double> y_0_grid( 4 );
  y_0_grid[0] = -10.0;
  y_0_grid[1] = -1.0;
  y_0_grid[2] = 1.0;
  y_0_grid[3] = 10.0;

  std::vector<double> z_0_grid( 4 );
  z_0_grid[0] = 100.0;
  z_0_grid[1] = 0.0;
  z_0_grid[2] = 1.0;
  z_0_grid[3] = 10.0;

  std::vector<double> y_1_grid( 3 );
  y_1_grid[0] = -10.0;
  y_1_grid[1] = -5.0;
  y_1_grid[2] = 10.0;

  std::vector<double> z_1_grid( 3 );
  z_1_grid[0] = 50.0;
  z_1_grid[1] = 10.0;
  z_1_grid[2] = 5.0;

  double z = Utility::LinLinLog::interpolateProcessed( x0,
                               x1,
                               x,
                               y,
                               y_0_grid.begin(),
                               y_0_grid.end(),
                               z_0_grid.begin(),
                               z_0_grid.end(),
                               y_1_grid.begin(),
                               y_1_grid.end(),
                               z_1_grid.begin(),
                               z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 4.237449828637356, 1e-12 );

  x = Utility::LinLinLog::processFirstIndepVar(0.1);

  z = Utility::LinLinLog::interpolateProcessed( x0,
                        x1,
                        x,
                        y,
                        y_0_grid.begin(),
                        y_0_grid.end(),
                        z_0_grid.begin(),
                        z_0_grid.end(),
                        y_1_grid.begin(),
                        y_1_grid.end(),
                        z_1_grid.begin(),
                        z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.5, 1e-12 );

  x = Utility::LinLinLog::processFirstIndepVar(1.0);

  z = Utility::LinLinLog::interpolateProcessed( x0,
                        x1,
                        x,
                        y,
                        y_0_grid.begin(),
                        y_0_grid.end(),
                        z_0_grid.begin(),
                        z_0_grid.end(),
                        y_1_grid.begin(),
                        y_1_grid.end(),
                        z_1_grid.begin(),
                        z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 8.3333333333333, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the intermediate grid length can be calculated
FRENSIE_UNIT_TEST( LinLinLog, calculateIntermediateGridLengthProcessed )
{
  double x0 = Utility::LinLinLog::processFirstIndepVar(0.1);
  double x1 = Utility::LinLinLog::processFirstIndepVar(1.0);
  double x = Utility::LinLinLog::processFirstIndepVar(0.3);
  double L0 = 3.0, L1 = 5.0;

  double Lx = Utility::LinLinLog::calculateIntermediateGridLengthProcessed(
                               x0, x1, x, L0, L1 );

  FRENSIE_CHECK_FLOATING_EQUALITY( Lx, 3.9542425094393248, 1e-15 );

  x = Utility::LinLinLog::processFirstIndepVar(0.1);

  Lx = Utility::LinLinLog::calculateIntermediateGridLengthProcessed(
                               x0, x1, x, L0, L1 );

  FRENSIE_CHECK_FLOATING_EQUALITY( Lx, 3.0, 1e-15 );

  x = Utility::LinLinLog::processFirstIndepVar(1.0);

  Lx = Utility::LinLinLog::calculateIntermediateGridLengthProcessed(
                               x0, x1, x, L0, L1 );

  FRENSIE_CHECK_FLOATING_EQUALITY( Lx, 5.0, 1e-15 );
}

//---------------------------------------------------------------------------//
// Check that the intermediate grid min value can be calculated
FRENSIE_UNIT_TEST( LinLinLog, calculateIntermediateProcessedGridLimit )
{
  double x0 = Utility::LinLinLog::processFirstIndepVar(0.1);
  double x1 = Utility::LinLinLog::processFirstIndepVar(1.0);
  double x = Utility::LinLinLog::processFirstIndepVar(0.3);
  double y0_min = -10.0, y1_min = 10.0;

  double yx_min = Utility::LinLinLog::calculateIntermediateProcessedGridLimit(
                           x0, x1, x, y0_min, y1_min );

  FRENSIE_CHECK_FLOATING_EQUALITY( yx_min, -0.4575749056067533, 1e-15 );

  x = Utility::LinLinLog::processFirstIndepVar(0.1);

  yx_min = Utility::LinLinLog::calculateIntermediateProcessedGridLimit(
                           x0, x1, x, y0_min, y1_min );

  FRENSIE_CHECK_FLOATING_EQUALITY( yx_min, -10.0, 1e-15 );

  x = Utility::LinLinLog::processFirstIndepVar(1.0);

  yx_min = Utility::LinLinLog::calculateIntermediateProcessedGridLimit(
                           x0, x1, x, y0_min, y1_min );

  FRENSIE_CHECK_FLOATING_EQUALITY( yx_min, 10.0, 1e-15 );
}

//---------------------------------------------------------------------------//
// Check that the linear-linear-log unit base interpolation policy between
// four points can be done
FRENSIE_UNIT_TEST_TEMPLATE_EXPAND( LinLinLog,
                                   interpolateProcessedUnitBase_separate_tuple_grids,
                                   TestTwoTupleTwoElementTypes )
{
  FETCH_TEMPLATE_PARAM( 0, WrappedYMember );
  FETCH_TEMPLATE_PARAM( 1, ytuple );
  FETCH_TEMPLATE_PARAM( 2, WrappedZMember );
  FETCH_TEMPLATE_PARAM( 3, ztuple );

  constexpr const size_t ymember = WrappedYMember::value;
  constexpr const size_t zmember = WrappedZMember::value;

  double x0 = Utility::LinLinLog::processFirstIndepVar(0.1);
  double x1 = Utility::LinLinLog::processFirstIndepVar(1.0);
  double x = Utility::LinLinLog::processFirstIndepVar(0.3);
  double y = 0.0;

  std::vector<ytuple> y_0_grid( 4 );
  Utility::set<ymember>( y_0_grid[0], -2.0 );
  Utility::set<ymember>( y_0_grid[1], -1.0 );
  Utility::set<ymember>( y_0_grid[2], 1.0 );
  Utility::set<ymember>( y_0_grid[3], 2.0 );

  std::vector<ztuple> z_0_grid( 4 );
  Utility::set<zmember>( z_0_grid[0], 0.0 );
  Utility::set<zmember>( z_0_grid[1], 1.0 );
  Utility::set<zmember>( z_0_grid[2], 2.0 );
  Utility::set<zmember>( z_0_grid[3], 3.0 );

  std::vector<ytuple> y_1_grid( 3 );
  Utility::set<ymember>( y_1_grid[0], -1.0 );
  Utility::set<ymember>( y_1_grid[1], 0.0 );
  Utility::set<ymember>( y_1_grid[2], 2.0 );

  std::vector<ztuple> z_1_grid( 3 );
  Utility::set<zmember>( z_1_grid[0], 1.0 );
  Utility::set<zmember>( z_1_grid[1], 2.0 );
  Utility::set<zmember>( z_1_grid[2], 3.0 );

  double z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.6830508003062505, 1e-12 );

  y = -1.5228787452803376; // min possible y at x = 0.3

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.40630514634561576, 1e-12 );

  y = 2.0; // max possible y at x = 0.3

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );

  y = 0.0;
  x = Utility::LinLinLog::processFirstIndepVar(0.1);

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.5, 1e-12 );

  y = -2.0;

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.0, 1e-12 );

  y = 2.0;

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );

  y = 0.0;
  x = Utility::LinLinLog::processFirstIndepVar(1.0);

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 2.0, 1e-12 );

  y = -1.0;

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.0, 1e-12 );

  y = 2.0;

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      y_0_grid.begin(),
                                  y_0_grid.end(),
                                  z_0_grid.begin(),
                                  z_0_grid.end(),
                                  y_1_grid.begin(),
                                  y_1_grid.end(),
                                  z_1_grid.begin(),
                                  z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the linear-linear-log unit base interpolation policy between
// four points can be done
FRENSIE_UNIT_TEST_TEMPLATE_EXPAND( LinLinLog,
                                   interpolateProcessedUnitBase_combined_tuple_grids,
                                   TestTupleTwoElementTypes )
{
  FETCH_TEMPLATE_PARAM( 0, WrappedYMember );
  FETCH_TEMPLATE_PARAM( 1, WrappedZMember );
  FETCH_TEMPLATE_PARAM( 2, tuple );

  constexpr const size_t ymember = WrappedYMember::value;
  constexpr const size_t zmember = WrappedZMember::value;
  
  double x0 = Utility::LinLinLog::processFirstIndepVar(0.1);
  double x1 = Utility::LinLinLog::processFirstIndepVar(1.0);
  double x = Utility::LinLinLog::processFirstIndepVar(0.3);
  double y = 0.0;

  std::vector<tuple> grid_0( 4 );
  Utility::set<ymember>( grid_0[0], -2.0 );
  Utility::set<ymember>( grid_0[1], -1.0 );
  Utility::set<ymember>( grid_0[2], 1.0 );
  Utility::set<ymember>( grid_0[3], 2.0 );
  Utility::set<zmember>( grid_0[0], 0.0 );
  Utility::set<zmember>( grid_0[1], 1.0 );
  Utility::set<zmember>( grid_0[2], 2.0 );
  Utility::set<zmember>( grid_0[3], 3.0 );

  std::vector<tuple> grid_1( 3 );
  Utility::set<ymember>( grid_1[0], -1.0 );
  Utility::set<ymember>( grid_1[1], 0.0 );
  Utility::set<ymember>( grid_1[2], 2.0 );
  Utility::set<zmember>( grid_1[0], 1.0 );
  Utility::set<zmember>( grid_1[1], 2.0 );
  Utility::set<zmember>( grid_1[2], 3.0 );

  double z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.6830508003062505, 1e-12 );

  y = -1.5228787452803376; // min possible y at x = 0.3

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.40630514634561576, 1e-12 );

  y = 2.0; // max possible y at x = 0.3

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );

  y = 0.0;
  x = Utility::LinLinLog::processFirstIndepVar(0.1);

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.5, 1e-12 );

  y = -2.0;

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.0, 1e-12 );

  y = 2.0;

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );

  y = 0.0;
  x = Utility::LinLinLog::processFirstIndepVar(1.0);

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 2.0, 1e-12 );

  y = -1.0;

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.0, 1e-12 );

  y = 2.0;

  z = Utility::LinLinLog::interpolateProcessedUnitBase<ymember,zmember>(
                                  x0,
                                  x1,
                                  x,
                                  y,
                                      grid_0.begin(),
                                  grid_0.end(),
                                  grid_1.begin(),
                                  grid_1.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );
}

//---------------------------------------------------------------------------//
// Check that the linear-linear-log unit base interpolation policy between
// four points can be done
FRENSIE_UNIT_TEST( LinLinLog, interpolateProcessedUnitBase_no_tuple_grids )
{
  double x0 = Utility::LinLinLog::processFirstIndepVar(0.1);
  double x1 = Utility::LinLinLog::processFirstIndepVar(1.0);
  double x = Utility::LinLinLog::processFirstIndepVar(0.3);
  double y = 0.0;

  std::vector<double> y_0_grid( 4 );
  y_0_grid[0] = -2.0;
  y_0_grid[1] = -1.0;
  y_0_grid[2] = 1.0;
  y_0_grid[3] = 2.0;

  std::vector<double> z_0_grid( 4 );

  z_0_grid[0] = 0.0;
  z_0_grid[1] = 1.0;
  z_0_grid[2] = 2.0;
  z_0_grid[3] = 3.0;

  std::vector<double> y_1_grid( 3 );
  y_1_grid[0] = -1.0;
  y_1_grid[1] = 0.0;
  y_1_grid[2] = 2.0;

  std::vector<double> z_1_grid( 3 );
  z_1_grid[0] = 1.0;
  z_1_grid[1] = 2.0;
  z_1_grid[2] = 3.0;

  double z = Utility::LinLinLog::interpolateProcessedUnitBase(
                              x0,
                              x1,
                              x,
                              y,
                              y_0_grid.begin(),
                              y_0_grid.end(),
                              z_0_grid.begin(),
                              z_0_grid.end(),
                              y_1_grid.begin(),
                              y_1_grid.end(),
                              z_1_grid.begin(),
                              z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.6830508003062505, 1e-12 );

  y = -1.5228787452803376; // min possible y at x = 0.3

  z = Utility::LinLinLog::interpolateProcessedUnitBase( x0,
                            x1,
                            x,
                            y,
                            y_0_grid.begin(),
                            y_0_grid.end(),
                            z_0_grid.begin(),
                            z_0_grid.end(),
                            y_1_grid.begin(),
                            y_1_grid.end(),
                            z_1_grid.begin(),
                            z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.40630514634561576, 1e-12 );

  y = 2.0; // max possible y at x = 0.3

  z = Utility::LinLinLog::interpolateProcessedUnitBase( x0,
                            x1,
                            x,
                            y,
                            y_0_grid.begin(),
                            y_0_grid.end(),
                            z_0_grid.begin(),
                            z_0_grid.end(),
                            y_1_grid.begin(),
                            y_1_grid.end(),
                            z_1_grid.begin(),
                            z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );

  y = 0.0;
  x = Utility::LinLinLog::processFirstIndepVar(0.1);

  z = Utility::LinLinLog::interpolateProcessedUnitBase( x0,
                            x1,
                            x,
                            y,
                            y_0_grid.begin(),
                            y_0_grid.end(),
                            z_0_grid.begin(),
                            z_0_grid.end(),
                            y_1_grid.begin(),
                            y_1_grid.end(),
                            z_1_grid.begin(),
                            z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.5, 1e-12 );

  y = -2.0;

  z = Utility::LinLinLog::interpolateProcessedUnitBase( x0,
                            x1,
                            x,
                            y,
                            y_0_grid.begin(),
                            y_0_grid.end(),
                            z_0_grid.begin(),
                            z_0_grid.end(),
                            y_1_grid.begin(),
                            y_1_grid.end(),
                            z_1_grid.begin(),
                            z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 0.0, 1e-12 );

  y = 2.0;

  z = Utility::LinLinLog::interpolateProcessedUnitBase( x0,
                            x1,
                            x,
                            y,
                            y_0_grid.begin(),
                            y_0_grid.end(),
                            z_0_grid.begin(),
                            z_0_grid.end(),
                            y_1_grid.begin(),
                            y_1_grid.end(),
                            z_1_grid.begin(),
                            z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );

  y = 0.0;
  x = Utility::LinLinLog::processFirstIndepVar(1.0);

  z = Utility::LinLinLog::interpolateProcessedUnitBase( x0,
                            x1,
                            x,
                            y,
                            y_0_grid.begin(),
                            y_0_grid.end(),
                            z_0_grid.begin(),
                            z_0_grid.end(),
                            y_1_grid.begin(),
                            y_1_grid.end(),
                            z_1_grid.begin(),
                            z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 2.0, 1e-12 );

  y = -1.0;

  z = Utility::LinLinLog::interpolateProcessedUnitBase( x0,
                            x1,
                            x,
                            y,
                            y_0_grid.begin(),
                            y_0_grid.end(),
                            z_0_grid.begin(),
                            z_0_grid.end(),
                            y_1_grid.begin(),
                            y_1_grid.end(),
                            z_1_grid.begin(),
                            z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 1.0, 1e-12 );

  y = 2.0;

  z = Utility::LinLinLog::interpolateProcessedUnitBase( x0,
                            x1,
                            x,
                            y,
                            y_0_grid.begin(),
                            y_0_grid.end(),
                            z_0_grid.begin(),
                            z_0_grid.end(),
                            y_1_grid.begin(),
                            y_1_grid.end(),
                            z_1_grid.begin(),
                            z_1_grid.end() );

  FRENSIE_CHECK_FLOATING_EQUALITY( z, 3.0, 1e-12 );
}

//---------------------------------------------------------------------------//
// end tstLinLinLogTwoDInterpolationPolicy.cpp
//---------------------------------------------------------------------------//
