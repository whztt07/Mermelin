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
 * @file WoodWall.h
 * @brief Represents a wooden wall that can be burned down.
 * @author Eduardo Hahn Paredes
 * 
 * It can be in 3 different states: normal, burning and gone
 */

#ifndef WOODWALL_H
#define	WOODWALL_H

#include "stdafx.h"
#include "Entities/Entity.h"
#include "Components/BaseComponent.h"
#include "Components/AudioComponent.h"
#include "Components/PhysicsComponent.h"

namespace CotopaxiEngine
{

    /**
     * @class WoodWall
     * @brief Represents a wooden wall that can be burned down.
     *
     * It's realized by a single mesh calles Wall_Wood.mesh. If the wall is
     * in NORMAL state the Shpere will colide against it, but when it is in
     * GONE state the Sphere can pass through.
     */
    class WoodWall : public Entity
    {
    public:

        /** @enum WoodWallState	Defines all types of states a door can have */
        enum WoodWallState
        {
            NORMAL,
            BURNING,
            GONE
        };

        WoodWall(std::string name, Ogre::SceneNode* parentNode);
        
        virtual ~WoodWall();

        /**
         * @fn setState
         * Puts the TypeChanger in the given state
         * @param state the WoodWallState to be set
         */
        void setState(WoodWallState state) {
            this->state = state;
        }

        /**
         * @fn getState
         * Indicates the current state of the wall.
         * @return one of the 3 possible states
         */
        WoodWallState getState() const {
            return state;
        }

        static Entity* create(std::string name, Ogre::SceneNode* parentNode) {
            return new WoodWall(name, parentNode);
        }

        virtual void receiveEvent(Event* event);
    private:
        PhysicsComponent* physics;
        WoodWallState state;
        AudioComponent* audioComponent;
        BaseComponent* trigger;
    };
}

#endif