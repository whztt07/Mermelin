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
 * @file LevelManager.h
 * @brief Responsible for accessing level resources.
 *
 * @author Eduardo Hahn Paredes
 */

#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <OgreResourceManager.h>
#include "Resources/Files/Level.h"

namespace CotopaxiEngine {

    /**
     * @class LevelManager
     * @brief Responsible for accessing level resources.
     */
    class LevelManager : public Ogre::ResourceManager, public Ogre::Singleton<LevelManager>
    {
    protected:
        Ogre::Resource* createImpl(const Ogre::String& name,
                Ogre::ResourceHandle handle,
                const Ogre::String& group,
                bool isManual, Ogre::ManualResourceLoader* loader,
                const Ogre::NameValuePairList* createParams);

    public:
        LevelManager();
        virtual ~LevelManager();

        /**
         * @fn load
         * Loads the Level using the resource manager from Ogre.
         * @param name Name of the level file (ex. level1.txt")
         * @param group The resource group in Ogre
         * @return An instance of Level.
         */
        virtual LevelPtr load(const Ogre::String& name, const Ogre::String& group);
        
        /**
         * @fn unload
         * Unloads the Level using the resource manager from Ogre.
         * @param name Name of the level file (ex. level1.txt")
         * @param group The resource group in Ogre
         * @return An instance of Level.
         */
        virtual void unload(const Ogre::String& name, const Ogre::String& group);

        /**
         * @fn getSingleton
         * Makes use of the Ogre template class OgreSingleton to creating a single-instance
         * of LevelManager.
         * @return A LevelManager instance.
         */
        static LevelManager& getSingleton();

        /**
         * @fn getSingletonPtr
         * Makes use of the Ogre template class OgreSingleton to creating a single-instance
         * of LevelManager.
         * @return A pointer to a LevelManager instance.
         */
        static LevelManager* getSingletonPtr();
    };
}


#endif