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
 * GraphicModule.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Modules/GraphicModule.h"
#include "Engine.h"
#include "Entities/Camera.h"

using namespace CotopaxiEngine;

GraphicModule::GraphicModule() { }

BaseComponent* GraphicModule::getComponent(Entity* parent)
{
    GraphicComponent* component = new GraphicComponent();
    component->setParent(parent);
    components[parent->getName()] = component;

    return (BaseComponent*) components[parent->getName()];
}

void GraphicModule::removeComponent(std::string componentName)
{
    std::map<std::string, GraphicComponent*>::iterator i = components.find(componentName);          
    if (i != components.end()) {
        components.erase(i);
    }
}

bool GraphicModule::frameRenderingQueued(const Ogre::FrameEvent& event)
{
    if (ENGINE->getCamera()->isAttached()) {
        ENGINE->getCamera()->update();
    }
    return true;
}

void GraphicModule::refreshAllShaders()
{
    std::map<std::string, GraphicComponent*>::const_iterator i;
    for (i = components.begin(); i != components.end(); i++) {    
        if(i->second->hasShader()) {
            i->second->reloadShader();
        }
    }

    Ogre::GpuProgramManager::getSingletonPtr()->reloadAll();
}

void GraphicModule::receiveEvent(Event* e) { }