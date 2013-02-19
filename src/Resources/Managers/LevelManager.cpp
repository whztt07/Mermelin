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
 * @file LevelManager.cpp
 * @author Patrick Joos
 */

#include "stdafx.h"
#include "Resources/Managers/LevelManager.h"
using namespace CotopaxiEngine;

template<> LevelManager* Ogre::Singleton<LevelManager>::msSingleton = 0;

LevelManager* LevelManager::getSingletonPtr()
{
    return msSingleton;
}

LevelManager& LevelManager::getSingleton()
{
    assert(msSingleton);
    return (*msSingleton);
}

LevelManager::LevelManager()
{
    mResourceType = "Level";
    mLoadOrder = 30.0f;

    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);

}

LevelManager::~LevelManager()
{
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

LevelPtr LevelManager::load(const Ogre::String& name, const Ogre::String& group)
{
    LevelPtr level = getByName(name);

    if (level.isNull())
        level = create(name, group);

    level->load();
    return level;
}

Ogre::Resource* LevelManager::createImpl(const Ogre::String& name,
        Ogre::ResourceHandle handle,
        const Ogre::String& group,
        bool isManual, Ogre::ManualResourceLoader* loader,
        const Ogre::NameValuePairList* createParams)
{
    return new Level(this, name, handle, group, isManual, loader);
}