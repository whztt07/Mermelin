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
 * @file ConfigStore.h
 * @brief Enables to save settings in the Ogre configuration file
 * @author Firestryke31, Eduardo Hahn Paredes
 */

#ifndef CONFIGSTORE_H
#define	CONFIGSTORE_H

namespace CotopaxiEngine {

    /**
     * @class ConfigStore
     * @brief Enables to save settings in the Ogre configuration file
     * 
     * This can be useful to write back changes in the configuration.
     */
    class ConfigStore : public Ogre::ConfigFile
    {
    public:
        void setSetting(const Ogre::String &key, const Ogre::String &section, const Ogre::String &value, bool append = false)
        {
            Ogre::ConfigFile::SettingsMultiMap* currentSettings;

            // finding the section
            Ogre::ConfigFile::SettingsBySection::const_iterator seci = mSettings.find(section);

            if (seci == mSettings.end()) {
                // if the section doesn't exist, creating it
                currentSettings = OGRE_NEW_T(SettingsMultiMap, Ogre::MEMCATEGORY_GENERAL)();
                mSettings[section] = currentSettings;
            } else {
                currentSettings = seci->second;
            }

            if (!append) {
                // removing the existing setting(s)
                Ogre::ConfigFile::SettingsMultiMap::iterator seti = currentSettings->find(key);

                if (seti != currentSettings->end())
                    currentSettings->erase(seti);
            }

            // adding the new setting
            currentSettings->insert(Ogre::ConfigFile::SettingsMultiMap::value_type(key, value));
        }
    };
}

#endif