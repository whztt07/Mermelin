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
 * @file WoodWall.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Entities/WoodWall.h"
#include "Entities/Sphere.h"
#include "Modules/LogicModule.h"

using namespace CotopaxiEngine;

WoodWall::WoodWall(std::string name, Ogre::SceneNode* parentNode)
: Entity(name, "Wall_Wood", parentNode)
{
    this->state = NORMAL;
    trigger = ENGINE->getLogic()->createTrigger(this, name);
    this->addComponent(trigger);
    this->registerListener(Event::COLLISION_ENTER, trigger);
    
    audioComponent = dynamic_cast<AudioComponent*> (ENGINE->getAudio()->getComponent(this));
    this->addComponent(audioComponent);
    audioComponent->addSound("burning");
    
}

WoodWall::~WoodWall()
{
    delete audioComponent;
    delete trigger;
}

void WoodWall::receiveEvent(Event* event)
{
    if (event->getType() == Event::COLLISION_ENTER && state == NORMAL) {
        state = BURNING;
        audioComponent->play("burning", false);
        trigger->receiveEvent(event);
        
        Sphere* playerSphere = dynamic_cast<Sphere*> (event->entity);

        if (playerSphere != NULL) {
            if (playerSphere->getState() == Element::FIRE) {
                
                physics = dynamic_cast<PhysicsComponent*>(ENGINE->getPhysics()->getComponent(this));
                physics->setActive(false);
                
                this->getNode()->getParent()->removeChild(this->getNode());                
            }
        }
    }
}