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
 * @file Camera.cpp
 * @authors Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Entities/Camera.h"

using namespace CotopaxiEngine;

Camera::Camera(std::string name, Ogre::SceneNode* parentNode)
: Entity(name, "", parentNode)
{
    this->name = name;
    prepare();
}

bool Camera::isAttached() const
{
    return attached;
}

void Camera::attachTo(CotopaxiEngine::Entity* object)
{
    this->object = object;
    this->setParentNode(object->getNode());
    this->getNode()->setInheritOrientation(false);
    this->getNode()->lookAt(object->getNode()->getPosition(),
            Ogre::Node::TS_PARENT);

    attached = true;
}

void Camera::detach()
{
    this->setParentNode(ENGINE->getSceneManager()->getRootSceneNode());
    this->getNode()->setPosition(object->getNode()->getPosition() + offset);
    camera->lookAt(object->getNode()->getPosition());
    attached = false;
}

void Camera::reattach()
{
    if (object) {
        attachTo(object);
    } else {
        Ogre::LogManager::getSingletonPtr()->logMessage(
                Ogre::LogMessageLevel::LML_TRIVIAL,
                "Cannot reattach a Camera if it has not been attached first"
                );
    }
}

void Camera::setOffset(Ogre::Vector3 offset)
{
    this->offset = offset;
}

Camera::~Camera()
{    
    if (attached) {
        detach();
    }

    if (camera) {
        ENGINE->getSceneManager()->destroyCamera(camera);
        delete camera;
    }
    camera = NULL;
    object = NULL;
}

void Camera::update()
{
    Ogre::Vector3 position = Ogre::Vector3(
            this->getNode()->getPosition().x + offset.x, 0.0,
            this->getNode()->getPosition().z + offset.z);
    this->getNode()->setPosition(position);
    this->getNode()->setOrientation(1.0, 1.0, 0.0, 0.0);
    camera->lookAt(object->getNode()->getPosition());
}

void Camera::setParentNode(Ogre::SceneNode* newParent)
{
    Entity::setParentNode(newParent);
    getNode()->setInheritOrientation(false);
}

void Camera::translate(const Ogre::Vector3& translation)
{
    getNode()->translate(translation, Ogre::Node::TS_PARENT);
}

void Camera::rotate(const Ogre::Quaternion& rotation)
{
    getNode()->rotate(rotation, Ogre::Node::TS_LOCAL);
}

Ogre::Camera* Camera::getCamera()
{
    if(!camera) {
        prepare();
    }
    return camera;
}

void Camera::prepare()
{
    offset = Ogre::Vector3(0, 10, 0);

    camera = ENGINE->getSceneManager()->createCamera(name);
    camera->setNearClipDistance(0.1);
    camera->setFarClipDistance(10000);

    this->getNode()->attachObject(camera);
    ENGINE->getWindow()->addViewport(camera);
}