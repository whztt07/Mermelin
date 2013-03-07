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
 * @file Button.cpp
 * @author Eduardo Hahn Paredes
 */

#include "stdafx.h"
#include "Entities/Button.h"
#include "Modules/LogicModule.h"
#include "Engine.h"

using namespace CotopaxiEngine;

Button::Button(std::string name, Ogre::SceneNode* parentNode)
: Entity(name, "Button", parentNode)
{
    trigger = ENGINE->getLogic()->createTrigger(this, name);
    this->addComponent(trigger);
    this->registerListener(Event::COLLISION_ENTER, trigger);

    audioComponent = dynamic_cast<AudioComponent*> (ENGINE->getAudio()->getComponent(this));
    this->addComponent(audioComponent);
    audioComponent->addSound("button");
}

Button::~Button()
{
    delete audioComponent;
    delete trigger;
}

void Button::receiveEvent(Event* event)
{
    if (event->getType() == Event::COLLISION_ENTER) {
        audioComponent->play("button", false);
        trigger->receiveEvent(event);
    }
}