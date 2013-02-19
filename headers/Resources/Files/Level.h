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
@file Level.h
@brief It's all about level parsing.
@author Eduardo Hahn Paredes
 */

#ifndef LEVEL_H
#define	LEVEL_H

#include "stdafx.h"
#include "Modules/GraphicModule.h"
#include "Engine.h"
#include "Components/PhysicsComponent.h"
#include "Logic/Condition.h"
#include "Logic/Trigger.h"
#include "Entities/Camera.h"
#include "Entities/TypeChanger.h"
#include "Modules/PhysicsModule.h"

using namespace std;

namespace CotopaxiEngine {

    // forward declarations
    class Entity;
    class Door;
    class Engine;
    class GraphicModule;

    /**
     * @class Level
     * @brief This class is a parses the level
     * 
     * It loads a level from file, parses the containing script and creates all necessary Entities.
     */
    class Level : public Ogre::Resource
    {
#ifdef DEBUG
        friend class Test;
#endif

    protected:
        void loadImpl();
        void unloadImpl();
        size_t calculateSize() const
        {
            return 0;
        };
    public:

        /**
         * @enum State
         * Defines all types of states in
         * - FREE               Free object placement state @link readFreeLine @endlink
         * - LOGIC              Reading logic sector @link readLogicLine @endlink
         * - OBJECTS            Reading objects sector @link readObjectLine @endlink
         * - NONE               No state
         * - PROPERTIES         Reading the properties sector @link readPropertiesLine @endlink
         */
        enum State
        {
            FREE,
            LOGIC,
            OBJECTS,
            NONE,
            PROPERTIES
        };

        Level(Ogre::ResourceManager *creator,
                const Ogre::String& name,
                Ogre::ResourceHandle handle,
                const Ogre::String& group,
                bool isManual = false,
                Ogre::ManualResourceLoader* loader = 0);

        virtual ~Level();

        void readFile(string path);
        
        /**
         * @fn getLevelName()
         * Shows the name of the level.
         * @return The level name
         */
        std::string getLevelName() const;

    private:
        State state;
        Ogre::SceneNode* levelNode;
        Ogre::Vector3 tempVec;
        std::string levelName;

		std::vector<Entity*> entityPointers;

        static const Ogre::Quaternion rot90; //  90 degree rotation around y-axis 
        static const Ogre::Quaternion rot180; // 180 degree rotation around y-axis 
        static const Ogre::Quaternion rot270; // 270 degree rotation around y-axis 

        int objectCounter;
        int lineCounter;
        int floorCounter;

        Entity* readObject(char object, std::string name);

        void readFreeLine(string line);
        void readLogicLine(string line);
        void readObjectLine(string line);
        void readPropertiesLine(string line);
        void transform(Entity* entity, Ogre::Vector3 vec,
                float mass = 0.0, int group = PhysicsModule::COL_NOTHING,
                int mask = PhysicsModule::COL_NOCOLL, bool trigger = false);
        Ogre::Vector3 readVector(string vectorString);
        LogicComponent* buildConditionTree(std::string expression, Entity* entity);
        void readLine(string line);
    };

    /**
     * @class LevelPtr
     * @brief LevelPtr to an instance of Level
     */
    class LevelPtr : public Ogre::SharedPtr<Level>
    {
    public:
        LevelPtr() : Ogre::SharedPtr<Level>() { }
        explicit LevelPtr(Level* rep) : Ogre::SharedPtr<Level>(rep) { }
        LevelPtr(const LevelPtr& resource) : Ogre::SharedPtr<Level>(resource) { }
        LevelPtr(const Ogre::ResourcePtr& resource) : Ogre::SharedPtr<Level>()
        {
            if (resource.isNull())
                return;

            OGRE_LOCK_MUTEX(*resource.OGRE_AUTO_MUTEX_NAME)
            OGRE_COPY_AUTO_SHARED_MUTEX(resource.OGRE_AUTO_MUTEX_NAME)

            pRep = static_cast<Level*> (resource.getPointer());
            pUseCount = resource.useCountPointer();
            useFreeMethod = resource.freeMethod();

            if (pUseCount) {
                ++(*pUseCount);
            }
        }
        LevelPtr& operator=(const Ogre::ResourcePtr& resourece)
        {
            if (pRep == static_cast<Level*> (resourece.getPointer()))
                return *this;

            release();
            if (resourece.isNull())
                return *this;

            OGRE_LOCK_MUTEX(*resourece.OGRE_AUTO_MUTEX_NAME)
            OGRE_COPY_AUTO_SHARED_MUTEX(resourece.OGRE_AUTO_MUTEX_NAME)
            pRep = static_cast<Level*> (resourece.getPointer());

            pUseCount = resourece.useCountPointer();
            useFreeMethod = resourece.freeMethod();
            
            if (pUseCount) {
                ++(*pUseCount);
            }

            return *this;
        }
    };
}

#endif