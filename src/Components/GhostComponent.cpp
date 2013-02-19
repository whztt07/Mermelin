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
 * @file GhostComponent.cpp
 * @author Patrick Joos
 */

#include "stdafx.h"
#include "Components/GhostComponent.h"

using namespace CotopaxiEngine;

GhostComponent::GhostComponent(btScalar mass, PhysicsModule::Shape s, short group, short mask)
: PhysicsComponent(mass, s, group, mask) { 
}

GhostComponent::~GhostComponent()
{
	ENGINE->getPhysics()->removeComponent(this->parent->getName());
	ENGINE->getPhysics()->getWorld()->removeCollisionObject(ghostObject);

    delete ghostObject;
}

void GhostComponent::setParent(Entity* entity)
{
    PhysicsComponent::setParent(entity);

    btCollisionShape* ghostShape;
    this->ghostObject = new btGhostObject();

    BtOgre::StaticMeshToShapeConverter* converter =
            new BtOgre::StaticMeshToShapeConverter(this->physicsEntity);

    if (this->physicsShape == PhysicsModule::PLANE) {
        ghostShape = converter->createPlane();
    } else {
        ghostShape = converter->createBox();
        ghostShape->setLocalScaling(btVector3(0.5, 0.5, 0.5));
    }

    Ogre::Vector3 position = this->parent->getNode()->_getDerivedPosition();

    btTransform trans(btTransform(btQuaternion(0, 0, 0, 1), BtOgre::Convert::toBullet(position)));

    this->ghostObject->setCollisionShape(ghostShape);
    this->ghostObject->setUserPointer(this);
    this->ghostObject->setWorldTransform(trans);

    this->ghostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE
            | this->ghostObject->getCollisionFlags());

    ENGINE->getPhysics()->getWorld()->addCollisionObject(ghostObject,
            PhysicsModule::CollisionMask::COL_GHOST, PhysicsModule::CollisionMask::COL_PLAYER);
}

btGhostObject* GhostComponent::getGhost()
{
    return this->ghostObject;
}

void GhostComponent::checkOverlappingObjects()
{
    int max = ghostObject->getNumOverlappingObjects();
    std::set<std::string> currentlyCollidingObjects;

    for (int i = 0; i < max; i++) {
        btCollisionObject* collidee = ghostObject->getOverlappingObject(i);
        PhysicsComponent* component = static_cast<PhysicsComponent*> (collidee->getUserPointer());

        std::string colliderName = component->getParent()->getName();
        currentlyCollidingObjects.insert(colliderName);

        std::set<std::string>::iterator colliderIterator = lastKnownColliders.begin();

        bool previouslyCollided = false;

        for (; colliderIterator != lastKnownColliders.end(); colliderIterator++) {
            if (colliderName == *colliderIterator) {
                previouslyCollided = true;
            }
        }

        Event* e;

        if (!previouslyCollided) {
            e = new Event(Event::COLLISION_ENTER);
        } else {
            e = new Event(Event::COLLISION);
        }

        e->entity = component->getParent();
        this->parent->receiveEvent(e);
        //delete e;
    }

    int setDeltas = lastKnownColliders.size() - currentlyCollidingObjects.size();

    if (setDeltas > 0) {

        // handling exit collisions
        std::vector<std::string>::iterator it;
        std::vector<std::string> delta(setDeltas);

        it = std::set_difference(lastKnownColliders.begin(),
                lastKnownColliders.end(),
                currentlyCollidingObjects.begin(),
                currentlyCollidingObjects.end(),
                delta.begin());

        for (; it != delta.end(); it++) {
            Entity* exitCollision = ENGINE->getEntity(*it);
            Event* exitEvent = new Event(Event::COLLISION_EXIT);
            exitEvent->entity = exitCollision;
            parent->receiveEvent(exitEvent);
            delete exitEvent;
        }
        //@todo delete events after sending them. Right now they all remain on the heap
    }

    lastKnownColliders = currentlyCollidingObjects;
}

void GhostComponent::receiveEvent(Event* e)
{
	if(e->getType() == Event::TRANSLATE)
	{
		btTransform transform;

        transform.setIdentity();
        Ogre::Vector3 position = parent->getNode()->_getDerivedPosition();
        Ogre::Quaternion orientation = parent->getNode()->_getDerivedOrientation();

		transform.setOrigin(BtOgre::Convert::toBullet(position));
		transform.setRotation(BtOgre::Convert::toBullet(orientation));

		ghostObject->setWorldTransform(transform);
	}
}