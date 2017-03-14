//---------------------------------------------------------------------------//
//!
//! \file   Geometry_DagMCNavigator.hpp
//! \author Alex Robinson
//! \brief  The DagMC navigator class declaration
//!
//---------------------------------------------------------------------------//

#ifndef GEOMETRY_DAGMC_NAVIGATOR_HPP
#define GEOMETRY_DAGMC_NAVIGATOR_HPP

// Boost Includes
#include <boost/bimap.hpp>

// Moab Includes
#include <DagMC.hpp>

// FRENSIE Includes
#include "Geometry_DagMCCellHandler.hpp"
#include "Geometry_DagMCSurfaceHandler.hpp"
#include "Geometry_DagMCRay.hpp"
#include "Geometry_Navigator.hpp"

namespace Geometry{

//! The DagMC ray tracer
class DagMCNavigator : public Navigator
{

public:

  //! The reflecting surface map type
  typedef boost::bimap<ModuleTraits::InternalSurfaceHandle,moab::EntityHandle>
  ReflectingSurfaceIdHandleMap;

  //! Constructor
  DagMCNavigator( moab::DagMC* dagmc_instance,
                  const std::shared_ptr<const Geometry::DagMCCellHandle>&
                  cell_handler,
                  const std::shared_ptr<const Geometry::DagMCSurfaceHandler>&
                  surface_handler,
                  const std::shared_ptr<const ReflectinSurfaceIdHandleMap>&
                  reflecting_surfaces );

  //! Destructor
  ~DagMCNavigator()
  { /* ... */ }

  //! Enable thread support
  void enableThreadSupport( const size_t num_threads ) override;

  //! Get the point location w.r.t. a given cell
  PointLocation getPointLocation(
               const Ray& ray,
               const ModuleTraits::InternalCellHandle cell_id ) const override;

  //! Get the surface normal at a point on the surface
  void getSurfaceNormal( const ModuleTraits::InternalSurfaceHandle surface_id,
                         const double position[3],
                         const double direction[3],
                         double normal[3] ) const override;

  //! Get the boundary cell
  ModuleTraits::InternalCellHandle getBoundaryCell(
         const ModuleTraits::InternalCellHandle cell_id,
         const ModuleTraits::InternalSurfaceHandle boundary_surface_id ) const;

  //! Find the cell that contains the start ray
  ModuleTraits::InternalCellHandle findCellContainingStartRay(
                                    const Ray& ray,
                                    CellSet& start_cell_cache ) const override;

  //! Find the cell that contains the ray
  ModuleTraits::InternalCellHandle findCellContainingRay( const Ray& ray ) const override;

  //! Fire the ray through the geometry
  double fireRay( const Ray& ray,
                  ModuleTraits::InternalSurfaceHandle& surface_hit ) const override;

  //! Check if the internal ray is set
  bool isInternalRaySet() const override;

  //! Initialize (or reset) an internal DagMC ray
  void setInternalRay( const double x_position,
                       const double y_position,
                       const double z_position,
                       const double x_direction,
                       const double y_direction,
                       const double z_direction ) override;
                      
                       
  //! Initialize (or reset) an internal DagMC ray
  void setInternalRay( const double x_position,
                       const double y_position,
                       const double z_position,
                       const double x_direction,
                       const double y_direction,
                       const double z_direction,
                       const ModuleTraits::InternalCellHandle current_cell ) override;

  //! Get the internal DagMC ray position
  const double* getInternalRayPosition() const override;

  //! Get the internal DagMC ray direction
  const double* getInternalRayDirection() const override;

  //! Find the cell containing the internal DagMC ray position
  ModuleTraits::InternalCellHandle findCellContainingInternalRay() override;

  //! Get the distance from the internal DagMC ray pos. to the nearest boundary
  double fireInternalRay( ModuleTraits::InternalSurfaceHandle& surface_hit ) override;

  //! Advance the internal DagMC ray to the next boundary
  bool advanceInternalRayToCellBoundary( double* surface_normal ) override;

  //! Advance the internal DagMC ray a substep
  void advanceInternalRayBySubstep( const double substep_distance ) override;

  //! Change the internal ray direction (without changing its location)
  void changeInternalRayDirection( const double x_direction,
                                   const double y_direction,
                                   const double z_direction ) override;

private:

  // Convert an array to a string
  static std::string arrayToString( const double data[3] );

  // Check if the surface handle is a reflecting surface
  bool isReflectingSurfaceHandle(
                               const moab::EntityHandle surface_handle ) const;

  // Get the point location w.r.t. a given cell
  PointLocation getPointLocation(
                         const double position[3],
                         const double direction[3],
                         const moab::EntityHandle cell_handle,
                         const moab::DagMC::RayHistory* history = NULL ) const;

  // Get the surface normal at a point on the surface
  void getSurfaceHandleNormal(
                         const moab::EntityHandle surface_handle,
                         const double position[3],
                         const double direction[3],
                         double normal[3],
                         const moab::DagMC::RayHistory* history = NULL ) const;

  // Get the boundary cell handle
  moab::EntityHandle getBoundaryCellHandle(
                      const moab::EntityHandle cell_handle,
                      const moab::EntityHandle boundary_surface_handle ) const;

  // Get the internal DagMC ray
  DagMCRay& getInternalRay();

  // Get the internal DagMC ray
  const DagMCRay& getInternalRay() const;

  //! Find the cell that contains the ray
  ModuleTraits::InternalCellHandle findCellContainingRayWithoutBoundaryCheck(
                                                        const Ray& ray ) const;

  // Find the cell handle that contains the ray
  moab::EntityHandle findCellHandleContainingRay(
                                      const Ray& ray,
                                      const bool boundary_check = true ) const;

  // Find the cell handle that contains the external ray
  moab::EntityHandle findCellHandleContainingRay(
                                  const double position[3],
                                  const double direction[3],
                                  const bool check_on_boundary = false ) const;

  // Get the distance from the ray position to the nearest boundary
  double fireRayWithCellHandle(
                               const double position[3],
                               const double direction[3],
                               const moab::EntityHandle current_cell_handle,
                               moab::EntityHandle& surface_hit_handle,
                               moab::DagMC::RayHistory* history = NULL ) const;

  // Set an internal DagMC ray
  void setInternalRay( const double x_position,
                       const double y_position,
                       const double z_position,
                       const double x_direction,
                       const double y_direction,
                       const double z_direction,
                       const moab::EntityHandle current_cell_handle );

  // The boundary tolerance
  static const double s_boundary_tol;

  // The raw DagMC instance
  moab::DagMC* d_dagmc;

  // The DagMC cell handler
  std::shared_ptr<const Geometry::DagMCCellHandler> d_cell_handler;

  // The DagMC surface handler
  std::shared_ptr<const Geometry::DagMCSurfaceHandler> d_surface_handler;

  // The reflecting surfaces
  std::shared_ptr<const ReflectingSurfaceIdHandleMap> d_reflecting_surfaces;

  // The internal rays
  std::vector<DagMCRay> s_internal_rays;
};

/*! The DagMC geometry error
 * \details This error class can be used to record geometry gaps.
 */
class DagMCGeometryError : public std::runtime_error
{

public:

  DagMCGeometryError( const std::string& what_arg )
    : std::runtime_error( what_arg )
  { /* ... */ }
};
  
} // end Geometry namespace

#endif // end GEOMETRY_DAGMC_NAVIGATOR_HPP

//---------------------------------------------------------------------------//
// end Geometry_DagMCNavigator.hpp
//---------------------------------------------------------------------------//
