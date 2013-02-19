/*
 * Mermelin, Copyright © 2013 by Eduardo Hahn Paredes (mermelin@outlook.com)
 * 
 * Mermelin is free software: You can redistribute it and/or modify it under the terms of the 
 * GNU General Public License as published by the Free Software Foundation, either version 3 of 
 * the License, or (at your option) any later version.
 * Mermelin is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without 
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with Mermelin. 
 * Look for a file named COPYING. If it's not there, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file Event.h
 * @brief Defines the different types of events that can be used.
 *
 * @author Patrick Joos, Eduardo Hahn Paredes
 */

#ifndef EVENT_H
#define EVENT_H
namespace CotopaxiEngine {

    // forward declarations
    class BaseComponent;
    class Entity;

    /**
     * @class Event
     * @brief Defines Event Types for internal communication.
     *
     * It's an almost empty container for messaging between different classes. An Entity* can be 
     * used as additional information if needed.
     */
    class Event
    {
    public:

        /**
         * @enum EventType
         * Defines all types of events
         * - COLLISION_ENTER            Collistion occured (started)
         * - COLLISION                  General collision event
         * - COLLISION_EXIT             Collision ended
         * - PHYSICS_UPDATE             Update the physics-world
         * - TRANSLATE                  A translation has occured (in level-parsing for example)
         * - TRIGGER                    Used by LogicComponents (trigger has been fired)
         * - CONDITION_FULFILLED        Used by LogicComponents (condition has been fulfilled)
         * - ANIMATION_STARTED          The mesh animation has started
         * - ANIMATION_ENDED            The mesh animation has ended
         */
        enum EventType
        {
            COLLISION_ENTER,
            COLLISION,
            COLLISION_EXIT,
            PHYSICS_UPDATE,
            TRANSLATE,
            TRIGGER,
            CONDITION_FULFILLED,
            ANIMATION_STARTED,
            ANIMATION_ENDED,
            LEVEL_END
        };

        Event(EventType);

        /**
         * @fn getType
         * @return the type of an event
         */
        virtual EventType getType() const;

        /** Can be used to access the Entity that produced the Event. */
        Entity* entity;
        BaseComponent* component;
    private:
        EventType type;
    };

    class EventListener
    {
    public:
        virtual void receiveEvent(Event* e) = 0;
    };
}

#endif