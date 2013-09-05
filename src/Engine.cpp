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

Engine::~Engine()
{
    unload();
}

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
    unloadAllModules();
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

    if (modules[moduleType] == NULL) {
        switch (moduleType) {
            case MODULE_AUDIO:
            {
                audio = new AudioModule();
                module = audio;
                break;
            }
            case MODULE_GRAPHIC:
            {
                graphic = new GraphicModule();
                module = graphic;
                break;
            }
            case MODULE_INPUT:
            {
                input = new InputModule();
                module = input;
                break;
            }
            case MODULE_LOGIC:
            {
                logic = new LogicModule();
                module = logic;
                break;
            }
            case MODULE_PHYSICS:
            {
                physics = new PhysicsModule();
                module = physics;
                break;
            }
            case MODULE_GUI:
            {
                gui = new GUIModule();
                module = gui;
                break;
            }
            default:
            {
                LogManager::getSingleton().logMessage(LML_CRITICAL, moduleType + " not found.");
                return;
            }
        }

        if (module) {
            module->load();
            modules[moduleType] = module;
        }
    }
}

void Engine::unloadModule(ModuleType moduleType)
{
    modules[moduleType]->unload();
    delete modules[moduleType];
    modules[moduleType] = NULL;
}

void Engine::unloadAllModules()
{
    unloadModule(MODULE_GUI);
    unloadModule(MODULE_LOGIC);
    unloadModule(MODULE_AUDIO);
    unloadModule(MODULE_GRAPHIC);
    unloadModule(MODULE_PHYSICS);
    unloadModule(MODULE_INPUT);
    modules.clear();
}

BaseModule* Engine::getModule(ModuleType moduleType)
{
    BaseModule* ptr;
    ptr = modules[moduleType];
    if (!ptr) {
        LogManager::getSingleton().logMessage(LML_CRITICAL, moduleType + " not in list");
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

CotopaxiEngine::Camera* Engine::getCamera()
{
    if (!camera) {
        camera = new Camera();
    }
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

void Engine::loadSections(string resourceFile)
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
    std::map<std::string, Entity*>::iterator it = entities.find(name);
    if (it != entities.end()) {
        return it->second;
    }

    return NULL;
}

CotopaxiEngine::Entity* Engine::createEntity(std::string name, std::string meshName,
        Ogre::SceneNode* parentNode)
{
    if (getEntity(name) == NULL) {
        CotopaxiEngine::Entity* entityToCreate = new Entity(name, meshName, parentNode);
        entities[name] = entityToCreate;
        return entityToCreate;
    }

    LogManager::getSingleton().logMessage(LML_TRIVIAL, "Entity " + name
            + " already exists", true);
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
    return audio;
}

GraphicModule* Engine::getGraphic()
{
    return graphic;
}

InputModule* Engine::getInput()
{
    return input;
}

LogicModule* Engine::getLogic()
{
    return logic;
}

PhysicsModule* Engine::getPhysics()
{
    return physics;
}

GUIModule* Engine::getGUI()
{
    return gui;
}

void Engine::throwEvent(const Event::EventType& type)
{
    Event* event = new Event(type);
    std::vector<EventListener*>::iterator i;
    for (i = eventListeners[type].begin(); i != eventListeners[type].end(); i++) {
        (*i)->receiveEvent(event);
    }
}

void Engine::throwEvent(CotopaxiEngine::Event* event)
{
    if (event->getType() == Event::LEVEL_END) {
        CotopaxiEngine::Event* next = new Event(Event::LEVEL_READY_FOR_NEXT);
        ParallelEvent* parallel = new ParallelEvent(next, event->getEntity());
        parallel->Launch();
    } else {
        std::vector<EventListener*>::iterator i;
        for (i = eventListeners[event->getType()].begin(); i != eventListeners[event->getType()].end(); i++) {
            (*i)->receiveEvent(event);
        }
    }
}

void Engine::registerForEventType(CotopaxiEngine::EventListener* entity, const Event::EventType& type)
{
    eventListeners[type].push_back(entity);
}

void Engine::registerFactoryMethod(std::string name, ProduceEntity prdEnt)
{
    factoryMap[name] = prdEnt;
}

CotopaxiEngine::Entity* Engine::produceEntity(std::string type, std::string name,
        Ogre::SceneNode* parentNode)
{
    FactoryMap::iterator i = factoryMap.find(type);

    if (i != factoryMap.end()) {
        Entity* entity = i->second(name, parentNode);
        entity->setParentNode(parentNode);
        entity->setName(name);

        entities[name] = entity;

        return entity;
    }

    LogManager::getSingleton().logMessage(LML_TRIVIAL, name + " does not exist", true);

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

    std::map<int, BaseModule*>::iterator i = modules.begin();
    do {
        if (!i->second->preUpdate(evt)) {
            return false;
        }
        i++;
    } while (i != modules.end());

    return true;
}

bool Engine::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    std::map<int, BaseModule*>::iterator i = modules.begin();
    do {
        if (!i->second->update(evt)) {
            return false;
        }
        i++;
    } while (i != modules.end());

    return true;
}

bool Engine::frameEnded(const Ogre::FrameEvent& evt)
{
    std::map<int, BaseModule*>::iterator i = modules.begin();

    do {
        if (!i->second->postUpdate(evt)) {
            return false;
        }
        i++;
    } while (i != modules.end());

    return true;
}

void Engine::removeAllEntities()
{
    std::map<std::string, Entity*>::iterator i;
    for (i = entities.begin(); i != entities.end(); i++) {
        if (i->second) {
            Camera* camera = dynamic_cast<Camera*> (i->second);
            if (!camera) {
                removeEntity(i->second);
            }
        }
    }
    entities.clear();
    
}

void Engine::removeEntity(CotopaxiEngine::Entity* entity)
{
    Ogre::LogManager::getSingleton().logMessage(LML_TRIVIAL, "Removing  " + entity->getName());
    std::map<std::string, Entity*>::iterator i;
    i = entities.find(entity->getName());

    if (i != entities.end()) {
        try {
            if (entity) {
                entities.erase(i);
                delete entity;
            }
        } catch (...) {
            Ogre::LogManager::getSingleton().logMessage("Couldn't properly remove "
                    + entity->getName());
        }
    }
}

Engine::ParallelEvent::ParallelEvent(Event* e, EventListener* t)
: event(e), target(t) { }

void Engine::ParallelEvent::Run()
{
    target->receiveEvent(event);
}
