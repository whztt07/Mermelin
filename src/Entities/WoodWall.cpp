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
#include "Components/PhysicsComponent.h"
#include "Components/AudioComponent.h"

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
    audioComponent->addSound("wood");
}

WoodWall::~WoodWall()
{
    delete audioComponent;
    delete trigger;
    audioComponent = NULL;
    trigger = NULL;
}

void WoodWall::receiveEvent(Event* event)
{
    if (event->getType() == Event::COLLISION_ENTER && state == NORMAL) {
        trigger->receiveEvent(event);
        audioComponent->play("wood", false);

        Sphere* playerSphere = dynamic_cast<Sphere*> (event->getEntity());

        if (playerSphere) {
            if (playerSphere->getState() == Element::FIRE) {
                state = BURNING;
                audioComponent->play("burning", false);
                Destruction* destroy = new Destruction(this);
                destroy->Launch();
            }
        }
    }
}

WoodWall::Destruction::Destruction(WoodWall* outer)
: wall(outer)
{}

void WoodWall::Destruction::Run()
{
    // Creating smoke
    Ogre::ParticleSystem* smoke = ENGINE->getSceneManager()->createParticleSystem("Smoke");
    wall->getNode()->attachObject(smoke);
    
    // Showing the burning wall
    FireShader* fire = new FireShader(wall);
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()->getByName("Black");
    wall->getGraphicComponent()->setShader(fire);
    sf::Sleep(8);

    // Displaying it as coal (black)
    wall->getOgreEntity()->setMaterial(material);
    sf::Sleep(2);
    
    physics = dynamic_cast<PhysicsComponent*> (ENGINE->getPhysics()->getComponent(wall));
    physics->setActive(false);
    wall->getNode()->getParent()->removeChild(wall->getNode());
    delete fire;
}