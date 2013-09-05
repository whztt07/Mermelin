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
 * @file GroundPlate.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Entities/GroundPlate.h"
#include "Entities/Sphere.h"
#include "Modules/PhysicsModule.h"
#include "Components/PhysicsComponent.h"
#include "Engine.h"

using namespace CotopaxiEngine;

GroundPlate::GroundPlate(std::string name, Ogre::SceneNode* parentNode)
: Entity(name, "Floor", parentNode)
{
    this->getNode()->setPosition(0.0, -20.0, 0.0);
    this->getNode()->setVisible(false);

    this->addComponent(ENGINE->getGraphic()->getComponent(this));
    this->addComponent(ENGINE->getPhysics()->getComponent(this, 0.0,
            PhysicsModule::PLANE, PhysicsModule::COL_GHOST, PhysicsModule::COL_PLAYER, true));
}

GroundPlate::~GroundPlate() { }

void GroundPlate::receiveEvent(Event* e)
{
    Sphere* sphere = dynamic_cast<Sphere*> (e->getEntity());

    // checking if the collision occurs at the right place
    if (sphere && sphere->getNode()->getPosition().y < -10) {

        // making the sphere stand still
        sphere->getPhysics()->getBody()->setLinearVelocity(btVector3(0, 0, 0));
        sphere->getPhysics()->getBody()->setAngularVelocity(btVector3(0, 0, 0));

        // moving it back to the start position
        sphere->getPhysics()->getBody()->translate(BtOgre::Convert::toBullet(
                ENGINE->getStartPosition() - sphere->getNode()->getPosition()));

    }
}