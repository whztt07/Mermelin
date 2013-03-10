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
 * @file Engine.h
 * @brief The main class for handeling our objects and states. 
 * @author Eduardo Hahn Paredes
 * The Engine class is implemented as a singleton
 */

#ifndef ENGINE_H
#define ENGINE_H

#include "stdafx.h"
#include <stack>
#include "Modules/BaseModule.h"
#include "Entities/Entity.h"
#include "Shaders/Shader.h"

// Engine is a singleton and can be accessed with ENGINE from everywhere
#define ENGINE Engine::getInstance()

namespace CotopaxiEngine {

    // forward delcarations
    class Entity;
    class AudioModule;
    class PhysicsModule;
    class GraphicModule;
    class InputModule;
    class LogicModule;
    class GUIModule;
    class GameState;
    class Camera;
    class GameState;
    class IntroState;

    /**
     * @class Engine
     * 
     * @brief Main engine class of the CotopaxiEngine
     *
     * This class is the main hub for the engine. It handles creation of game objects, 
     * management of modules and starts up and shuts down everything needed.
     */
    class Engine : public Ogre::FrameListener
    {
    public:
        
        // Singleton: private constructor and instance
        Engine() { }
        static Engine* instance;
        
        virtual ~Engine();

        /**
         * @enum ModuleType
         * List of all avaiable modules:
         * - MODULE_GRAPHIC             GraphicModule
         * - MODULE_INPUT               InputModule
         * - MODULE_AUDIO               AudioModule
         * - MODULE_PHYSICS             PhysicsModule
         * - MODULE_LOGIC               LogicModule
         * - MODULE_GUI                 GUIModule
         */
        enum ModuleType
        {
            MODULE_GRAPHIC,
            MODULE_INPUT,
            MODULE_AUDIO,
            MODULE_PHYSICS,
            MODULE_LOGIC,
            MODULE_GUI
        };

        /**
         * @enum State
         * States that the engine can be in:
         * - STATE_RUNNING              Default
         * - STATE_DEBUGING             Debug-draw is enabled in GraphicModule
         * - STATE_STOPPED              Engine is stopped
         */
        enum State
        {
            STATE_RUNNING,
            STATE_DEBUGING,
            STATE_STOPPED,
        };

        /** @brief Scaling of the world. */
        static const int SCALE = 1;

        /**
         * @fn load
         * Loads the engine with the specified (window) title
         * @param name The title for the engine
         */
        bool load(std::string title);

        /**
         * @fn unload
         * Shuts the engine down
         */
        bool unload(void);

        /**
         * @fn start
         * Starts the engine
         * @attention This function should not be called before the engine has loaded
         * @see load
         */
        void start(void);

        /**
         * @fn loadModule
         * Loads the specified Module
         * @param moduleType The moduletype that needs to be loaded
         */
        void loadModule(ModuleType moduleType);
        
        /**
         * @fn unloadModule
         * Unloads the specified Module
         * @param moduleType The moduletype that needs to be unloaded
         */
        void unloadModule(ModuleType moduleType);
        
        /**
         * @fn loadAllModules
         * Loads every module that exists.
         */
        void loadAllModules();
        
        /**
         * @fn unloadAllModules
         * Unloads every module that exists.
         */
        void unloadAllModules();

        /**
         * @fn getModule
         * Gets the specified module, if it's loaded
         * @param moduleType The requested type of the module
         * @return The desired basemodule, NULL if it's not loaded
         */
        BaseModule* getModule(ModuleType moduleType);

        AudioModule* getAudio();
        GraphicModule* getGraphic();
        InputModule* getInput();
        LogicModule* getLogic();
        PhysicsModule* getPhysics();
        GUIModule* getGUI();

        Camera* getCamera() const;

        /**
         * @fn getInstance
         * The engine is implemented as a singleton, and this function returns a pointer to the 
         * singleton instance.
         * @return The engine instance
         */
        static Engine* getInstance();

        /**
         * @fn getSceneManager
         * Returns the responsible Ogre::SceneManager, that builds and controls the scenegraph.
         * @return The Ogre::SceneManager
         */
        Ogre::SceneManager* getSceneManager() const;

        /**
         * @fn getWindow
         * Gives access to the Ogre::RenderWindow in which the contents of a scene are rendered.
         * @return The Ogre::RenderWindow.
         */
        Ogre::RenderWindow* getWindow() const;

        /**
         * @fn getRoot
         * The Ogre::Root class represents a starting point for the client application. An instance 
         * of Root must be created before any other Ogre operations are called. This is the way
         * to access it later on. Probably it will not be used at all.
         * @return The Ogre::Root used by the Engine.
         */
        Ogre::Root* getRoot() const;

        /**
         * @fn getEntity
         * Gets the entity with the specified name
         * @param name Name of the desired entity
         * @return The desired entity, NULL if it's not found
         */
        Entity* getEntity(std::string name);

        /**
         * @fn createEntity
         * Creates an empty entity with the given name
         * @param name A name for the entity
         * @return Pointer to the newly created entity
         */
        Entity* createEntity(std::string name);

        /**
         * @fn createEntity
         * Creates an empty entity with the given name and a given mesh
         * @param name A name for the entity
         * @param meshName The name of the mesh for this entity
         * @return Pointer to the newly created entity
         */
        Entity* createEntity(std::string name, std::string meshName);

        /**
         * @fn createEntity
         * Creates an empty entity with the given name and a given mesh. This method will 
         * automatically attach the entity to the given parentNode
         * @param name A name for the entity
         * @param meshName The name of the mesh for this entity
         * @param parentNode The parent node for this entity
         * @return Pointer to the newly created entity
         */
        Entity* createEntity(std::string name, std::string meshName, Ogre::SceneNode* parentNode);

        /**
        @fn removeEntity
        Removes and destroys and entity
        @param entity the entity to remove
         */
        void removeEntity(Entity* entity);
        
        /**
        @fn removeAllEntities
        Removes and destroys all entieties.
         */
        void removeAllEntities();
        

        /**
         * @fn registerFactoryMethod
         * This method registers a function pointer to a @see Create method.
         * @param name Typename for the wanted entity
         * @param prdEnt Function pointer to the create method
         */
        void registerFactoryMethod(std::string name, ProduceEntity prdEnt);

        /**
         * @fn produceEntity
         * Creates an entity of the desired type. This approach is based on the factory pattern and 
         * input was taken froman article on codeproject
         * For every desired entity(or subclass thereoff) a create method is registered.
         * @param type The type for the desired entity
         * @param name The name for the new entity
         * @param parentNode The parent node for the new entity
         * @return A pointer to a new entity
         * @sa registerFactoryMethod
         */
        Entity* produceEntity(std::string type, std::string name,
                Ogre::SceneNode* parentNode = ENGINE->getSceneManager()->getRootSceneNode());

        /**
         * @fn registerForEventType
         * Register an entity for a global event
         * @param entity Pointer to the entity to register
         * @param type EventType the entity wants to listen to
         */
        void registerForEventType(CotopaxiEngine::EventListener* entity,
                const CotopaxiEngine::Event::EventType& type);
        /**
         * @fn throwEvent
         * Throws a global event
         * @param type The type of event to be thrown
         */
        void throwEvent(const CotopaxiEngine::Event::EventType& type);
        /**
         * @fn setState
         * Sets the state of the engine
         * @param state New state for the engine
         */
        void setState(State state)
        {
            this->state = state;
        }
        /**
         * @fn getState
         * @return Gets the state of the engine
         */
        State getState() const
        {
            return state;
        }

        /**
         * @fn pushState
         * Pushes a new active GameState onto the gamestate stack
         * @param state The new gamestate
         */
        void pushState(GameState* gameState);

        /**
         * @fn popState
         * Pops the top state from the gamestate stack.
         * @warning this calls the destructor of the top gamestate, so copy it somewhere if you want to keep it
         */
        void popState();

        /**
         * @fn getTopState
         * Returns the topmost gamestate from the gamestate stack
         * @return The Gamestate
         */
        GameState* getTopState();

        bool frameStarted(const Ogre::FrameEvent& evt);
        bool frameRenderingQueued(const Ogre::FrameEvent& evt);
        bool frameEnded(const Ogre::FrameEvent& evt);
        /**
         * @fn setStartPosition
         * Sets the starting point for the sphere.
         * @param startPosition A 3 dimensional vector, pointing to the start pos.
         * @todo Move this to the level
         */
        void setStartPosition(Ogre::Vector3 startPosition)
        {
            this->startPosition = startPosition;
        }
        /**
         * @fn getStartPosition
         * Gets the starting point for the sphere
         * @return Returns a 3 dimensional vector, pointing to the start position
         */
        Ogre::Vector3 getStartPosition() const
        {
            return startPosition;
        }

    private:
        void loadSections(std::string resourceFile);
        
        AudioModule* audio;
        GraphicModule* graphic;
        InputModule* input;
        LogicModule* logic;
        PhysicsModule* physics;
        GUIModule* gui;
        
        typedef std::map<std::string, ProduceEntity> FactoryMap;
        FactoryMap factoryMap;

        State state;
        CotopaxiEngine::Camera* camera;

        Ogre::Root* root;
        Ogre::RenderWindow* window;
        Ogre::SceneManager* sceneManager;
        Ogre::Viewport* viewPort;
        Ogre::Vector3 startPosition;

        std::stack<GameState*> gameStates;
        std::map<int, BaseModule*> modules;       
        std::map<std::string, Entity*> entities;
        std::map<Event::EventType, std::vector<EventListener* >> eventListeners;
    };
}

#endif
