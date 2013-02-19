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
 * @file LogicComponent.h
 * @brief Adds some logic to an Entity.
 * @author Eduardo Hahn Paredes
 */

#ifndef LOGICCOMPONENT_H
#define	LOGICCOMPONENT_H

#include "BaseComponent.h"
#include "Modules/LogicModule.h"

namespace CotopaxiEngine {

    /**
     * @class LogicComponent
     * @brief Used if the Entity must have a role in a logical context.
     * 
     * It's the interface and the lowest common denominator for 
     * implementing the logic using the composite pattern.
     * @author Eduardo Hahn Paredes
     */
    class LogicComponent : public BaseComponent
    {
    public:

        /**
         * Checks Conditions and Triggers.
         * @return true if the condition is fulfilled or the trigger fired.
         */
        virtual bool check() = 0; // pure virutal (abstract) function
    };
}

#endif
