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
 * @file Shpere.h
 * @brief Represents the marble. The most important object in the game.
 * @author Eduardo Hahn Paredes
 * 
 * It can be in different states, including the representation of the 4 elements.
 */
#ifndef SPHERE_H
#define	SPHERE_H

#include "Entity.h"
#include "Modules/GraphicModule.h"
#include "Modules/PhysicsModule.h"
#include "Components/PhysicsComponent.h"

#include "Shaders/Shader.h"
#include "Shaders/AirShader.h"
#include "Shaders/EarthShader.h"
#include "Shaders/FireShader.h"
#include "Shaders/WaterShader.h"

namespace CotopaxiEngine {

    /**
     * @class Sphere
     * @brief It's the most important Entity in the game.
     *
     * Its states represents the four elements of the game.
     * @see Element
     */
    class Sphere : public Entity
    {
    public:
        Sphere(std::string name, Ogre::SceneNode* parentNode);
        /**
         * @fn setWeight
         * Sets the weight (used by the PhysicsModule)
         * @param weight a positive integer
         */
        void setWeight(int weight)
        {
            this->weight = weight;
        }
        /**
         * @fn getWeight
         * Shows the weight
         * @return The used in physics
         */
        int getWeight() const
        {
            return weight;
        }
        /**
         * @fn getState
         * Indicates which Element is loaded on the sphere
         * @return the element
         */
        Element getState() const
        {
            return state;
        }
        
        /**
         * @fn getDirection
         * Indicates the direction in which the sphere is moving
         * @return the direction
         */
        Ogre::Vector3 getDirection();

        /**
         * @fn setElement
         * The Sphere can be put in a predefined state using a given Element
         * @param element An Element like AIR or FIRE
         */
        void setElement(const Element& element);
        virtual void receiveEvent(Event* e);
        static Entity* create(std::string name, Ogre::SceneNode* parentNode)
        {
            return new Sphere(name, parentNode);
        };
    private:
        PhysicsComponent* physics;
        Ogre::Light* light;
        Ogre::Vector3 lastPosition;
        Element state;
        int weight;
    };
}

#endif