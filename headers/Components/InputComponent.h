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
 * @file InputComponent.h
 * @brief Contains the InputComponent.
 * @author Eduardo Hahn Paredes
 */

#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "BaseComponent.h"
#include "Modules/InputModule.h"

namespace CotopaxiEngine {

    /**
     * @class InputComponent
     * @brief Defines a basic component for interacting with the input system.
     */
    class InputComponent : public BaseComponent
    {
    public:
        
        /**
         * @fn transfer
         * Passes the Event to the Entity.
         * @param event
         */
        void transfer(Event* event);
        void setParent(Entity* parent);
    };
}

#endif