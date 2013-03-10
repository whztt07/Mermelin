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
 * AudioModule.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Components/AudioComponent.h"
#include "Entities/Camera.h"

using namespace CotopaxiEngine;

AudioModule::~AudioModule()
{
    unload();
}

PlaySound* AudioModule::playSound(std::string path, bool loop)
{
    PlaySound* sound = new PlaySound(path, loop);
    sound->play();
    return sound;
}

PlayMusic* AudioModule::playMusic(std::string path, bool loop)
{
    PlayMusic* music = new PlayMusic(path, loop);
    music->play();
    return music;
}

void AudioModule::stopSound(PlaySound* sound)
{
    delete sound;
    sound = NULL;
}

void AudioModule::stopMusic(PlayMusic* music)
{
    delete music;
    music = NULL;
}

BaseComponent* AudioModule::getComponent(Entity* parent)
{
    AudioComponent* component = new AudioComponent();
    component->setParent(parent);
    components.push_back(component);
    return (BaseComponent*) components.back();
}

void AudioModule::setGlobalVolume(float volume)
{
    float internalVolume = volume;

    if (internalVolume > 100.0) {
        internalVolume = 100.0;
    }
    if (internalVolume < 0.0) {
        internalVolume = 0.0;
    }

    sf::Listener::SetGlobalVolume(internalVolume);
}

bool AudioModule::update(const Ogre::FrameEvent &evt)
{
    Ogre::Vector3 position = ENGINE->getCamera()->getNode()->_getDerivedPosition();
    sf::Listener::SetPosition(position.x, position.y, position.z);
    return true;
}

void AudioModule::unload()
{
    for (int i = 0; i < components.size(); i++) {
        delete components[i];
        components[i] = NULL;
    }
    components.clear();
}