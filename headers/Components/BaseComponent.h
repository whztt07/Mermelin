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
 * @file BaseComponent.h
 * @author Eduardo Hahn Paredes
 * @brief Defines the interface for all our components.
 */

#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include "Entities/Entity.h"

namespace CotopaxiEngine {
	class BaseModule;
    /**
     * @class BaseComponent
     * @brief Every Component should inherit from this class.
     * 
     * It has a pointer to an Entity and can delegate events to it.
     */
	class BaseComponent: public EventListener
    {
    protected:
        
        /** @brief The Entity that has the component */
        Entity* parent;
        
    public:
        virtual ~BaseComponent(){
			parent = NULL;
		}

        /**
         * @fn setParent
         * Setter of the Entity which uses the component.
         * @param parent The entity that has to be set
         */
        virtual void setParent(Entity* parent)
        {
            this->parent = parent;
        }
        
        /**
         * @fn getParent
         * Getter of the Entity which uses the component.
         * @return The entity that the component belongs to.
         */
        virtual Entity* getParent()
        {
            return parent;
        }
        
        /**
         * @fn receiveEvent
         * Passes an event to the component.
         * @param event The event that has to be passed.
         */
        virtual void receiveEvent(Event* event) { }
    };
}

#endif