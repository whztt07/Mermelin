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
 * @file PlayState.cpp
 * @author Patrick Joos, Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "GameStates/PlayState.h"
#include "Engine.h"
#include "Resources/Managers/LevelManager.h"
#include "Entities/Sphere.h"

using namespace CotopaxiEngine;

PlayState::PlayState()
: level(1)
{
    changeLevel = false;
    ENGINE->registerForEventType(this, Event::LEVEL_END);
}

void PlayState::load()
{
    ENGINE->setState(ENGINE->STATE_STOPPED);

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    LevelManager::getSingleton().load("level_" + std::to_string(((long double) level)) + ".txt",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
#else
    LevelManager::getSingleton().load("level_" + std::to_string(level) + ".txt",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
#endif    
    Entity* sphere = ENGINE->produceEntity("sphere", "playerSphere");
    sphere->getNode()->setPosition(ENGINE->getStartPosition());
    sphere->receiveEvent(new Event(Event::TRANSLATE));
    
    Entity* ground = ENGINE->produceEntity("groundplate", "groundPlate");
    ground->receiveEvent(new Event(Event::TRANSLATE));
    
    Ogre::Light* light = ENGINE->getSceneManager()->createLight("MainLight");
    light->setPosition(50, 50, 50);
    light->setPowerScale(20);
    light->setType(Ogre::Light::LT_POINT);
    light->setCastShadows(true);
    light->setDiffuseColour(1.0, 1.0, 1.0);
    
    ENGINE->getCamera()->attachTo(sphere);

    loaded = true;
}

void PlayState::loadNext()
{
    LevelManager::getSingleton().unload("level_" + std::to_string(((long double) level)) + ".txt");
    ENGINE->removeAllEntities();
    ENGINE->getSceneManager()->clearScene();    
    level++;
    load();
}

void PlayState::receiveEvent(Event* e)
{
    if (e->getType() == Event::LEVEL_END) {
        changeLevel = true;
    }
}

bool PlayState::frameStarted(const Ogre::FrameEvent& evt)
{
    if (changeLevel) {
        loadNext();
        changeLevel = false;
    }
    return true;
}
