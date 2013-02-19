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
    this->getNode()->setVisible(true);

	this->addComponent(ENGINE->getGraphic()->getComponent(this));
    
    this->addComponent(ENGINE->getPhysics()->getComponent(this, 0.0, 
            PhysicsModule::PLANE, PhysicsModule::COL_GHOST, PhysicsModule::COL_PLAYER, true));
}

GroundPlate::~GroundPlate() { }

void GroundPlate::receiveEvent(Event* e)
{    
    if (e->getType() == Event::COLLISION_ENTER) {
        Sphere* sphere = dynamic_cast<Sphere*> (e->entity);
        if (sphere != NULL) {
            sphere->getNode()->setPosition(ENGINE->getStartPosition());
            Event* e = new Event(Event::TRANSLATE);
            sphere->receiveEvent(e);
            delete e;
        }
    } else {
        Entity::receiveEvent(e);
    }

	if(e->getType() == Event::TRANSLATE)
	{
		int i = 1;
		ENGINE->getPhysics()->getComponent(this)->receiveEvent(e);
	}
}