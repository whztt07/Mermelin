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
 * PhysicsComponent.cpp
 *
 * @author Patrick Joos, Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Components/PhysicsComponent.h"
#include "Components/GraphicComponent.h"

using namespace CotopaxiEngine;

PhysicsComponent::PhysicsComponent(btScalar mass, PhysicsModule::Shape s, short group, short mask)
{
    this->mass = mass;
    this->physicsShape = s;
    this->group = group;
    this->mask = mask;
}

PhysicsComponent::~PhysicsComponent()
{
    ENGINE->getPhysics()->getWorld()->removeRigidBody(body);
    ENGINE->getPhysics()->removeComponent(this->parent->getName());
    delete body;
    delete shape;
}

void PhysicsComponent::setParent(Entity* parent)
{
    BaseComponent::setParent(parent);
    this->parent->registerListener(Event::PHYSICS_UPDATE, this);
    createBody();
}

void PhysicsComponent::createBody()
{
    int numObjects = this->parent->getNode()->numAttachedObjects();
    PhysicsModule::Shape s = physicsShape;

    if (numObjects > 0) {
        physicsEntity = dynamic_cast<Ogre::Entity*> (parent->getNode()->getAttachedObject(0));
        BtOgre::StaticMeshToShapeConverter* converter =
                new BtOgre::StaticMeshToShapeConverter(physicsEntity);

        switch (s) {
            case PhysicsModule::BOX:
            {
                shape = converter->createBox();
                break;
            }
            case PhysicsModule::CYLINDER:
            {
                shape = converter->createCylinder();
                break;
            }
            case PhysicsModule::TRIMESH:
            {
                shape = converter->createTrimesh();
                break;
            }
            case PhysicsModule::PLANE:
            {
                shape = converter->createPlane();
                break;
            }
            case PhysicsModule::SPHERE:
            default:
            {
                shape = converter->createSphere();
                break;
            }
        }
    } else {
        switch (s) {
            case PhysicsModule::BOX:
            {
                shape = new btBoxShape(btVector3(0, 0, 0));
                break;
            }
            case PhysicsModule::CYLINDER:
            {
                shape = new btCylinderShape(btVector3(0, 0, 0));
                break;
            }
            case PhysicsModule::PLANE:
            {
                shape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);                
                break;
            }
            case PhysicsModule::TRIMESH:
            case PhysicsModule::SPHERE:
            default:
            {
                shape = new btSphereShape(0);
                break;
            }
        }
    }

    // calculating inertia
    if (mass != 0) {
        inertia = btVector3(0, 0, 0);
        shape->calculateLocalInertia(mass, inertia);
    } else {
        inertia = btVector3(0, 0, 0);
    }

    // creating BtOgre MotionState (connects Ogre and Bullet)
    state = new BtOgre::RigidBodyState(parent->getNode());
    btRigidBody::btRigidBodyConstructionInfo bodyProps(mass, state, shape, inertia);
    bodyProps.m_friction = 20;
    body = new btRigidBody(bodyProps);

    // creating the Body    
    if (mass == 0) {
        body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        body->setActivationState(DISABLE_DEACTIVATION);
    }

    body->setUserPointer(this);
    ENGINE->getPhysics()->getWorld()->addRigidBody(body, group, mask);
}

btRigidBody* PhysicsComponent::getBody() const
{
    return body;
}

void PhysicsComponent::applyPhysics(Ogre::Vector3 v)
{
    float force = 0.01;
    btVector3 impVec(v.x * force, v.z * force, v.y * force);
    body->applyCentralImpulse(impVec);
}

void PhysicsComponent::receiveEvent(Event* event)
{
    if (event->getType() == Event::PHYSICS_UPDATE) {
        btTransform transform;

        transform.setIdentity();
        Ogre::Vector3 position = parent->getNode()->_getDerivedPosition();
        Ogre::Quaternion orientation = parent->getNode()->_getDerivedOrientation();

        if (this->getParent()->getGraphicComponent()->isAnimated() && false) {
            Ogre::Bone* bone = parent->getOgreEntity()->getSkeleton()->getBone(0);
            Ogre::Vector3 bone_converted_pos = parent->getNode()
                    ->convertLocalToWorldPosition(bone->_getDerivedPosition());
            Ogre::Quaternion bone_converted_or = parent->getNode()
                    ->convertLocalToWorldOrientation(bone->_getDerivedOrientation());

            Ogre::Vector3 bonePos =
                    parent->getNode()->_getDerivedPosition() +
                    parent->getNode()->_getDerivedOrientation() *
                    bone_converted_pos;

            Ogre::Quaternion boneRot = parent->getNode()->_getDerivedOrientation() * (bone
                    ->getInitialOrientation().Inverse() * bone->_getDerivedOrientation());

            position = bone_converted_pos;
            orientation = boneRot;

        }

        transform.setOrigin(BtOgre::Convert::toBullet(position));
        transform.setRotation(BtOgre::Convert::toBullet(orientation));

        state->setKinematicPos(transform);
    }
}

void PhysicsComponent::handleContact(PhysicsComponent* component)
{
    if (component == this) {
        Ogre::LogManager::getSingletonPtr()->logMessage(Ogre::LogMessageLevel::LML_CRITICAL, 
                "Comparing identical PhysicsComponent: " + this->parent->getName());
    }

    Event e(Event::EventType::COLLISION_ENTER);
    e.entity = component->getParent();

    parent->receiveEvent(&e);
}

void PhysicsComponent::setComponentGravity(const Ogre::Vector3& gravity)
{
    this->body->setGravity(BtOgre::Convert::toBullet(gravity));
}

void PhysicsComponent::setActive(bool active)
{
    if (active) {
        if (body == NULL) {
            createBody();
        }
    } else {
        ENGINE->getPhysics()->getWorld()->removeRigidBody(body);
        body = NULL;
    }
}