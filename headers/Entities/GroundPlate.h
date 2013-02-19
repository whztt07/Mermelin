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
 * @file GroundPlate.h
 * @brief The floor that is located below the game.
 * 
 * @author Eduardo Hahn Paredes
 */
#ifndef GROUNDPLATE_H
#define GROUNDPLATE_H

#include "stdafx.h"
#include "Entity.h"

namespace CotopaxiEngine
{
    /**
     * @class GroundPlate
     * @brief An infinite plane that is located under the level.
     *
     * If the Sphere collides with it it will be reset to the starting position.
     */
	class GroundPlate: public Entity
	{
	public:
		GroundPlate(std::string name, Ogre::SceneNode* parentNode);
        virtual ~GroundPlate();
		virtual void receiveEvent(Event* e);

		static Entity* create(std::string name, Ogre::SceneNode* parentNode) {
            return new GroundPlate(name, parentNode);
        }
	};
}

#endif