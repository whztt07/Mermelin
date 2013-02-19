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
 * @file Rotor.h
 * @brief Represents a fan that is able to elevate the Sphere
 * @author Eduardo Hahn Paredes
 * 
 * It can be switched on or off.
 */

#include "Engine.h"
#include "Entity.h"
#include "Modules/GraphicModule.h"
#include "Shaders/RotationShader.h"
#include "Entities/Sphere.h"
#include "Components/PhysicsComponent.h"
#include "Components/GraphicComponent.h"
#include "Resources/Managers/AudioManager.h"
#include "Components/AudioComponent.h"

#include "Logic/Trigger.h"

#ifndef ROTOR_H
#define	ROTOR_H

namespace CotopaxiEngine {
    // forward declaration
    class LogicComponent;

    /**
     * @class Rotor
     * @brief Creates a Rotor Entity which can elevate the Sphere.
     * 
     * It consists of 3 meshes:
     * Rotor (the the turning part), RotorHole (the place where it's placed)
     * and Wind (a mesh that gets visible when the fan is active)
     */
    class Rotor : public Entity
    {
    public:
        Rotor(std::string name, Ogre::SceneNode* parentNode, bool active = false);
        virtual ~Rotor();

        /**
         * @fn turnOn
         * Turns on the rotor. Loads a shader and makes the Wind.mesh 
         * visible.
         */
        void turnOn();

        /**
         * @fn turnOff
         * Turns off the rotor. Unloads a shader and makes the Wind.mesh 
         * invisible.
         */
        void turnOff();
        /**
         * @fn isActive
         * Indicates if the Rotor is active (turning) or not.
         * @return true if it's active.
         */
        bool isActive() const
        {
            return active;
        }

        void receiveEvent(Event* e);
        void addComponent(BaseComponent* component);
        static Entity* create(std::string name, Ogre::SceneNode* parentNode)
        {
            return new Rotor(name, parentNode);
        };
    private:
        bool active;
        LogicComponent* logicComponent;
        Entity* floor;
        Entity* rotor;
        Entity* wind;
        AudioComponent* audioComponent;
    };
}
#endif

