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
 * @file AudioManager.h
 * @brief Responsible for accessing audio resources.
 * @author Eduardo Hahn Paredes
 */
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <OgreResourceManager.h>
#include "Resources/Files/Audio.h"

namespace CotopaxiEngine {

    /**
     * @class AudioManager
     * @brief Responsible for accessing audio resources.
     */
    class AudioManager : public Ogre::ResourceManager, public Ogre::Singleton<AudioManager>
    {
    protected:
        // must implement this from ResourceManager's interface
        Ogre::Resource *createImpl(const Ogre::String &name,
                Ogre::ResourceHandle handle,
                const Ogre::String &group,
                bool isManual,
                Ogre::ManualResourceLoader *loader,
                const Ogre::NameValuePairList *createParams);
    public:

        AudioManager();
        virtual ~AudioManager();

        /**
         * @fn load
         * Loads sound files using the resource manager from Ogre.
         * @param name Name of the level file (ex. level1.txt")
         * @param group The resource group in Ogre
         * @return An instance of Level.
         * @param mode An Audio::MODE
         * @return An instance of Audio.
         */
        virtual AudioPtr load(const Ogre::String &name, const Ogre::String &group, Audio::MODE mode = Audio::SOUND);

        /**
         * @fn getSingleton
         * Makes use of the Ogre template class OgreSingleton to creating a single-instance
         * of AudioManager.
         * @return An AudioManager instance.
         */
        static AudioManager &getSingleton();

        /**
         * @fn getSingletonPtr
         * Makes use of the Ogre template class OgreSingleton to creating a single-instance
         * of AudioManager.
         * @return A pointer to an AudioManager instance.
         */
        static AudioManager *getSingletonPtr();
    };
}

#endif