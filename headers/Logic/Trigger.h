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
 * @file Trigger.h
 * @brief Is used to fire events.
 *
 * @author Eduardo Hahn Paredes
 */

#ifndef TRIGGER_H
#define	TRIGGER_H

#include "Components/LogicComponent.h"
#include "Condition.h"
#include "Engine.h"

namespace CotopaxiEngine {

    /**
     * @class Trigger
     * 
     * @brief Is used to fire events.
     *
     * It can be fulfilled or not. The @link check @endlink function indicates it.
     */
    class Trigger : public LogicComponent
    {
    public:
        Trigger(std::string name);
        virtual ~Trigger();

        void addTarget(Condition* entity);
        bool check();

        /**
         * @fn fire
         * Sendes Events of type Event::TRIGGER to the registered targets.
         */
        void fire();

        /**
         * @fn receiveEvent
         * Accepts and calles @link fire @endlink if the incoming Event is of type 
         * Event::COLLISION_ENTER.
         * @param event Any Event
         */
        virtual void receiveEvent(Event* event);

        /**
         * @fn setName
         * Every Trigger has to be a name, so it can be identified later.
         * @param name The name to give
         * @attention do not use the same name twice
         */
        void setName(std::string name);

        /**
         * @fn getName
         * Shows the name
         * @return The name and identifier of the Trigger instance
         */
        std::string getName() const;

    private:
        std::string name;
        std::vector<Condition*>* targets;
        bool fired;
    };
}

#endif
