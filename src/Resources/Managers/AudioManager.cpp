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
 * @file AudioManager.cpp
 * @author Patrick Joos
 */

#include "stdafx.h"
#include "Resources/Managers/AudioManager.h"
using namespace CotopaxiEngine;

template<> AudioManager *Ogre::Singleton<AudioManager>::msSingleton = 0;

AudioManager *AudioManager::getSingletonPtr()
{
    return msSingleton;
}

AudioManager &AudioManager::getSingleton()
{
    assert(msSingleton);
    return (*msSingleton);
}

AudioManager::AudioManager()
{
    mResourceType = "Audio";

    // low, because it will likely reference other resources
    mLoadOrder = 30.0f;

    // this is how we register the ResourceManager with OGRE
    Ogre::ResourceGroupManager::getSingleton()._registerResourceManager(mResourceType, this);
}

AudioManager::~AudioManager()
{
    // and this is how we unregister it
    Ogre::ResourceGroupManager::getSingleton()._unregisterResourceManager(mResourceType);
}

AudioPtr AudioManager::load(const Ogre::String &name, const Ogre::String &group, Audio::MODE mode)
{
    AudioPtr audioPtr = getByName(name);

    if (audioPtr.isNull()) {
        audioPtr = create(name, group, mode);
    }

    audioPtr->load();
    return audioPtr;
}

Ogre::Resource *AudioManager::createImpl(const Ogre::String &name, Ogre::ResourceHandle handle,
        const Ogre::String &group, bool isManual, Ogre::ManualResourceLoader *loader,
        const Ogre::NameValuePairList *createParams)
{
    return new Audio(this, name, handle, group, Audio::MODE::SOUND, isManual, loader);
}