//---------------------------------------------------------------------------//
// \file   Cell.cpp
// \author Alex Robinson
// \brief  Cell class definition
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <string>
#include <sstream>
#include <map>
#include <list>
#include <set>
#include <iterator>
#include <algorithm>
#include <cmath>

// Trilinos Includes
#include <Teuchos_Array.hpp>
#include <Teuchos_ArrayRCP.hpp>
#include <Teuchos_Tuple.hpp>
#include <Teuchos_RCP.hpp>

// FACEMC Includes
#include "Cell.hpp"
#include "Surface.hpp"
#include "Tuple.hpp"
#include "LinearAlgebraAlgorithms.hpp"
#include "ContractException.hpp"
#include "FACEMC_Assertion.hpp"

namespace FACEMC{

//! Constructor
Cell::Cell( unsigned id,
	    std::string &cell_definition,
	    std::map<unsigned,Teuchos::RCP<Surface> > &global_surface_map,
	    bool calculate_geometric_data )
  : d_id( id ), 
    d_cell_definition_evaluator( cell_definition ), 
    d_volume( 0.0 ),
    d_geometric_data_calculated(calculate_geometric_data)
{
  simplifyCellDefinitionString( cell_definition );
  assignSurfaces( cell_definition, global_surface_map );
  
  if( calculate_geometric_data )
    calculateVolumeAndSurfaceAreas();
  else
  {    
    Teuchos::Array<Pair<Teuchos::RCP<Surface>,Surface::Sense> >::const_iterator
      surface, end_surface;
    surface = d_surfaces.begin();
    end_surface = d_surfaces.end();

    unsigned surface_id = surface->first->getId();

    while( surface != end_surface )
    {
      if( d_surface_areas.count( surface->first->getId() ) == 0 )
	d_surface_areas[surface_id] = 0.0;

      ++surface;
    }
  }      
}

//! Return if the point is in the cell
bool Cell::isIn( const Vector &point ) const
{
  Teuchos::Array<Pair<Teuchos::RCP<Surface>,Surface::Sense> >::const_iterator
    surface, end_surface;
  surface = d_surfaces.begin();
  end_surface = d_surfaces.end();

  // Sense of the point with respect to the surface of interest
  short sense;

  // If the sense matches the cell surface sense, true is added to this array
  // else false is added (The surface ordering is the same as in the d_surfaces
  // array)
  Teuchos::ArrayRCP<bool> sense_tests( d_surfaces.size() );
  Teuchos::ArrayRCP<bool>::iterator test = sense_tests.begin();

  while( surface != end_surface )
  {
    sense = surface->first->getSense( point );
    
    if( sense == surface->second )
      *test = true;
    else
      *test = false;
    
    ++surface;
    ++test;
  }

  return d_cell_definition_evaluator( sense_tests );
}

//! Return if the point is on the cell
bool Cell::isOn( const Vector &point ) const
{
  Teuchos::Array<Pair<Teuchos::RCP<Surface>,Surface::Sense> >::const_iterator
    surface, end_surface;
  surface = d_surfaces.begin();
  end_surface = d_surfaces.end();

  // Sense of the point with respect to the surface of interest
  short sense;

  // If the sense matches the cell surface sense, true is added to this array
  // else false is added (The surface ordering is the same as in the d_surfaces
  // array)
  Teuchos::ArrayRCP<bool> sense_tests( d_surfaces.size() );
  Teuchos::ArrayRCP<bool>::iterator test = sense_tests.begin();

  while( surface != end_surface )
  {
    sense = surface->first->getSense( point );
    
    if( sense == surface->second || sense == 0 )
      *test = true;
    else
      *test = false;
    
    ++surface;
    ++test;
  }

  return d_cell_definition_evaluator( sense_tests );
}

//! Return the volume of the cell
double Cell::getVolume() const
{
  // The volume must have been calculated
  testPrecondition( d_geometric_data_calculated );
    
  return d_volume;
}

//! Return the area of a surface bounding the cell
double Cell::getSurfaceArea( unsigned surface_id ) const
{
  // The surface areas must have been calculated
  testPrecondition( d_geometric_data_calculated );
  // The requested surface must be present in the cell
  testPrecondition( d_surface_areas.count( surface_id ) > 0 );

  std::map<unsigned,double>::const_iterator area, end;
  end = d_surface_areas.end();
  area = d_surface_areas.find( surface_id );

  if( area != end )
    return area->second;
  else
    return 0.0;
}
  

//! Strip the cell definition string of set operation characters
void Cell::simplifyCellDefinitionString( std::string &cell_definition )
{
  // The cell definition must be valid
  testPrecondition( cell_definition.find_first_not_of( "0123456789-nu() ", 0 ) 
		    == std::string::npos );
  // The cell definition must not contain free floating negative signs
  testPrecondition( cell_definition.find( "- ", 0 ) == std::string::npos );
  // The cell definition must not contain any double negative signs
  testPrecondition( cell_definition.find( "--", 0 ) == std::string::npos );

  std::string operation_characters( "nu()" );

  unsigned operation_loc = 
    cell_definition.find_first_of( operation_characters );

  // Remove the set operation characters
  while( operation_loc < cell_definition.size() )
  {
    cell_definition[operation_loc] = ' ';

    operation_loc = cell_definition.find_first_of( operation_characters,
						   operation_loc+1 );
  }
  
  // Make sure that the cell definition is free of trailing white space
  unsigned end_white_space_loc = 
    cell_definition.find_last_of( "0123456789" ) + 1;

  if( end_white_space_loc < cell_definition.size() )
  {
    cell_definition.erase( end_white_space_loc, 
			   cell_definition.size() - end_white_space_loc );  
  }
}

//! Assign surfaces to the cell
void Cell::assignSurfaces( std::string &cell_definition,
			   std::map<unsigned,Teuchos::RCP<Surface> > &global_surface_map )
{
  // The cell_definition must be simplified
  testPrecondition( cell_definition.find_first_of( "nu()", 0 ) == 
		    std::string::npos );
  // A surface cannot have an id of 0
  testPrecondition( cell_definition.find( " 0 " ) == std::string::npos );
  testPrecondition( cell_definition.find( " -0 " ) == std::string::npos );

  std::map<unsigned,Teuchos::RCP<Surface> >::const_iterator global_surface;
  std::map<unsigned,Teuchos::RCP<Surface> >::const_iterator 
    global_end_surface = global_surface_map.end();
  Pair<Teuchos::RCP<Surface>,Surface::Sense> local_surface;

  std::stringstream simplified_cell_definition( cell_definition );
  int surface_sense_id;
  unsigned surface_id;
  Surface::Sense surface_sense;

  while( simplified_cell_definition )
  {
    simplified_cell_definition >> surface_sense_id;
    
    if( surface_sense_id < 0 )
      surface_sense = -1;
    else
      surface_sense = 1;

    surface_id = abs( surface_sense_id );

    // Pull the surface from the global map
    global_surface = global_surface_map.find( surface_id );
    
    // If the surface was not found exit the program
    FACEMC_ASSERT_ALWAYS_MSG( global_surface != global_end_surface, "Fatal Error: Surface " << surface_id << " requested by Cell " << d_id << " does not exist." );

    // Store this surface
    local_surface.first = global_surface->second;
    local_surface.second = surface_sense;
    d_surfaces.push_back( local_surface );
    
    // Remove the next whitespace from the cell_definition
    simplified_cell_definition.ignore();
  }
}

//! Calculate the cell volume and surface areas of the bounding surfaces
void Cell::calculateVolumeAndSurfaceAreas()
{
  // Check if the cell is a Polyhedron
  bool is_polyhedron = true;
  
  Teuchos::Array<Pair<Teuchos::RCP<Surface>,Surface::Sense> >::const_iterator
    surface, end_surface;
  surface = d_surfaces.begin();
  end_surface = d_surfaces.end();

  while( surface != end_surface )
  {
    if( !surface->first->isPlanar() )
    {
      is_polyhedron = false;
      break;
    }

    ++surface;
  }

  if( is_polyhedron )
    calculatePolyhedralCellVolumeAndSurfaceAreas();
  else
  {
    // Calculate the rotationally symmetric cell volume and surface areas
    bool success = calculateRotationallySymmetricCellVolumeAndSurfaceAreas();

    // Calculate the generic cell volume and surface areas using brute force
    // Monte Carlo integration
    if( !success )
      calculateVolumeAndSurfaceAreasUsingMonteCarlo();
  }
}  

//! Calculate the polyhedron volume and surface areas
void Cell::calculatePolyhedralCellVolumeAndSurfaceAreas()
{
  // Reference surface for calculating volume of polyhedron 
  Surface reference_surface = *d_surfaces[0].first;
  
  // Z-axis unit normal
  Surface::Vector z_axis = Teuchos::tuple( 0.0, 0.0, 1.0 );

  // Processed surfaces (the same surface may appear more than once in a cell
  // definition)
  std::set<unsigned> processed_surfaces;

  // For every surface, transform the cell so that the surface is the x-y plane
  for( unsigned i = 0; i < d_surfaces.size(); ++i )
  {
    // Only use a surface if it hasn't been processed yet
    if( processed_surfaces.find( d_surfaces[i].first->getId() ) != 
	processed_surfaces.end() )
      continue;
    else
      processed_surfaces.insert( d_surfaces[i].first->getId() );
      
    // Make a copy of the surfaces that make up this cell
    Teuchos::Array<Pair<Surface,Surface::Sense> > copy_surfaces;
    Surface copy_reference_surface = reference_surface;
  
    {
      Teuchos::Array<Pair<Teuchos::RCP<Surface>,Surface::Sense> >::const_iterator surface, end_surface;
      surface = d_surfaces.begin();
      end_surface = d_surfaces.end();
    
      while( surface != end_surface )
      {
	Pair<Surface,Surface::Sense> 
	  copied_surface( *(surface->first), surface->second );
	
	copy_surfaces.push_back( copied_surface );
	
	++surface;
      }
    }
    
    Teuchos::Array<Pair<Surface,Surface::Sense> >::iterator 
      master_surface, surface, end_surface;

    // Specify the surface that will have its area and volume constribution 
    // calculated (master surface)
    master_surface = copy_surfaces.begin();
    std::advance( master_surface, i );

    surface = copy_surfaces.begin();
    end_surface = copy_surfaces.end();

    // Create a rotation matrix that will be used to rotate the master surface 
    // to the z-axis
    Surface::Vector master_unit_normal = 
      master_surface->first.getLinearTermVector();
    LinearAlgebra::normalizeVector( master_unit_normal );

    Surface::Matrix master_rotation_matrix = 
      LinearAlgebra::generateRotationMatrixFromUnitVectors( z_axis,
							    master_unit_normal );
    // Translation vector that will be used to move the rotated master surface
    // to the z=0 plane
    Surface::Vector master_translation_vector;
    if( fabs(master_unit_normal[0]) > 1e-12 )
    {
      master_translation_vector = 
	Teuchos::tuple( -master_surface->first.getConstantTerm()/
			master_unit_normal[0], 0.0, 0.0 );
    }
    else if( fabs(master_unit_normal[1]) > 1e-12 )
    {
      master_translation_vector = 
	Teuchos::tuple( 0.0, -master_surface->first.getConstantTerm()/
			master_unit_normal[1], 0.0 );
    }
    else
    {
      master_translation_vector = 
	Teuchos::tuple( 0.0, 0.0, -master_surface->first.getConstantTerm()/
			master_unit_normal[2] );
    }
      
    // Transform all surfaces
    copy_reference_surface.transformSurface( master_rotation_matrix,
					     master_translation_vector );
    while( surface != end_surface )
    {
      surface->first.transformSurface( master_rotation_matrix,
				       master_translation_vector );
      ++surface;
    }
	  
    // Determine the intersection points with the master surface (z=0 plane)
    std::list<Quad<double,double,unsigned,unsigned> > intersection_points;
    calculatePolygonIntersectionPoints( copy_surfaces,
					intersection_points );

    // Organize the points into a polygon (polygons)
    createPolygon( intersection_points,
		   copy_surfaces );

    std::list<double> polygon_areas;
    
    // Calculate the area of the master surface
    calculatePolygonArea( master_surface->first.getId(),
			  intersection_points,
			  polygon_areas );

    // Calculate the volume contribution from the master surface
    calculatePolygonVolumeContribution( copy_reference_surface,
					intersection_points,
					polygon_areas );
  }
}

//! Calculate the intersection points of planes with the z-axis
void Cell::calculatePolygonIntersectionPoints(
			    const Teuchos::Array<Pair<Surface,Surface::Sense> > 
			    &surfaces,
			    std::list<Quad<double,double,unsigned,unsigned> > 
			    &intersection_points ) const
{
  // Make sure that the intersection points list is empty
  intersection_points.clear();
  
  Teuchos::Array<Pair<Surface,Surface::Sense> >::const_iterator
    master_surface, slave_surface, end_surface;
  master_surface = surfaces.begin();
  end_surface = surfaces.end();

  // Processed surfaces (the same surface may appear more than once in a cell
  // definition)
  std::set<unsigned> processed_surfaces;
  
  while( master_surface != end_surface )
  {
    // Only use a surface if it hasn't been processed yet
    if( processed_surfaces.find( master_surface->first.getId() ) != 
	processed_surfaces.end() )
    {
      ++master_surface;
      continue;
    }
    else
      processed_surfaces.insert( master_surface->first.getId() );
      
    // Check if the master surface is parallel to the z=0 plane
    Surface::Vector master_normal = 
      master_surface->first.getLinearTermVector();
    
    if( fabs(master_normal[0]) < 1e-12 &&
	fabs(master_normal[1]) < 1e-12 )
    {
      ++master_surface;
      continue;
    }
    
    slave_surface = surfaces.begin();

    while( slave_surface != end_surface )
    {
      // Check if the slave surface has already been processed as a master
      if( processed_surfaces.find( slave_surface->first.getId() ) !=
	  processed_surfaces.end() )
      {
	++slave_surface;
	continue;
      }

      // Check if the slave surface is parallel to the z=0 plane
      Surface::Vector slave_normal = 
	slave_surface->first.getLinearTermVector();
      
      if( fabs(slave_normal[0]) < 1e-12 &&
	  fabs(slave_normal[1]) < 1e-12 )
      {
	++slave_surface;
	continue;
      }

      // Check if the slave surface is parallel to the master surface
      LinearAlgebra::normalizeVector( master_normal );
      LinearAlgebra::normalizeVector( slave_normal );

      double mu = master_normal*slave_normal;

      if( fabs(fabs(mu) - 1.0 ) < 1e-12 )
      {
	++slave_surface;
	continue;
      }

      // Calculate the intersection point
      Quad<double,double,unsigned,unsigned> intersection_point;
      
      master_normal = master_surface->first.getLinearTermVector();
      slave_normal = slave_surface->first.getLinearTermVector();
      double master_constant_term = master_surface->first.getConstantTerm();
      double slave_constant_term = slave_surface->first.getConstantTerm();

      // y coordinate
      intersection_point.second = -(slave_normal[0]*master_constant_term -
				    master_normal[0]*slave_constant_term)/
	(slave_normal[0]*master_normal[1] - 
	 master_normal[0]*slave_normal[1]);

      // G1 != 0
      if( fabs(master_normal[0]) > 1e-12 )
      {
	// x coordinate
	intersection_point.first = (-master_constant_term +
	  master_normal[1]*intersection_point.second)/master_normal[0];
      }
      // G2 != 0
      else
      {
	// x coordinate
	intersection_point.first = (-slave_constant_term +
	  slave_normal[1]*intersection_point.second)/slave_normal[0];
      }

      intersection_point.third = master_surface->first.getId();
      intersection_point.fourth = slave_surface->first.getId();

      // Test if the intersection point is real
      bool true_intersection = testIntersectionPoint( surfaces,
						      intersection_point );
      if( true_intersection )
	intersection_points.push_back( intersection_point );

      ++slave_surface;
    }
    
    ++master_surface;
  }
}

//! Test if an intersection point is real
bool Cell::testIntersectionPoint( 
			    const Teuchos::Array<Pair<Surface,Surface::Sense> > 
			    &surfaces,
			    const Quad<double,double,unsigned,unsigned>
			    &intersection_point) const
{
  unsigned short delta_value;

  unsigned test_function_value = 0.0;

  for( unsigned int i = 0; i < 4; ++i )
  {
    bool first_surface_boolean_parameter;
    bool second_surface_boolean_parameter;
	
    if( i == 0 )
    {
      first_surface_boolean_parameter = true;
      second_surface_boolean_parameter = true;
    }
    else if( i == 1 )
    {
      first_surface_boolean_parameter = false;
      second_surface_boolean_parameter = true;
    }
    else if( i == 2 )
    {
      first_surface_boolean_parameter = true;
      second_surface_boolean_parameter = false;
    }
    else
    {
      first_surface_boolean_parameter = false;
      second_surface_boolean_parameter = false;
    }

    Surface::Vector point = Teuchos::tuple( intersection_point.first,
					    intersection_point.second,
					    0.0 );
	  
    Teuchos::Array<Pair<Surface,Surface::Sense> >::const_iterator
      surface, end_surface;
    surface = surfaces.begin();
    end_surface = surfaces.end();
    
    Teuchos::ArrayRCP<bool> sense_tests( surfaces.size() );
    Teuchos::ArrayRCP<bool>::iterator test = sense_tests.begin();
    
    while( surface != end_surface )
    {
      if( surface->first.getId() == intersection_point.third )
	*test = first_surface_boolean_parameter;
      else if( surface->first.getId() == intersection_point.fourth )
	*test = second_surface_boolean_parameter;
      else
      {
	Surface::Sense sense = surface->first.getSense( point );
	
	if( sense == surface->second || sense == 0 )
	  *test = true;
	else
	  *test = false;
      }
      
      ++surface;
      ++test;
    }
    
    bool cell_present = d_cell_definition_evaluator( sense_tests );
    if( cell_present )
      delta_value = 1;
    else
      delta_value = 0; 

    unsigned multiplier;
    if( i == 0 )
      multiplier = 1;
    else if( i == 1 )
      multiplier = 2;
    else if( i == 2 )
      multiplier = 4;
    else
      multiplier = 8;
    
    // Test function value: SUM( delta_i*2^(i-1); i=0..3 )
    test_function_value += delta_value*multiplier;
  }

  // The intersection is only true if the test function value is not 0 modulo 3
  if( test_function_value%3 != 0 )
    return true;
  else
    return false;
}  

void Cell::createPolygon( std::list<Quad<double,double,unsigned,unsigned> >
			  &intersection_points,
			  const Teuchos::Array<Pair<Surface,Surface::Sense> > 
			  &surfaces ) const
{
  // There must be at least three intersection points to create a polygon
  testPrecondition( intersection_points.size() >= 3 );
  
  std::list<Quad<double,double,unsigned,unsigned> > polygon;
  
  std::list<Quad<double,double,unsigned,unsigned> >::const_iterator start_point;
  std::list<Quad<double,double,unsigned,unsigned> >::iterator
    point, next_point, prev_point, end_point;
  point = intersection_points.begin();
  start_point = point;
  end_point = intersection_points.end();

  // There may be multiple disjoint polygons - keep processing them until
  // all intersection points have been processed
  while( intersection_points.size() > 0 )
  {
    unsigned current_surface_id, next_surface_id;
    
    // Find the first three points on the boundary of the polygon
    start_point = initializePolygon( polygon,
				     intersection_points,
				     surfaces,
				     current_surface_id );

    // Complete the rest of the polygon
    prev_point = polygon.end();
    --prev_point;

    // Continue adding points to the polygon until no more points can be found
    // (break from loop if this happens)
    while( true )
    {
      point = intersection_points.begin();
      end_point = intersection_points.end();
      next_point = end_point;
    
      while( point != end_point )
      {
	if( point->third == current_surface_id ||
	    point->fourth == current_surface_id )
	{
	  if( next_point != end_point )
	  {
	    Teuchos::Tuple<double,3> current_distance_vector = 
	      Teuchos::tuple( next_point->first - prev_point->first,
			      next_point->second - prev_point->second,
			      0.0 );
	    
	    Teuchos::Tuple<double,3> new_distance_vector = 
	      Teuchos::tuple( point->first - prev_point->first,
			      point->second - prev_point->second,
			      0.0 );
	  
	    if( LinearAlgebra::vectorMagnitude( new_distance_vector ) <
		LinearAlgebra::vectorMagnitude( current_distance_vector ) )
	    {
	      next_point = point;
	      if( point->third == current_surface_id )
		next_surface_id = point->fourth;
	      else
		next_surface_id = point->third;
	    }
	  }
	  else
	  {
	    next_point = point;
	    if( point->third == current_surface_id )
	      next_surface_id = point->fourth;
	    else
	      next_surface_id = point->third;
	  }
	}
	
	++point;
      }
    
      if( next_point != end_point )
      {
	++prev_point;
	prev_point = polygon.insert( prev_point, *next_point );
	intersection_points.erase( next_point );

	current_surface_id = next_surface_id;
      }
      // The polygon is finished - add a copy of the first point to the end
      // of the polygon point list
      else
      {
	++prev_point;
	prev_point = polygon.insert( prev_point, *start_point );
	break;
      }
    }
  }
  
  // The polygon(s) have been created
  intersection_points = polygon;
}

//! Initialize the polygon
std::list<Quad<double,double,unsigned,unsigned> >::const_iterator
Cell::initializePolygon(
			std::list<Quad<double,double,unsigned,unsigned> > 
			&polygon,
			std::list<Quad<double,double,unsigned,unsigned> > 
			&intersection_points,
			const Teuchos::Array<Pair<Surface,Surface::Sense> > 
			&surfaces,
			unsigned &current_surface_id ) const
{
  // There must be at least three intersection points left to create a polygon
  testPrecondition( intersection_points.size() >= 3 );
  
  std::list<Quad<double,double,unsigned,unsigned> >::iterator
    point, start_point, next_point, prev_point, return_point, end_point;
  point = intersection_points.begin();
  start_point = point;
  end_point = intersection_points.end();
  
  // Find the starting point (upper-rightmost point)
  while( point != end_point )
  {
    if( point->first - start_point->first > 1e-12 )
      start_point = point;
    else if( fabs(point->first - start_point->first) < 1e-12 &&
	     point->second > start_point->second )
      start_point = point;
    
    ++point;
  }

  // Add the start point to the polygon
  polygon.push_back( *start_point );
  point = intersection_points.erase( start_point );

  // Find the two points closest to the starting point on the intersecting 
  // surfaces
  start_point = polygon.end();
  --start_point;
  point = intersection_points.begin();
  next_point = end_point;
  prev_point = end_point;
  while( point != end_point )
  {
    if( point->third == start_point->third ||
	point->fourth == start_point->third )
    {
      if( next_point != end_point )
      {
	Teuchos::Tuple<double,3> current_distance_vector = 
	  Teuchos::tuple( next_point->first - start_point->first,
			  next_point->second - start_point->second,
			  0.0 );

	Teuchos::Tuple<double,3> new_distance_vector = 
	  Teuchos::tuple( point->first - start_point->first,
			  point->second - start_point->second,
			  0.0 );

	if( LinearAlgebra::vectorMagnitude( new_distance_vector ) <
	    LinearAlgebra::vectorMagnitude( current_distance_vector ) )
	  next_point = point;
      }
      else
	next_point = point;
      
      ++point;
      continue;
    }
    if( point->third == start_point->fourth ||
	point->fourth == start_point->fourth )
    {
      if( prev_point != end_point )
      {
	Teuchos::Tuple<double,3> current_distance_vector = 
	  Teuchos::tuple( prev_point->first - start_point->first,
			  prev_point->second - start_point->second,
			  0.0 );

	Teuchos::Tuple<double,3> new_distance_vector = 
	  Teuchos::tuple( point->first - start_point->first,
			  point->second - start_point->second,
			  0.0 );

	if( LinearAlgebra::vectorMagnitude( new_distance_vector ) <
	    LinearAlgebra::vectorMagnitude( current_distance_vector ) )
	  prev_point = point;
      }
      else
	prev_point = point;
      
      ++point;
      continue;
    }

    ++point;
  }

  // Make sure that the next point is the point that would follow the start
  // point assuming a counterclockwise polygon (cell on inside of polygon)
  unsigned prev_surface_id, curr_surface_id, next_surface_id;
  if( next_point->second < prev_point->second )
  {
    std::swap( next_point, prev_point );
    prev_surface_id = start_point->third;
    curr_surface_id = start_point->fourth;
  }
  else
  {
    prev_surface_id = start_point->fourth;
    curr_surface_id = start_point->third;
  }
  
  // Find the previous surface
  Teuchos::Array<Pair<Surface,Surface::Sense> >::const_iterator
    surface, prev_surface, end_surface;
  surface = surfaces.begin();
  end_surface = surfaces.end();
  
  while( surface != end_surface )
  {
    if( surface->first.getId() == prev_surface_id )
    {
      prev_surface = surface;
      break;
    }
    
    ++surface;
  }

  // If the sense of the next point w.r.t. the previous surface equals the cell
  // w.r.t. the previous surface, the polygon is counterclockwise and the 
  // next point is indeed next. O.w. the polygon is clockwise and the previous 
  // point is next.
  Surface::Vector next_point_3d = 
    Teuchos::tuple( next_point->first, next_point->second, 0.0 );

  if( prev_surface->first.getSense( next_point_3d ) == 
      prev_surface->second )
  {
    return_point = polygon.insert( start_point, *prev_point );
    point = intersection_points.erase( prev_point );
    
    if( next_point->third == curr_surface_id )
      next_surface_id = next_point->fourth;
    else
      next_surface_id = next_point->third;
    
    ++start_point;
    start_point = polygon.insert( start_point, *next_point );
    point = intersection_points.erase( next_point );
  }
  else
  {
    return_point = polygon.insert( start_point, *next_point );
    point = intersection_points.erase( next_point );

    if( prev_point->third == prev_surface_id )
      next_surface_id = prev_point->fourth;
    else
      next_surface_id = prev_point->third;
    
    ++start_point;
    start_point = polygon.insert( start_point, *prev_point );
    point = intersection_points.erase( prev_point );
  }

  // The id of the other surface associated with the last point added to the 
  // polygon list must be returned
  current_surface_id = next_surface_id;

  return return_point;
} 

//! Calculate the area of a surface bounding the polyhedral cell
void Cell::calculatePolygonArea( 
	       const unsigned surface_id,
	       const std::list<Quad<double,double,unsigned,unsigned> > &polygon,
	       std::list<double> &polygon_areas )
{
  // The polygon must have at least 4 points (triangle with first point copied)
  testPrecondition( polygon.size() >= 4 );
  
  // The polygon areas list must be empty
  polygon_areas.clear();
  
  std::list<Quad<double,double,unsigned,unsigned> >::const_iterator
    point, next_point, start_point, end_point;
  
  start_point = polygon.begin();
  
  point = start_point;
  next_point = start_point;
  ++next_point;
  
  end_point = polygon.end();

  double area = 0.0;

  while( next_point != end_point )
  {
    double partial_area = (point->first + next_point->first)*
      (next_point->second - point->second);

    ++point;
    ++next_point;
    
    while( *point != *start_point )
    {
      partial_area += (point->first + next_point->first)*
      (next_point->second - point->second);

      ++point;
      ++next_point;
    }

    area += 0.5*partial_area;
    polygon_areas.push_back( 0.5*partial_area );
    
    if( next_point != end_point )
    {
      start_point = next_point;
      point = next_point;
      ++next_point;
    }
  }
  
  // Record this surfaces area relative to this cell
  d_surface_areas[surface_id] = area;
}

//! Calculate the volume contribution from a surface bounding this cell
void Cell::calculatePolygonVolumeContribution(
	       const Surface &reference_surface,
	       const std::list<Quad<double,double,unsigned,unsigned> > &polygon,
	       const std::list<double> &polygon_areas )
{
  std::list<double>::const_iterator polygon_area = polygon_areas.begin();
  
  std::list<Quad<double,double,unsigned,unsigned> >::const_iterator
    point, next_point, start_point, end_point;
  
  start_point = polygon.begin();
  
  point = start_point;
  next_point = start_point;
  ++next_point;
  
  end_point = polygon.end();

  Surface::Vector ref_unit_normal = reference_surface.getLinearTermVector();
  double ref_normal_magnitude = 
    LinearAlgebra::vectorMagnitude( ref_unit_normal );
  LinearAlgebra::normalizeVector( ref_unit_normal );
  double mu = ref_unit_normal[2];

  while( next_point != end_point )
  {
    Pair<double,double> centroid;
    centroid.first = (next_point->first*next_point->first +
		      next_point->first*point->first +
		      point->first*point->first)*
      (next_point->second - point->second);
    
    centroid.second = (next_point->first - point->first)*
      (next_point->second*next_point->second +
       next_point->second*point->second +
       point->second*point->second);

    ++point;
    ++next_point;
    
    while( (*point) != (*start_point) )
    {
      centroid.first += (next_point->first*next_point->first +
		      next_point->first*point->first +
		      point->first*point->first)*
	(next_point->second - point->second);

      centroid.second += (next_point->first - point->first)*
	(next_point->second*next_point->second +
	 next_point->second*point->second +
	 point->second*point->second);

      ++point;
      ++next_point;
    }

    centroid.first /= 6.0*(*polygon_area);
    centroid.second /= 6.0*(*polygon_area);

    double distance = fabs(ref_unit_normal[0]*centroid.first +
			   ref_unit_normal[1]*centroid.second +
			   reference_surface.getConstantTerm()/
			   ref_normal_magnitude);

    // If the polygon area is negative, its volume contribution will
    // be subtracted off
    d_volume += distance*(*polygon_area)*mu;
    
    if( next_point != end_point )
    {
      start_point = next_point;
      point = next_point;
      ++next_point;
      
      ++polygon_area;
    }
  }
}

//! Calculate the rotationally symmetric cell volume and surface areas
bool Cell::calculateRotationallySymmetricCellVolumeAndSurfaceAreas()
{

}

//! Calculate the cell volume and surface areas using monte carlo integration
void Cell::calculateVolumeAndSurfaceAreasUsingMonteCarlo()
{

}

//! Calculate a bounding box for the cell
Cell Cell::calculateBoundingBox() const
{

}

//! Get the surface array
Teuchos::Array<Pair<Teuchos::RCP<Surface>,Surface::Sense> >
Cell::getSurfaceArray() const
{
  return d_surfaces;
}

} // end FACEMC namespace

//---------------------------------------------------------------------------//
// end Cell.cpp
//---------------------------------------------------------------------------//
