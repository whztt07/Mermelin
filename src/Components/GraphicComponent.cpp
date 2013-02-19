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
 * GraphicComponent.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Components/GraphicComponent.h"
#include "Engine.h"


using namespace CotopaxiEngine;

GraphicComponent::GraphicComponent()
:animated(false), shaderUsage(false)
{}

GraphicComponent::~GraphicComponent()
{
    ENGINE->getModule(Engine::ModuleType::MODULE_GRAPHIC)->removeComponent(parent->getName());

    if (animated) {
        delete animation;
    }

    if (shaderUsage) {
        delete shader;
    }
}

void GraphicComponent::setParent(Entity* parent)
{
    BaseComponent::setParent(parent);
    parent->setGraphicComponent(this);
    parent->getOgreEntity()->setCastShadows(true);
}

void GraphicComponent::update(Ogre::Real time)
{
    if (animated) {
        animation->addTime(time);
        Event* e = new Event(Event::PHYSICS_UPDATE);
        parent->receiveEvent(e);

        if (animation->hasEnded()) {
            animated = false;
            Event* animationEnded = new Event(Event::ANIMATION_ENDED);
            this->parent->receiveEvent(animationEnded);
            delete animationEnded;
        }
        delete e;
    }
}

void GraphicComponent::setAnimation(std::string name, bool loop)
{
    animation = parent->getOgreEntity()->getAnimationState(name);
    parent->getOgreEntity()->setDisplaySkeleton(true);

    animation->setLoop(loop);
    animation->setEnabled(true);
    animated = true;

    this->parent->receiveEvent(new Event(Event::ANIMATION_STARTED));
}

void GraphicComponent::setShader(Shader* s)
{    
//    if(shaderUsage) {
//        delete shader;
//    }
    shader = s;
//    shaderUsage = true;
}

void GraphicComponent::reloadShader()
{
    shader->reload();
}