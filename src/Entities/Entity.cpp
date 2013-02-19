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
 * Entity.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Entities/Entity.h"
#include "Components/BaseComponent.h"
#include "Modules/GraphicModule.h"
#include "Engine.h"

using namespace std;
using namespace CotopaxiEngine;

Entity::Entity() { }

Entity::Entity(std::string name, std::string meshName, Ogre::SceneNode* parentNode)
{
    this->name = name;
    this->node = parentNode->createChildSceneNode(name);

    if (meshName != "") {
        ogreEntity = ENGINE->getSceneManager()->createEntity(meshName + ".mesh");
        this->node->attachObject(ogreEntity);
    } else {
        ogreEntity = NULL;
    }
}

Entity::~Entity()
{
    ComponentVector::iterator i;
    for (i = components.begin(); i != components.end(); i++) {
        delete *i;
    }
}

void Entity::addComponent(BaseComponent *component)
{
    components.push_back(component);
}

void Entity::registerListener(const Event::EventType& type, EventListener* listener)
{
    eventListeners[type].push_back(listener);
    ENGINE->registerForEventType(this, type);
}

void Entity::receiveEvent(Event* e)
{
    EventListenerVector::iterator i;
    for (i = eventListeners[e->getType()].begin(); i != eventListeners[e->getType()].end(); i++) {
        (*i)->receiveEvent(e);
    }

}

void Entity::setParentNode(Ogre::SceneNode* newParent)
{
    this->node->getParentSceneNode()->removeChild(this->node);
    newParent->addChild(node);
}

Ogre::SceneNode* Entity::getNode() const
{
    return node;
}

void Entity::setOgreEntity(Ogre::Entity* ogreEntity)
{
    this->ogreEntity = ogreEntity;
}

Ogre::Entity* Entity::getOgreEntity() const
{
    return ogreEntity;
}

void Entity::setGraphicComponent(GraphicComponent* graphicComponent)
{
    this->graphicComponent = graphicComponent;
}

GraphicComponent* Entity::getGraphicComponent() const
{
    return graphicComponent;
}

void Entity::setName(std::string name)
{
    this->name = name;
}

std::string Entity::getName() const
{
    return name;
}

void Entity::removeComponent(BaseComponent* component)
{
    ComponentVector::iterator index = std::find(components.begin(), components.end(), component);
    this->components.erase(index);
}