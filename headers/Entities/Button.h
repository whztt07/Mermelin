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
 * @file Button.h
 * @brief Button definition resides in this file.
 *
 * @author Eduardo Hahn Paredes
 */

#ifndef BUTTON_H
#define BUTTON_H

#include "stdafx.h"
#include "Entities/Entity.h"
#include "Components/BaseComponent.h"
#include "Components/AudioComponent.h"

namespace CotopaxiEngine
{

    /**
     * @class Button
     * 
     * @brief Represents a button that can be activated by the Sphere.
     */
    class Button : public Entity
    {
    public:
        Button(std::string name, Ogre::SceneNode* parentNode);
        virtual ~Button();
        virtual void receiveEvent(Event* e);

        static Entity* create(std::string name, Ogre::SceneNode* parentNode) {
            return new Button(name, parentNode);
        }
    private:
        AudioComponent* audioComponent;
        BaseComponent* trigger;
    };
}
#endif

