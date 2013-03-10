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
@file BaseModule.h
@Contains the BaseModule

@author Eduardo Hahn Paredes
 */

#ifndef BASEMODULE_H
#define BASEMODULE_H
#include "stdafx.h"
#include "Event.h"

namespace CotopaxiEngine {

    class BaseComponent; //Forward delcaration
    class Entity;

    /**
    @class BaseModule
    @brief BaseModule implemented by all Modules
	
    This is our base class for all our modules. It implements a framelistener from Ogre, so that we can use Ogre for our gameloop.
    Each Module inherits from this, so this serves as a common interface for all modules.
     */
    class BaseModule : public EventListener
    {
    public:
        /**
         * @fn load
         * Loads the module
         */
        virtual void load() { };
        /**
         * @fn unload
         * Unloads the module
         */
        virtual void unload() { };
        /**
         * @fn getComponent
         * Produces a new Entity and attaches it to the given entity
         * @param entity The entity to attache the new component to
         * @return A pointer to the newly created component. This is a pointer to a BaseComponent, so, if needed, it should be casted to the corresponding subclass
         */
        virtual BaseComponent* getComponent(Entity* entity) = 0;
        virtual void removeComponent(std::string component) { };
        virtual bool preUpdate(const Ogre::FrameEvent& evt)
        {
            return true;
        };
        virtual bool update(const Ogre::FrameEvent& evt)
        {
            return true;
        };
        virtual bool postUpdate(const Ogre::FrameEvent& evt)
        {
            return true;
        };
        virtual void receiveEvent(Event* e) { }
    };
}

#endif
