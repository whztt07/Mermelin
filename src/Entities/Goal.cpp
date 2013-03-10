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
 * @file Goal.cpp
 * @authors Eduardo Hahn Paredes
 */
#include "stdafx.h"
#include "Entities/Goal.h"
#include "Entities/Sphere.h"

using namespace CotopaxiEngine;

Goal::Goal(std::string name, Ogre::SceneNode* parentNode)
: Entity(name, "Floor", parentNode)
{
    this->addComponent(ENGINE->getGraphic()->getComponent(this));
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingletonPtr()
            ->create("Goal", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    Ogre::TextureUnitState* texture = material->getTechnique(0)->getPass(0)
            ->createTextureUnitState("goal_color.gif");

    this->getOgreEntity()->setMaterial(material);
}

void Goal::receiveEvent(Event* e)
{
    if (e->getType() == Event::COLLISION_ENTER) {
        Sphere* sphere = dynamic_cast<Sphere*> (e->entity);
        if (sphere != NULL) {
            ENGINE->throwEvent(Event::LEVEL_END);
            delete e;
            e = NULL;
        }
    } else if (e->getType() == Event::TRANSLATE) {
        ENGINE->getPhysics()->getComponent(this)->receiveEvent(e);
    } else {
        Entity::receiveEvent(e);
    }
}

