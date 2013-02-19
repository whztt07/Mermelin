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
 * @file GhostComponent.h
 * @brief Contains the GhostComponent
 * @author Eduardo Hahn Paredes
 * @todo write doxygen
 */

#ifndef GHOSTCOMPONENT_H
#define	GHOSTCOMPONETN_H

#include "stdafx.h"
#include <string>
#include "Modules/PhysicsModule.h"
#include "Components/PhysicsComponent.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

namespace CotopaxiEngine {

    /**
     * @class GhostComponent
     * @brief Access to Bullets btGhostObject
     */
    class GhostComponent : public PhysicsComponent
    {
    public:
        GhostComponent(btScalar mass, PhysicsModule::Shape s, short group, short mask);
        ~GhostComponent();

        virtual void setParent(Entity* parent);
        btGhostObject* getGhost();
        void checkOverlappingObjects();
        virtual void receiveEvent(Event* e);
    private:
        btGhostObject* ghostObject;
        std::set<std::string> lastKnownColliders;
    };
}

#endif