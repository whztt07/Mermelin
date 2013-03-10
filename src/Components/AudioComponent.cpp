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
 * AudioComponent.cpp
 * @author Patrick Joos, Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Components/AudioComponent.h"

using namespace CotopaxiEngine;

AudioComponent::AudioComponent() { }

AudioComponent::~AudioComponent()
{
    std::map<std::string, Audio*>::iterator i;
    for (i = audioList.begin(); i != audioList.end(); i++) {
        delete i->second;
        i->second = NULL;
    }
    audioList.clear();
}

bool AudioComponent::addSound(std::string soundName)
{
    Audio* audio = AudioManager::getSingleton().load(soundName + ".ogg",
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).getPointer();
    if (audio->isLoaded()) {
        Ogre::Vector3 position = this->getParent()->getNode()->_getDerivedPosition();

        audio->getAudio()->SetPosition(position.x, position.y, position.z);
        audioList[soundName] = audio;
        return true;
    }

    return false;
}

void AudioComponent::removeSound(std::string soundName) {
    //@todo IMPLEMENT
}

bool AudioComponent::play(std::string name, bool loop)
{
    Audio* audioToPlay = audioList[name];
    if (audioToPlay != NULL) {
        audioToPlay->getAudio()->SetLoop(loop);
        audioToPlay->getAudio()->Play();
        return true;
    }

    return false;
}

void AudioComponent::stop(std::string soundName)
{
    Audio* audioToPlay = audioList[soundName];
    if (audioToPlay != NULL) {
        audioToPlay->getAudio()->Pause();
    }
}

void AudioComponent::receiveEvent(Event* e)
{
    if (e->getType() == Event::TRANSLATE) {       
        Ogre::Vector3 position = getParent()->getNode()->_getDerivedPosition();

        std::map<std::string, Audio*>::iterator i;
        for (i = audioList.begin(); i != audioList.end(); i++) {
            i->second->getAudio()->SetPosition(position.x, position.y, position.z);
        }
    }

    BaseComponent::receiveEvent(e);
}

void AudioComponent::setParent(Entity* e)
{
    BaseComponent::setParent(e);
    e->registerListener(Event::TRANSLATE, this);
}