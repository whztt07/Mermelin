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
 * @file Rotor.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Entities/Rotor.h"

using namespace CotopaxiEngine;

Rotor::Rotor(std::string name, Ogre::SceneNode* parentNode, bool active)
: Entity(name, "RotorHole", parentNode)
{
    active = active;

    this->addComponent(ENGINE->getGraphic()->getComponent(this));

    floor = ENGINE->createEntity("fakeFloor_" + name, "Floor", this->getNode());
    floor->addComponent(ENGINE->getGraphic()->getComponent(floor));
    floor->addComponent(ENGINE->getPhysics()->getComponent(floor, 0, 
            PhysicsModule::TRIMESH, PhysicsModule::COL_STATIC, 
            PhysicsModule::COL_PLAYER, false));
    floor->getNode()->setVisible(false);

    rotor = ENGINE->createEntity("rotor_" + name, "Rotor", this->getNode());
    rotor->addComponent(ENGINE->getGraphic()->getComponent(rotor));

    wind = ENGINE->createEntity("wind_" + name, "Wind", this->getNode());
    wind->addComponent(ENGINE->getGraphic()->getComponent(wind));
    wind->getNode()->setVisible(false);

    active = false;
    wind->getGraphicComponent()->setShader(new RotationShader(wind, -9, 0, "windRot"));
    wind->getGraphicComponent()->getShader()->setTexture("vortex_color.png", "texture");
    wind->getGraphicComponent()->getShader()->getFragment()->setNamedConstant("texture", 0);
    wind->getGraphicComponent()->getShader()->getPass()->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    wind->getGraphicComponent()->getShader()->getPass()->setDepthWriteEnabled(false);
    wind->getNode()->setVisible(false);

    rotor->getGraphicComponent()->setShader(new RotationShader(rotor, 2, 0, "rotorRot"));
    rotor->getGraphicComponent()->getShader()->setTexture("rusty_color.png");
    rotor->getGraphicComponent()->getShader()->getFragment()->setNamedConstant("texture", 0);
    rotor->getGraphicComponent()->getShader()->getPass()->setSceneBlending(Ogre::SBT_REPLACE);
    rotor->getGraphicComponent()->getShader()->getPass()->setDepthWriteEnabled(true);
    rotor->getNode()->setVisible(true);

	audioComponent = dynamic_cast<AudioComponent*>(ENGINE->getAudio()->getComponent(this));
	this->addComponent(audioComponent);
	audioComponent->addSound("rotor");
    
	active ? turnOn() : turnOff();
}

Rotor::~Rotor()
{
    delete wind;
    delete rotor;
    delete floor;
    wind = NULL;
    rotor = NULL;
    floor = NULL;
}

void Rotor::turnOn()
{
    active = true;
    wind->getNode()->setVisible(true);
    rotor->getGraphicComponent()->getShader()->getVertex()->setNamedConstant("speed", 2);
    
	audioComponent->play("rotor", true);
}

void Rotor::turnOff()
{
    active = false;
    wind->getNode()->setVisible(false);
    rotor->getGraphicComponent()->getShader()->getVertex()->setNamedConstant("speed", 0);
   
	audioComponent->stop("rotor");
}

void Rotor::receiveEvent(Event* event)
{
    if (event->getType() == Event::CONDITION_FULFILLED) {
        if (active) {
            turnOff();
        } else {
            turnOn();
        }
    }

    if (event->getType() == Event::COLLISION_ENTER && active) {
        Entity* player = dynamic_cast<Entity*> (event->entity);

        if (player != NULL) {
            PhysicsComponent* entityPhys = dynamic_cast<PhysicsComponent*> (ENGINE->getPhysics()->getComponent(player));
            entityPhys->getBody()->applyImpulse(btVector3(0, 80, 0), btVector3(0, 1, 0));
        }
    }

    if (event->getType() == Event::TRANSLATE) {
        Event* e = new Event(Event::PHYSICS_UPDATE);
        floor->receiveEvent(e);
        floor->receiveEvent(e);        
    }

	Entity::receiveEvent(event);
}

void Rotor::addComponent(BaseComponent* component)
{
    Entity::addComponent(component);
    Trigger* trigger = dynamic_cast<Trigger*> (component);

    if (trigger != NULL) {
        this->registerListener(Event::COLLISION_ENTER, component);
        this->registerListener(Event::COLLISION_EXIT, component);
        this->registerListener(Event::CONDITION_FULFILLED, component);
    }
}