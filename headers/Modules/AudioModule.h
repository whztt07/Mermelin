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
 * @file AudioModule.h
 * @brief Contains the AudioModule.
 *
 * @author Eduardo Hahn Paredes
 */

#ifndef AUDIOMODULE_H
#define AUDIOMODULE_H
#include "stdafx.h"
#include "Modules/BaseModule.h"

namespace CotopaxiEngine
{

    // forward declarations
    class AudioComponent;

    /**
     * @class PlaySound
     * @brief This class is needed for multithreading in audio for sounds (in memory)
     */
    class PlaySound : private sf::Thread
    {
    public:

        PlaySound(std::string path, bool loop)
        : path(path), loop(loop) { }

        virtual ~PlaySound() {
            delete sound;
            delete buffer;
            sound = NULL;
            buffer = NULL;
        }

        void play() {
            Launch();
        }

        sf::Sound* getSound() const {
            return sound;
        }

    private:
        bool loop;
        std::string path;
        sf::SoundBuffer* buffer;
        sf::Sound* sound;

        virtual void Run() {
            buffer = new sf::SoundBuffer();
            if (!buffer->LoadFromFile(path)) {
                throw std::exception();
            }
            sound = new sf::Sound();
            sound->SetBuffer(*buffer);
            sound->SetLoop(loop);
            sound->Play();
        }
    };

    /**
     * @class PlayMusic
     * @brief This class is needed for multithreading in audio for music (streaming)
     */
    class PlayMusic : private sf::Thread
    {
    public:

        PlayMusic(std::string path, bool loop)
        : path(path), loop(loop) { }

        virtual ~PlayMusic() {
            delete music;
            music = NULL;
        }

        void play() {
            Launch();
        }

        sf::Music* getMusic() const {
            return music;
        }

    private:
        bool loop;
        std::string path;
        sf::Music* music;

        virtual void Run() {
            music = new sf::Music();
            if (!music->OpenFromFile(path)) {
                throw std::exception();
            }
            music->SetLoop(loop);
            music->Play();
        }
    };

    /**
     * @class AudioModule
     * @brief This is the connection to the audio library SMFL
     * 
     * This class is the main interface to the audio library of SMFL.
     */
    class AudioModule : public BaseModule
    {
    public:
        
        virtual ~AudioModule();
        
        /**
         * @fn playSound
         * @param path where the file is located
         * @param loop the sound will be looped if true
         * @return a pointer to PlaySound
         */
        PlaySound* playSound(std::string path, bool loop = false);
        
        /**
         * @fn playMusic
         * @param path where the file is located
         * @param loop the music will be looped if true
         * @return a pointer to PlayMusic
         */
        PlayMusic* playMusic(std::string path, bool loop = false);
        
        /**
         * @fn stopSound
         * Stops the playback of the sound
         * @param sound pointer to the sound that shall be stopped.
         */
        void stopSound(PlaySound* sound);
        
        /**
         * @fn stopMusic
         * Stops the playback of the music
         * @param sound pointer to the music that shall be stopped.
         */
        void stopMusic(PlayMusic* music);
        
        /**
         * @fn setGlobalVolume
         * @param volume A float between 0.0 (mute) and 100.0 (loud)
         */
        void setGlobalVolume(float volume);
        
        void unload();
        
        BaseComponent* getComponent(Entity* entity);
        virtual bool update(const Ogre::FrameEvent &evt);

    private:
        std::vector<AudioComponent*> components;
    };
}
#endif