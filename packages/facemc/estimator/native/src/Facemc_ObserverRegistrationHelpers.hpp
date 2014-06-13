//---------------------------------------------------------------------------//
//!
//! \file   Facemc_ObserverRegistrationHelpers.hpp
//! \author Alex Robinson
//! \brief  Event dispatcher helper function declarations
//!
//---------------------------------------------------------------------------//

#ifndef FACEMC_OBSERVER_REGISTRATION_HELPERS_HPP
#define FACEMC_OBSERVER_REGISTRATION_HELPERS_HPP

// Boost Includes
#include <boost/mpl/find.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/begin_end.hpp>
#include <boost/mpl/contains.hpp>

// FRENSIE Includes
#include "Facemc_ParticleCollidingInCellEventDispatcherDB.hpp"
#include "Facemc_ParticleCrossingSurfaceEventDispatcherDB.hpp"
#include "Facemc_ParticleEnteringCellEventDispatcherDB.hpp"
#include "Facemc_ParticleLeavingCellEventDispatcherDB.hpp"
#include "Facemc_ParticleSubtrackEndingInCellEventDispatcherDB.hpp"

namespace Facemc{

//! Struct for iterating through all observer event tags
template<typename BeginEventTagIterator, typename EndEventTagIterator>
struct ObserverRegistrationHelper
{
  //! Register the observer with dispatchers associated with BeginEventTag tag
  template<typename Observer, typename EntityHandle>
  static void registerObserverWithTag(
			      Teuchos::RCP<Observer>& observer,
			      const Teuchos::Array<EntityHandle>& entity_ids );
};

//! Struct for ending iteration through all event tags
template<typename EndEventTagIterator>
struct ObserverRegistrationHelper<EndEventTagIterator,EndEventTagIterator>
{
  //! End registration iteration
  template<typename Observer, typename EntityHandle>
  static void registerObserverWithTag(
			      Teuchos::RCP<Observer>& observer,
			      const Teuchos::Array<EntityHandle>& entity_ids );
};

//! Register an observer with the appropriate dispatcher
template<typename Observer, typename EntityHandle>
void registerObserver( Teuchos::RCP<Observer>& observer,
		       const Teuchos::Array<EntityHandle>& entity_ids );

/*! Register an observer with the appropriate particle colliding in cell event 
 * dispatcher
 */
template<typename Observer, typename EntityHandle>
void registerObserver( Teuchos::RCP<Observer>& observer,
		       const Teuchos::Array<EntityHandle>& entity_ids,
		       ParticleCollidingInCellEventObserver::EventTag );

/*! Register an observer with the appropriate particle crossing surface event
 * dispatcher
 */
template<typename Observer, typename EntityHandle>
inline void registerObserver( Teuchos::RCP<Observer>& observer,
			      const Teuchos::Array<EntityHandle>& entity_ids,
			      ParticleCrossingSurfaceEventObserver::EventTag );

/*! Register an observer with the appropriate particle entering cell event
 * dispatcher
 */
template<typename Observer, typename EntityHandle>
inline void registerObserver( Teuchos::RCP<Observer>& observer,
			      const Teuchos::Array<EntityHandle>& entity_ids,
			      ParticleEnteringCellEventObserver::EventTag );

/*! Register an observer with the appropriate particle leaving cell event
 * dispatcher
 */
template<typename Observer, typename EntityHandle>
inline void registerObserver( Teuchos::RCP<Observer>& observer,
			      const Teuchos::Array<EntityHandle>& entity_ids,
			      ParticleLeavingCellEventObserver::EventTag );

/*! Register an observer with the appropriate particle subtrack ending in cell
 * event dispatcher
 */
template<typename Observer, typename EntityHandle>
inline void registerObserver( 
			 Teuchos::RCP<Observer>& observer,
			 const Teuchos::Array<EntityHandle>& entity_ids,
			 ParticleSubtrackEndingInCellEventObserver::EventTag );

} // end Facemc namespace

//! Macro for simple registering of observers
#define REGISTER_OBSERVER_WITH_DISPATCHERS( observer, entity_ids ) \
  Facemc::registerObserver( observer, entity_ids ) 

//---------------------------------------------------------------------------//
// Template includes.
//---------------------------------------------------------------------------//

#include "Facemc_ObserverRegistrationHelpers_def.hpp"

//---------------------------------------------------------------------------//

#endif // end FACEMC_OBSERVER_REGISTRATION_HELPERS_HPP

//---------------------------------------------------------------------------//
// end Facemc_ObserverRegistrationHelpers.hpp
//---------------------------------------------------------------------------//
