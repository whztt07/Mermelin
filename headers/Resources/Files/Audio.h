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
 * @file Audio.h
 * @brief Contains classes needed to load an audio file by Ogre
 * @author Eduardo Hahn Paredes
 */

#ifndef AUDIO_H
#define AUDIO_H

#include <OgreResourceManager.h>
#include "SFML/Audio.hpp"

namespace CotopaxiEngine
{

    /**
     * @class Audio
     * @brief Loads and plays a sound
     * 
     * The Audio implementation works as follows: The AudioModule creates an 
     * AudioComponent which has a list of Audio instances that are loaded by the AudioManager.
     */
    class Audio : public Ogre::Resource, private sf::Thread
    {
    protected:
        void loadImpl();
        void unloadImpl();
        size_t calculateSize() const;

    public:

        /**
         * @enum MODE
         * There are 2 MODES:
         * - SOUND      Fully loaded into memory
         * - MUSIC      Streamed
         */
        enum MODE
        {
            SOUND,
            MUSIC
        };

        /**
         * The constructor of audio takes several arguments.
         * 
         * @param creator
         * @param name name of the file
         * @param handle pointer to the resourcegroup 
         * @param group the group-name
         * @param mode SOUND or MUSIC
         * @param isManual defines if a ManualResourceLoader shall be used
         * @param loader pointer to a ManualResourceLoader from Ogre
         */
        Audio(Ogre::ResourceManager* creator,
                const Ogre::String &name,
                Ogre::ResourceHandle handle,
                const Ogre::String &group,
                MODE mode,
                bool isManual = false,
                Ogre::ManualResourceLoader* loader = 0);

        virtual ~Audio();

        /**
         * @fn play
         * Plays th sound / music.
         */
        void play();

        /**
         * Sets the mode for Audio
         * @param mode SOUND or MUSIC
         */
        void setMode(MODE mode) {
            this->mode = mode;
        }

        /**
         * Indicates the mode
         * @return can be SOUND or MUSIC
         */
        MODE getMode() {
            return this->mode;
        }

        /**
         * Get the SMFL Sound
         * @return A pointer to a SMFL Sound instance
         */
        sf::Sound* getAudio();

    private:
        std::string path;
        sf::SoundBuffer* buffer;
        sf::Sound* sound;

        MODE mode;

        virtual void Run() {
            buffer = new sf::SoundBuffer();
            if (!buffer->LoadFromFile(path)) {
                throw std::exception();
            }
            sound = new sf::Sound();
            sound->SetBuffer(*buffer);
            sound->Play();
        }
    };

    /**
     * @class AudioPtr
     * @brief AudioPtr to an instance of Audio
     */
    class AudioPtr : public Ogre::SharedPtr<Audio>
    {
    public:

        AudioPtr()
        : Ogre::SharedPtr<Audio>() { }

        explicit AudioPtr(Audio *rep) : Ogre::SharedPtr<Audio>(rep) { }

        AudioPtr(const AudioPtr &resource) : Ogre::SharedPtr<Audio>(resource) { }

        AudioPtr(const Ogre::ResourcePtr &resource) : Ogre::SharedPtr<Audio>() {
            if (resource.isNull()) {
                return;
            }

            OGRE_LOCK_MUTEX(*resource.OGRE_AUTO_MUTEX_NAME)
            OGRE_COPY_AUTO_SHARED_MUTEX(resource.OGRE_AUTO_MUTEX_NAME)

            pRep = static_cast<Audio*> (resource.getPointer());
            pUseCount = resource.useCountPointer();
            useFreeMethod = resource.freeMethod();

            if (pUseCount) {
                ++(*pUseCount);
            }
        }

        /**
         * Operator used to convert a ResourcePtr to a AudioPtr
         * @param r ResourcePtr from Ogre
         * @return AudioPtr
         */
        AudioPtr& operator=(const Ogre::ResourcePtr& resource) {
            if (pRep == static_cast<Audio*> (resource.getPointer()))
                return *this;
            release();

            if (resource.isNull()) {
                return *this;
            }

            OGRE_LOCK_MUTEX(*resource.OGRE_AUTO_MUTEX_NAME)
            OGRE_COPY_AUTO_SHARED_MUTEX(resource.OGRE_AUTO_MUTEX_NAME)

            pRep = static_cast<Audio*> (resource.getPointer());
            pUseCount = resource.useCountPointer();
            useFreeMethod = resource.freeMethod();

            if (pUseCount) {
                ++(*pUseCount);
            }
            return *this;
        }
    };
}

#endif