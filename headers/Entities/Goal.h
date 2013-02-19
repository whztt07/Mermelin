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
 * @file Goal.h
 * @brief Contains the Goal class.
 * @author Eduardo Hahn Paredes
 */
#ifndef GOAL_H
#define	GOAL_H

#include "Entity.h"

namespace CotopaxiEngine
{
    /**
     * @class Goal
     * @brief Represents the goal of every game-level.
     */
    class Goal : public Entity
    {
    public:
        Goal(std::string name, Ogre::SceneNode* parentNode);
        virtual void receiveEvent(Event* e);
        
        static Entity* create(std::string name, Ogre::SceneNode* parentNode) {
            return new Goal(name, parentNode);
        };
    private:

    };
}

#endif

