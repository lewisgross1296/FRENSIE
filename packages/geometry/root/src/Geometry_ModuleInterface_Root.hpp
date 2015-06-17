//---------------------------------------------------------------------------//
//! 
//! \file   Geometry_ModuleInterface_Root.hpp
//! \author Alex Robinson, Eli Moll
//! \brief  Root specialization decl. of the geometry ModuleInterface class
//!
//---------------------------------------------------------------------------//

#ifndef GEOMETRY_MODULE_INTERFACE_ROOT_HPP
#define GEOMETRY_MODULE_INTERFACE_ROOT_HPP

// Root Includes
#include <TObjArray.h>
#include <TGeoNavigator.h>
#include <TGeoNode.h>
#include <RtypesCore.h>

// Trilinos Includes
#include <Teuchos_ScalarTraits.hpp>
#include <Teuchos_Array.hpp>

// FRENSIE Includes
#include "Geometry_ModuleInterfaceDecl.hpp"
#include "Geometry_Root.hpp"
#include "Utility_ExceptionTestMacros.hpp"
#include "Utility_ContractException.hpp"
#include "Utility_GlobalOpenMPSession.hpp"

namespace Geometry{

/*! The specialization of the GeometryModuleInterface class for the Root 
 * geometry handler.
 * \ingroup geometry_module
 */ 
template<>
class ModuleInterface<Root>
{
Public:

  //! The external surface id class (used within the geometry handler)
  typedef Int_t ExternalSurfaceId;
  //! The external cell id class (used within the geometry handler )
  typedef Int_t ExternalCellId;
  
  //! The external surface handle class (used within the geometry handler)
  typedef Int_t ExternalSurfaceHandle;
  //! The external cell handle class (used within the geometry handler)
  typedef Int_t ExternalCellHandle;
  
  //! The internal surface handle class (used within FRENSIE)
  typedef ModuleTraits::InternalSurfaceHandle InternalSurfaceHandle;
  //! The internal cell handle class (used within FRENSIE)
  typedef ModuleTraits::InternalCellHandle InternalCellHandle;

private:
  
  //! Typedef for Teuchos::ScalarTraits
  typedef Teuchos::ScalarTraits<double> ST;

public:

  //! The value of an invalid surface handle
  static const ExternalSurfaceHandle invalid_external_surface_handle;

  //! The value of an invalid cell handle
  static const ExternalCellHandle invalid_external_cell_handle; 

  //! Set the geometry handler instance
  static void setHandlerInstance( TGeoManager* handler_instance );

  //! Do just in time initialization of interface members
  static void initialize();

  //! Enable support for multiple threads
  static void enableThreadSupport( const unsigned num_threads );

  //! Find the cell that contains a given point (start of history)
  static InternalCellHandle findCellContainingPoint( const Ray& ray );

  //! Find the cell that contains a given point (surface crossing)
  static InternalCellHandle findCellContainingPoint( 
					 const Ray& ray,
					 const InternalCellHandle current_cell,
					 const InternalSurfaceHandle surface );

  //! Fire a ray through the geometry
  static void fireRay( const Ray& ray,
		       const InternalCellHandle& current_cell,
		       InternalSurfaceHandle& surface_hit,
		       double& distance_to_surface_hit );

  //! Initialize a new ray (after a collision)
  static void newRay();

  //! Check if the cell is a termination cell
  static bool isTerminationCell( const InternalCellHandle cell );

  //! Get the point location w.r.t. a given cell
  static PointLocation getPointLocation( const Ray& ray,
					 const InternalCellHandle cell );

  //! Calculate the surface normal at a point on the surface
  static void getSurfaceNormal( const InternalSurfaceHandle surface,
				const double position[3],
				double normal[3] );

  //! Assign unique identites to all cells
  static void assignCellIds();
  
  //! Calculate and store the volumes of all cells
  static void storeCellVolumes();

  //! Get the volume of a cell
  static double getCellVolume( const InternalCellHandle cell );
  
  //! Get the surface area of a surface bounding a cell
  static double getCellSurfaceArea( const InternalSurfaceHandle surface,
				    const InternalCellHandle cell );

  //! Check that an external surface handle exists
  static bool doesSurfaceExist( const ExternalSurfaceId surface );

  //! Check that an external cell handle exists
  static bool doesCellExist( const ExternalCellId cell );

  //! Get the internal surf. handle corresponding to the external surf. handle
  static InternalSurfaceHandle getInternalSurfaceHandle(
				const ExternalSurfaceHandle surface_external );

  //! Get the internal surf. handle corresponding to the external surf. id
  static InternalSurfaceHandle getInternalSurfaceHandle(
					  const ExternalSurfaceId surface_id );

  //! Get the internal cell handle corresponding to the external cell handle
  static InternalCellHandle getInternalCellHandle( 
				      const ExternalCellHandle cell_external );

  //! Get the internal cell handle corresponding to the external cell id
  static InternalCellHandle getInternalCellHandle(
						const ExternalCellId cell_id );

  //! Get the external surf. handle corresponding to the internal surf. handle
  static ExternalSurfaceHandle getExternalSurfaceHandle(
					 const InternalSurfaceHandle surface );
  

  //! Get the external cell handle corresponding to the internal cell handle
  static ExternalCellHandle getExternalCellHandle(
					       const InternalCellHandle cell );

private:

  // An instance of TGeoManager
  static TGeoManager* const tgeomanager_instance;
  
  // Teuchos array of volumes
  static Teuchos::Array<double> tvolumes;
  
    // The map of cell ids and cell entity handles
  static boost::unordered_map<InternalCellHandle,ExternalCellHandle> 
  cell_handle_map;

};

// Set the geometry handler instance
/*! \details ROOT returns gGeoManager which handles the specific geometry 
     loaded into ROOT.
 */
inline void ModuleInterface<Root>::setHandlerInstance( 
			    TGeoManager* handler_instance )
{ /* ... */ }

// Initialize a new ray (after a collision)
/*! \details ROOT has a class, FindNextBoundary() which takes a position and
 *  direction to determine where the next boundary crossed is, but strict ray
 *  creation is not required.
 */
inline void ModuleInterface<Root>::newRay()
{ /* ... */ }


// Check if the cell is a termination cell
inline bool ModuleInterface<Root>::isTerminationCell( 
						const InternalCellHandle cell )
{ 
  ExternalCellHandle cell_external = 
    ModuleInterface<Root>::getExternalCellHandle( cell );

  return false; // TODO
}

// Calculate the surface normal at a point on the surface
/* \details This function will not modify normal[3] if the point is not on a 
 *  boundary.
 */
inline void ModuleInterface<Root>::getSurfaceNormal( 
					   const InternalSurfaceHandle surface,
					   const double position[3],
					   double normal[3] )
{
  if tgeomanager_instance->GetCurrentNavigator()->IsOnBoundary()
  {
    Root::Double_t* normal_t = tgeomanager_instance->FindNormal();
    
    normal[0] = normal_t[0];
    normal[1] = normal_t[1];
    normal[2] = normal_t[2];
  }
}

// Get the volume of a cell
inline double ModuleInterface<Root>::getCellVolume( 
						const InternalCellHandle cell )
{
  double volume = 0.0;
  
  ExternalCellHandle cell_external = 
    ModuleInterface<Root>::getExternalCellHandle( cell );
  
  return tvolumes[cell_external];
}

// Get the surface area of a surface bounding a cell
/*! \details Currently Root cannot calculate the surface area of most bounded
 *  volumes.
 */ 
inline double ModuleInterface<Root>::getCellSurfaceArea( 
					   const InternalSurfaceHandle surface,
					   const InternalCellHandle )
{
  return 1;
}

// Check that an external cell handle exists
inline bool ModuleInterface<Root>::doesCellExist(
						    const ExternalCellId cell )
{
  return true;
}

// Get the internal surf. handle corresponding to the external surf. handle
inline ModuleInterface<Root>::InternalSurfaceHandle 
ModuleInterface<Root>::getInternalSurfaceHandle(
				 const ExternalSurfaceHandle surface_external )
{ /* ... */ } // TODO

// Get the internal surf. handle corresponding to the external surf. handle
inline ModuleInterface<Root>::InternalSurfaceHandle 
ModuleInterface<Root>::getInternalSurfaceHandle(
				           const ExternalSurfaceId surface_id )
{ /* ... */ } // TODO

// Get the internal cell handle corresponding to the external cell handle
inline ModuleInterface<Root>::InternalCellHandle 
ModuleInterface<Root>::getInternalCellHandle( 
				       const ExternalCellHandle cell_external )
{ /* ... */ } // TODO

// Get the internal cell handle corresponding to the external cell handle
inline ModuleInterface<Root>::InternalCellHandle 
ModuleInterface<Root>::getInternalCellHandle( 
				                 const ExternalCellId cell_id )
{ /* ... */ } // TODO

// Get the external surf. handle corresponding to the internal surf. handle
inline ModuleInterface<Root>::ExternalSurfaceHandle 
ModuleInterface<Root>::getExternalSurfaceHandle(
					  const InternalSurfaceHandle surface )
{ /* ... */ } // TODO

// Get the external cell handle corresponding to the internal cell handle
inline ModuleInterface<Root>::ExternalCellHandle 
ModuleInterface<Root>::getExternalCellHandle(
					        const InternalCellHandle cell )
{ /* ... */ } // TODO

} // end Geometry namespace


#endif // end GEOMETRY_MODULE_INTERFACE_ROOT_HPP

//---------------------------------------------------------------------------//
// end Geometry_ModuleInterface_Root.hpp
//---------------------------------------------------------------------------//

