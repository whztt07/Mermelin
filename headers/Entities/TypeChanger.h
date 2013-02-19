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
 * @file TypeChanger.h
 * @brief Changes the type of the marble
 * @author Eduardo Hahn Paredes
 */
#ifndef TYPECHANGER_H
#define	TYPECHANGER_H

#include "Engine.h"
#include "Entities/Entity.h"
#include "Entities/Sphere.h"
#include "Shaders/Shader.h"
#include "Shaders/AirShader.h"
#include "Shaders/EarthShader.h"
#include "Shaders/FireShader.h"
#include "Shaders/WaterShader.h"

namespace CotopaxiEngine
{

    /**
     * @class TypeChanger
     * @brief Defines a trigger that changes the state of a Sphere
     *
     * A core part of our game is the ability to change the state of the 
     * players Sphere, to achieve certain effects. This class implements a 
     * simple trigger that will, on contact with a sphere, change the state of
     * the former.
     */
    class TypeChanger : public Entity
    {
    public:
        TypeChanger(std::string name, Ogre::SceneNode* parentNode);
        TypeChanger(const TypeChanger& orig);
        virtual ~TypeChanger();

        /**
         * setElement
         * Sets an Element for the TypeChanger.
         * @param element the Element to be set.
         */
        void setElement(CotopaxiEngine::Element element);
        virtual void receiveEvent(Event* e);

        static Entity* create(std::string name, Ogre::SceneNode* parentNode) {
            return new TypeChanger(name, parentNode);
        };
    private:
        Element element;
        Entity* symbolBar;
        Entity* symbolTriangle;
    };
}

#endif