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
 * @file Audio.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Resources/Files/Audio.h"

using namespace CotopaxiEngine;

Audio::Audio(Ogre::ResourceManager* creator,
        const Ogre::String &name,
        Ogre::ResourceHandle handle,
        const Ogre::String &group,
        MODE mode,
        bool isManual,
        Ogre::ManualResourceLoader *loader)
: Ogre::Resource(creator, name, handle, group, isManual, loader)
{
    createParamDictionary("Audio");
    this->mode = mode;
}

Audio::~Audio()
{
    unload(); 
}

void Audio::loadImpl()
{
    Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(mName, 
            mGroup);

    std::vector<char> bytes;

    // settin output values
    bytes.resize(stream->size());

    // getting the binary file contents
    stream->read(&bytes[0], bytes.size());

    buffer = new sf::SoundBuffer();
    buffer->LoadFromMemory(&bytes[0], bytes.size());

    sound = new sf::Sound(*buffer);
}

void Audio::unloadImpl() 
{
    delete buffer;
    delete sound;
}

size_t Audio::calculateSize() const
{
    return 0;
}

sf::Sound* Audio::getAudio()
{
    return this->sound;
}