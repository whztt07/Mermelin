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
 * Engine.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Engine.h"
#include "Components/GraphicComponent.h"

//Entities
#include "Entities/Camera.h"
#include "Entities/Rotor.h"
#include "Entities/WoodWall.h"
#include "Entities/Rotor.h"
#include "Entities/Goal.h"
#include "Entities/Door.h"
#include "Entities/Button.h"
#include "Entities/GroundPlate.h"

//Modules
#include "Modules/InputModule.h"
#include "Modules/GraphicModule.h"
#include "Modules/PhysicsModule.h"
#include "Modules/AudioModule.h"
#include "Modules/GUIModule.h"

//Resources
#include "Resources/Managers/LevelManager.h"
#include "Resources/Managers/AudioManager.h"
#include "GameStates/IntroState.h"


using namespace std;
using namespace CotopaxiEngine;
using namespace Ogre;

void loadSections(string resourceFile);

bool Engine::load(string title)
{
    string resourceFile = "resources.cfg";
    string pluginsFile = "plugins.cfg";

    // constructing Root
    root = new Root(pluginsFile);

    new LevelManager();
    new AudioManager();

    // loading resources
    loadSections(resourceFile);

    // showing config dialog if no config avaiable
    if (!(root->restoreConfig() || root->showConfigDialog())) {
        return false;
    }

    window = root->initialise(true, title); //window title

    // setting mipmaping level
    TextureManager::getSingleton().setDefaultNumMipmaps(5);
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // creating the SceneManager
    sceneManager = root->createSceneManager("DefaultSceneManager");

    // enabling shadows
    sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
    sceneManager->setShadowFarDistance(Ogre::Real(80));

    // disabling ambient light
    sceneManager->setAmbientLight(Ogre::ColourValue::ZERO);

    // creating the camera   
    camera = new Camera();

    loadAllModules();

    //Registering entities
    registerFactoryMethod("button", &CotopaxiEngine::Button::create);
    registerFactoryMethod("goal", &CotopaxiEngine::Goal::create);
    registerFactoryMethod("woodwall", &CotopaxiEngine::WoodWall::create);
    registerFactoryMethod("rotor", &CotopaxiEngine::Rotor::create);
    registerFactoryMethod("entity", &CotopaxiEngine::Entity::create);
    registerFactoryMethod("sphere", &CotopaxiEngine::Sphere::create);
    registerFactoryMethod("camera", &CotopaxiEngine::Camera::create);
    registerFactoryMethod("door", &CotopaxiEngine::Door::create);
    registerFactoryMethod("typechanger", &CotopaxiEngine::TypeChanger::create);
    registerFactoryMethod("groundplate", &CotopaxiEngine::GroundPlate::create);
    return true;
}

void Engine::loadAllModules()
{
    loadModule(MODULE_INPUT);
    loadModule(MODULE_PHYSICS);
    loadModule(MODULE_GRAPHIC);
    loadModule(MODULE_AUDIO);
    loadModule(MODULE_LOGIC);
    loadModule(MODULE_GUI);
}

bool Engine::unload(void)
{
    delete Ogre::ResourceGroupManager::getSingleton()._getResourceManager("Level");
    return true;
}

void Engine::start(void)
{
    root->addFrameListener(this);
    this->pushState(new IntroState());
    root->startRendering();
}

void Engine::loadModule(ModuleType moduleType)
{
    BaseModule* module = NULL;

    if (moduleList[moduleType] == NULL) {
        switch (moduleType) {
            case MODULE_AUDIO:
            {
                module = new AudioModule();
                break;
            }
            case MODULE_GRAPHIC:
            {
                module = new GraphicModule();
                break;
            }
            case MODULE_INPUT:
            {
                module = new InputModule();
                break;
            }
            case MODULE_LOGIC:
            {
                module = new LogicModule();
                break;
            }
            case MODULE_PHYSICS:
            {
                module = new PhysicsModule();
                break;
            }
            case MODULE_GUI:
            {
                module = new GUIModule();
                break;
            }
            default:
            {
                LogManager::getSingleton().logMessage("[ENGINE]Error: Module not found");
                return;
            }
        }

        if (module != NULL) {
            module->load();
            moduleList[moduleType] = module;
        }
    }
}

void Engine::unloadModule(ModuleType moduleType)
{
    moduleList[moduleType]->unload();
    delete moduleList[moduleType];
}

void Engine::unloadAllModules()
{
    unloadModule(MODULE_GUI);
    unloadModule(MODULE_LOGIC);
    unloadModule(MODULE_AUDIO);
    unloadModule(MODULE_GRAPHIC);
    unloadModule(MODULE_PHYSICS);
    unloadModule(MODULE_INPUT);
    moduleList.clear();
}

BaseModule* Engine::getModule(ModuleType moduleType)
{
    BaseModule* ptr;
    ptr = moduleList[moduleType];
    if (!ptr) {
        LogManager::getSingleton().logMessage(LML_NORMAL, "[ENGINE]: MODULE");
    }
    return ptr;
}

// Singleton implementation
Engine* Engine::instance = 0;

Engine* Engine::getInstance()
{
    if (!instance) {
        instance = new Engine();
        return instance;
    } else {
        return instance;
    }
}

CotopaxiEngine::Camera* Engine::getCamera() const
{
    return camera;
}

SceneManager* Engine::getSceneManager() const
{
    return sceneManager;
}

RenderWindow* Engine::getWindow() const
{
    return window;
}

Root* Engine::getRoot() const
{
    return root;
}

void loadSections(string resourceFile)
{
    ConfigFile cf;
    cf.load(resourceFile);

    //Going through all sections in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    String secName, typeName, archName;
    while (seci.hasMoreElements()) {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i) {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
        }
    }
}

CotopaxiEngine::Entity* Engine::getEntity(std::string name)
{
    std::map<std::string, Entity*>::iterator it = entityList.find(name);
    if (it != entityList.end()) {
        return it->second; //element found;
    }

    return NULL;
}

CotopaxiEngine::Entity* Engine::createEntity(std::string name, std::string meshName,
        Ogre::SceneNode* parentNode)
{
    if (getEntity(name) == NULL) {
        CotopaxiEngine::Entity* entityToCreate = new Entity(name, meshName, parentNode);
        entityList[name] = entityToCreate;
        return entityList[name];
    }

    LogManager::getSingleton().logMessage(LML_TRIVIAL, "[ENGINE] Entity already exists", true);
    return NULL;
}

CotopaxiEngine::Entity* Engine::createEntity(std::string name, std::string meshName)
{
    return this->createEntity(name, meshName, sceneManager->getRootSceneNode());
}

CotopaxiEngine::Entity* Engine::createEntity(std::string name)
{
    return this->createEntity(name, name);
}

AudioModule* Engine::getAudio()
{
    return (AudioModule*) moduleList[MODULE_AUDIO];
}

GraphicModule* Engine::getGraphic()
{
    return (GraphicModule*) moduleList[MODULE_GRAPHIC];
}

InputModule* Engine::getInput()
{
    return (InputModule*) moduleList[MODULE_INPUT];
}

LogicModule* Engine::getLogic()
{
    return (LogicModule*) moduleList[MODULE_LOGIC];
}

PhysicsModule* Engine::getPhysics()
{
    return (PhysicsModule*) moduleList[MODULE_PHYSICS];
}

GUIModule* Engine::getGUI()
{
    return (GUIModule*) moduleList[MODULE_GUI];
}

void Engine::throwEvent(const Event::EventType& type)
{
    Event event(type);

    std::vector<EventListener*>::iterator receiverIterator = eventTypeListenersMap[type].begin();
    for (; receiverIterator != eventTypeListenersMap[type].end(); ++receiverIterator) {
        (*receiverIterator)->receiveEvent(&event);
    }
}

void Engine::registerForEventType(CotopaxiEngine::EventListener* entity, const Event::EventType& type)
{
    eventTypeListenersMap[type].push_back(entity);
}

void Engine::registerFactoryMethod(std::string name, ProduceEntity prdEnt)
{
    factoryMap[name] = prdEnt;
}

CotopaxiEngine::Entity* Engine::produceEntity(std::string type, std::string name,
        Ogre::SceneNode* parentNode)
{
    FactoryMap::iterator it = factoryMap.find(type);

    if (it != factoryMap.end()) {
        Entity* entity = it->second(name, parentNode);
        entity->setParentNode(parentNode);
        entity->setName(name);

        entityList[name] = entity;

        return entityList[name];
    }

    LogManager::getSingleton().logMessage(LML_TRIVIAL, "[ENGINE] Method to create " + name
            + " does not exist", true);

    return NULL;
}

void Engine::pushState(GameState* gameState)
{
    gameStates.push(gameState);
    if (!gameStates.top()->isLoaded()) {
        gameStates.top()->load();
    }
}

void Engine::popState()
{
    gameStates.pop();
}

bool Engine::frameStarted(const Ogre::FrameEvent& evt)
{
    gameStates.top()->frameStarted(evt);
    std::map<int, BaseModule*>::iterator moduleIterator = moduleList.begin();
    do {
        if (!moduleIterator->second->frameStarted(evt)) {
            return false;
        }
        moduleIterator++;
    } while (moduleIterator != moduleList.end());

    return true;
}

bool Engine::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    std::map<int, BaseModule*>::iterator moduleIterator = moduleList.begin();
    do {
        if (!moduleIterator->second->frameRenderingQueued(evt)) {
            return false;
        }

        moduleIterator++;
    } while (moduleIterator != moduleList.end());

    return true;
}

bool Engine::frameEnded(const Ogre::FrameEvent& evt)
{
    std::map<int, BaseModule*>::iterator moduleIterator = moduleList.begin();

    do {
        if (!moduleIterator->second->frameEnded(evt)) {
            return false;
        }
        moduleIterator++;
    } while (moduleIterator != moduleList.end());

    return true;
}

void Engine::removeEntity(CotopaxiEngine::Entity* entity)
{
    std::map<std::string, Entity*>::iterator toErase;
    try {
        toErase = entityList.find(entity->getName());
    } catch (std::exception& e) {
        Ogre::LogManager::getSingleton().logMessage("Couldn't properly remove an entity");
        toErase = entityList.end();
    }

    if (toErase != entityList.end()) {
        try {
            entityList.erase(toErase);
        } catch (std::exception& e) {
            Ogre::LogManager::getSingleton().logMessage("Couldn't properly remove an entity");
        }
    }

    delete entity;
}