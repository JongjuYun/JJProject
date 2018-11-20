/*!
 * @file
 * @brief Interface for generic events.  Modeled after C# events/delegates.
 *
 * Events are basically multicast callbacks -- multiple callbacks can be set by multiple actors that
 * have no knowledge of each other.  The Event interface provides the ability for an actor to subscribe
 * or unsubscribe from a given event.
 *
 * Copyright GE Appliances - Confidential - All rights reserved
 */

#ifndef I_EVENT_H
#define I_EVENT_H

#include "EventSubscription.h"

struct I_Event_Api_t;

/*!
 * Generic event interface.
 */
typedef struct
{
   const struct I_Event_Api_t *api;
} I_Event_t;

/*!
 * Methods for interacting with events.
 */
typedef struct I_Event_Api_t
{
   /*!
    * Subscribe to an event using an event subscription.
    * @pre instance != NULL
    * @pre subscription != NULL
    * @param instance The event to subscribe to.
    * @param subscription The event subscription to use to subscribe.
    */
   void (*Subscribe)(I_Event_t *instance, EventSubscription_t *subscription);

   /*!
    * Remove an event subscription from an event.
    * @pre instance != NULL
    * @pre subscription != NULL
    * @param instance The event to unsubscribe from.
    * @param subscription The event subscription that will be removed from the event's subscription list.
    */
   void (*Unsubscribe)(I_Event_t *instance, EventSubscription_t *subscription);
} I_Event_Api_t;

#define Event_Subscribe(instance, subscription) \
   (instance)->api->Subscribe((instance), (subscription))

#define Event_Unsubscribe(instance, subscription) \
   (instance)->api->Unsubscribe((instance), (subscription))

#endif
